#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>

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
#define DHTTYPE   DHT22       // DHT type (DHT11, DHT22)
#define DHTPIN    D3          // Broche du DHT / DHT Pin
#define LEDPIN    LED_BUILTIN          // Led
#define REL1      D5            //relay 1
#define REL2      D6            //relay 2

float   t_dht = 0 ; //DHT temperature
float   h_dht = 0 ; //DHT humidity
float   p_bmp = 0;  //BMP pressure
float   t_bmp = 0;  //BMP temperature

int count = 0;
int ledState = LOW; 

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;


void setup() {
  // put your setup code here, to run once:
  Serial.println(); 
  Serial.println();
  Serial.begin ( 115200 );
  
  // Initialisation du BMP180 / Init BMP180
  if ( !bmp.begin() ) 
  {
    Serial.println(F("BMP180 NOK!"));
    while(1);
  } 
  else Serial.println(F("BMP180 OK"));

  dht.begin();
  Serial.println(F("DHT init"));
  
  pinMode(LEDPIN, OUTPUT);
  pinMode(REL1, OUTPUT);
  pinMode(REL2, OUTPUT);
  digitalWrite(REL1, HIGH);
  digitalWrite(REL2, HIGH);
  
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
}

void loop() {
  
  // DHT readings
  t_dht = dht.readTemperature();
  h_dht = dht.readHumidity();
  // BMP readings
  p_bmp = bmp.readPressure(); 
  p_bmp /= 100; //scale to hPa
  t_bmp = bmp.readTemperature();

   if(isnan(h_dht)) Serial.print(F("DHT.Humidity: fail, "));
  else
  {
    Serial.print(F("DHT.Humidity: ")); Serial.print(h_dht); Serial.print(F(", "));
  }
  
  if(isnan(t_dht)) Serial.print(F("DHT.Temperature: fail, "));
  else
  {
    Serial.print(F("DHT.Temperature: ")); Serial.print(t_dht); Serial.print(F(", "));
  }
    
  if(isnan(t_bmp)) Serial.print(F("BMP.Temperature: fail, "));
  else
  {
    Serial.print(F("BMP.Temperature: ")); Serial.print(t_bmp); Serial.print(F(", "));
  }
  
  if(isnan(p_bmp)) Serial.print(F("BMP.Pressure: fail, "));
  else 
  {
    Serial.print(F("BMP.Pressure: ")); Serial.print(p_bmp); Serial.print(F(", "));
  }
  
  Serial.println();
  
//  if (count == 0)
//  {
//    digitalWrite(REL1, LOW);
//    digitalWrite(REL2, LOW);
//    Serial.println(F("0, REL1: 0, REL2: 0"));  
//    count ++;
//  }  
//  else if (count == 1)
//  {
//    digitalWrite(REL1, HIGH);
//    digitalWrite(REL2, LOW);
//    Serial.println(F("1, REL1: 1, REL2: 0"));  
//    count ++;
//  }
//  else if (count == 2)
//  {
//    digitalWrite(REL1, LOW);
//    digitalWrite(REL2, HIGH);
//    Serial.println(F("2, REL1: 0, REL2: 1")); 
//    count ++; 
//  }
//  else if (count == 3)
//  {
//    digitalWrite(REL1, HIGH);
//    digitalWrite(REL2, HIGH);
//    Serial.println(F("3, REL1: 1, REL2: 1"));
//    count ++; 
//  }
//  else
//  {
//    count = 0;
//    Serial.println();
//  }
  delay(5000);
}
