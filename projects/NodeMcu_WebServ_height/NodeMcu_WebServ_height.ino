#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//#include <DHT.h>
//#include <Adafruit_BMP085.h>

#define LED_BUILTIN D4
#define D0 16
#define D1 5 // I2C Bus SCL (clock)
#define D2 4 // I2C Bus SDA (data)
#define D3 0
#define D4 2 // Same as "LED_BUILTIN", but inverted logic
#define D5 14 // SPI Bus SCK (clock)
#define D6 12 // SPI Bus MISO
#define D7 13 // SPI Bus MOSI
#define D8 15 // SPI Bus SS (CS)
#define D9 3 // RX0 (Serial console)
#define D10 1 // TX0 (Serial console)

#define ssid      "ssid"       // WiFi SSID
#define password  "pass"  // WiFi password
//#define DHTTYPE   DHT22       // DHT type (DHT11, DHT22)
//#define DHTPIN    D4          // Broche du DHT / DHT Pin
#define LEDPIN    LED_BUILTIN          // Led
float   t = 0 ;
float   h = 0 ;
float   p = 0;
int  Distance;  // Odległość w centymetrach
String  stateLed = "OFF";
//sonar
int Trig = D0;   // Numer pinu wyzwolenia
int Echo = D1;   // Numer pinu odpowiedzi
int Valve1 = D2;  // electro valve 1
int Valve2 = D3;  // electro valve 2
int Valve1State = 0, Valve2State = 0;

// Création des objets / create Objects
//DHT dht(DHTPIN, DHTTYPE);
//Adafruit_BMP085 bmp;
ESP8266WebServer server ( 80 );

String getPage()
{
  String page = "<html lang=en-EN><head><meta http-equiv='refresh' content='10'/>";
  page += "<title>ESP8266 Demo - www.projetsdiy.fr</title>";
  page += "<style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style>";
  page += "</head><body><h1>ESP8266 Demo</h1>";
//  page += "<h3>DHT22</h3>";
//  page += "<ul><li>Temperature : ";
//  page += t;
//  page += "°C</li>";
//  page += "<li>Humidite : ";
//  page += h;
//  page += "%</li></ul><h3>BMP180</h3>";
//  page += "<ul><li>Pression atmospherique : ";
//  page += p;
//  page += " mbar</li></ul>";
  page += "<h3>Sonar</h3>";
  page += "<ul><li>Distance : ";
  page += Distance;
  page += " cm</li></ul>";
  page += "<ul><li>Valve 1 : ";
  if(!Valve1State) page += " ON"; // 0 means ON
  else page += " OFF";  // 1 means OFF
  //page += Valve1State;
  page += "</li></ul>";
  page += "<ul><li>Valve 2 : ";
  if(!Valve2State) page += " ON";  // 0 means ON
  else page += " OFF";  // 1 means OFF
  //page += Valve2State;
  page += "</li></ul>";
  page += "<h3>GPIO</h3>";
  page += "<form action='/' method='POST'>";
  page += "<ul><li>D3 (state: ";
  page += stateLed;
  page += ")";
  page += "<INPUT type='radio' name='LED' value='1'>ON";
  page += "<INPUT type='radio' name='LED' value='0'>OFF</li></ul>";
  page += "<INPUT type='submit' value='Update State'>";
  page += "<br><br><p><a hrf='https://diyprojects.io'>diyprojects.io</p>";
  page += "</body></html>";
  return page;
}
void handleRoot(){ 
  if ( server.hasArg("LED") ) 
  {
    handleSubmit();
  } 
  else 
  {
//    Distance = MeasureDistanceAvg();
    server.send ( 200, "text/html", getPage() );
  }  
}

void handleSubmit() {
  // Actualise le GPIO / Update GPIO 
  String LEDValue;
  LEDValue = server.arg("LED");
  Serial.print("Set GPIO: "); Serial.println(LEDValue);
  if ( LEDValue == "1" ) {
    digitalWrite(LEDPIN, 1);
    stateLed = "On";
    server.send ( 200, "text/html", getPage() );
  } else if ( LEDValue == "0" ) {
    digitalWrite(LEDPIN, 0);
    stateLed = "Off";
    server.send ( 200, "text/html", getPage() );
  } else {
    Serial.println("Err Led Value");
  }
}

//-------------------------------------------
// takes few measurements, returns average
int MeasureDistanceAvg()
{
  int DistanceAvg = 0;
  int samples = 5;
  
  for(int i=0; i<samples; i++) 
  {
     DistanceAvg = DistanceAvg + MeasureDistance();     
  }
  DistanceAvg = DistanceAvg/ samples;
  Serial.print(F("Distance [cm]: ")); Serial.println(DistanceAvg);
  return DistanceAvg;
}
//-------------------------------------------
// Measure single distance in [cm]
int MeasureDistance()
{
  int dist; //odleglosc w cm
  long EchoTime;  // Czas trwania sygnału ECHO
 // Ustawiamy TRIG w stan niski na 2us
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
 
  // Ustawiamy TRIG w stan wysoki na 10us
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
 
  // Ustawiamy TRIG w stan niski - rozpoczynamy pomiar
  digitalWrite(Trig, LOW);
 
  // Odczytujamy czas trwania stanu wysokiego na pinie ECHO
  EchoTime = pulseIn(Echo, HIGH);
 
  // Obliczamy odległość
  dist = EchoTime / 58;
  return dist;
}

void setup() {
  Serial.begin ( 115200 );
  
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(Valve1, OUTPUT);
  pinMode(Valve2, OUTPUT);

  digitalWrite(Valve1, HIGH);
  Valve1State = 1;
  digitalWrite(Valve2, HIGH);
  Valve2State = 1;
  // Initialisation du BMP180 / Init BMP180
//  if ( !bmp.begin() ) {
//    Serial.println("BMP180 KO!");
//    while(1);
//  } else {
//    Serial.println("BMP180 OK");
//  }
  
  pinMode(LEDPIN, OUTPUT); 
  
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);  
  WiFi.begin ( ssid, password );
  // Attente de la connexion au réseau WiFi / Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) 
  {
    digitalWrite(LEDPIN, HIGH);
    delay ( 500 ); 
    digitalWrite(LEDPIN, LOW);
    Serial.print ( "." );
  }
  // Connexion WiFi établie / WiFi connexion is OK
  Serial.println ( F("") ); 
  Serial.print ( F("Connected to ") ); Serial.println ( ssid );
  Serial.print ( F("IP address: ") ); Serial.println ( WiFi.localIP() );

  // On branche la fonction qui gère la premiere page / link to the function that manage launch page 
  server.on ( "/", handleRoot );

  server.begin();
  Serial.println ( F("HTTP server started") );
  
}

void loop() {
  server.handleClient();

  Distance = MeasureDistanceAvg();
  if(Distance > 32)  //turn valve ON
  {
    digitalWrite(Valve1, LOW);
    digitalWrite(Valve2, LOW);
    Valve1State = 0;
    Valve2State = 0;
    Serial.print ( F("Distance > 30, Valves ON : ") ); Serial.print(Valve1State); Serial.print ( F(" : ") ); Serial.println(Valve2State);
  }
  else if(Distance < 30)  //turn valve OFF
  {
    digitalWrite(Valve1, HIGH);
    digitalWrite(Valve2, HIGH);
    Valve1State = 1;
    Valve2State = 1;
    Serial.print ( F("Distance < 30, Valves OFF : ") ); Serial.print(Valve1State); Serial.print ( F(" : ") ); Serial.println(Valve2State);
  }
  
//  t = dht.readTemperature();
//  h = dht.readHumidity();
//  p = bmp.readPressure() / 100.0F;
  delay(100);
}
