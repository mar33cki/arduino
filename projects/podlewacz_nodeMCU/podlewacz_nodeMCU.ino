/*
 * this sketch is to switch on watering sections, controlled by relay[]
 * it works on specified StartHour:StartMinute, launching each section for WateringTime
 * last section is enabled with section 0, and set off when last section is off
 * it is intended for drop watering
 * section0---------------------------| drop valve
 * section3|                            watering valve
 *          |section1|                  watering valve
 *                   |section2|         watering valve
 *                            |all off| watering valve
*/
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>


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

#define OUTPUTS 4 //max loop index
#define StartHour 07
#define StartMinute 00
#define WateringTime 10
#define JSON_BUFF_DIMENSION 2500

int StartWatering = 0, CurrentTimer = 0, ind =0, AllowWatering = 0;
int relays[OUTPUTS] = {D0, D1, D2, D3};
int ForceWatering = D5; //user button to force the watering
int ForceWateringState = 0, PreviousForceWateringState = 0; //user button state

const char *ssid     = "ssid";
const char *password = "pass";

WiFiUDP ntpUDP;
int hour, minute;
unsigned long  TimerMinutes = 0, previousMinute = 0;

//europe server, utcOffsetInSeconds (+1:3600, +2:7200), updateInterval (10s)
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 7200, 60000);

WiFiClient client;
// Open Weather Map API server name
const char server[] = "api.openweathermap.org";
// Replace the next line to match your city and 2 letter country code
String idOfCity = "7531860"; 
// Replace the next line with your API Key
String apiKey = "1f41036a759d7b051e4d0338bbc40a07"; 

void Timer()
{
  //get network time
  hour = timeClient.getHours();
  minute = timeClient.getMinutes();

  if(previousMinute != minute) //check if minute elapsed
  { 
    previousMinute = minute; //save current minute
    TimerMinutes++;         //increment timer

    //toggle the LED_BUILTIN
    if(TimerMinutes%2) digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    else digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    
    Serial.print(F("Time: "));Serial.print(hour); Serial.print(F(":")); Serial.print(minute);
    Serial.print(F("; TimerMinutes: ")); Serial.println(TimerMinutes);
  }
}

void watering()
{
  ForceWateringState = digitalRead(ForceWatering);
  if(PreviousForceWateringState != ForceWateringState) { Serial.print(F("ForceWateringState: ")); Serial.println(ForceWateringState);}
  PreviousForceWateringState = ForceWateringState;

  //wait for StartHour:StartMinute and StartWatering or for user button ForceWateringState
  if( (hour == StartHour && minute == StartMinute && StartWatering == 0 && AllowWatering = 1) || ForceWateringState == 0)
  { 
    StartWatering = 1;
    CurrentTimer = TimerMinutes + WateringTime;
    ind = 0;
    digitalWrite(relays[ind], LOW); //section0 relay ON
    ind++;
    digitalWrite(relays[ind], LOW); //section1 relay ON
    Serial.print(F("Time for watering!, section = 0, 3, Wait for ")); Serial.println(CurrentTimer);
  }

 //watering on-going - iterate over the sections (relays)
 if( StartWatering && CurrentTimer == TimerMinutes && ind != OUTPUTS)
 {   
   digitalWrite(relays[ind], HIGH); //running relay OFF
   ind++;
   digitalWrite(relays[ind], LOW);  //next relay ON
   CurrentTimer = TimerMinutes + WateringTime;
   Serial.print(F("section = ")); Serial.print(ind); Serial.print(F(", Wait for ")); Serial.println(CurrentTimer);
 }
 
 //stop the watering
 else if(ind == OUTPUTS)
 {
  for(int i = 0; i < OUTPUTS; i++) digitalWrite(relays[i], HIGH); //all relays OFF
  StartWatering = 0;
  Serial.println(F("Stop the watering! "));
  ind = 0;
 }
}

// to request data from OWM
void makehttpRequest() {
  // close any connection before send a new request to allow client make connection to server
  client.stop();

  // if there's a successful connection:
  if (client.connect(server, 80)) {
    // Serial.println("connecting...");
    // send the HTTP PUT request:
    client.println("GET /data/2.5/forecast?id=" + idOfCity + "&APPID=" + apiKey + "&mode=json&units=metric&cnt=2 HTTP/1.1");
    client.println("Host: api.openweathermap.org");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();
    
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
    
    char c = 0;
    while (client.available()) {
      c = client.read();
      // since json contains equal number of open and close curly brackets, this means we can determine when a json is completely received  by counting
      // the open and close occurences,
      //Serial.print(c);
      if (c == '{') {
        startJson = true;         // set startJson true to indicate json message has started
        jsonend++;
      }
      if (c == '}') {
        jsonend--;
      }
      if (startJson == true) {
        text += c;
      }
      // if jsonend = 0 then we have have received equal number of curly braces 
      if (jsonend == 0 && startJson == true) {
        parseJson(text.c_str());  // parse c string text in parseJson function
        text = "";                // clear text string for the next time
        startJson = false;        // set startJson to false to indicate that a new message has not yet started
      }
    }
  }
  else {
    // if no connction was made:
    Serial.println("connection failed");
    return;
  }
}

void setup(){
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ForceWatering, INPUT_PULLUP);
  for(int i = 0; i < OUTPUTS; i++) 
  { 
    pinMode(relays[i], OUTPUT);
    digitalWrite(relays[i], HIGH); //relays OFF
  }

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) 
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay ( 500 );
    Serial.print ( F(".") );
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  }
  Serial.println(F("Connected!"));

  timeClient.begin();
  Serial.print(F("Watering at: ")); Serial.print(StartHour); Serial.print(F(":")); Serial.print(StartMinute);
  Serial.print(F(" for ")); Serial.print(WateringTime); Serial.println(F(" min/section."));

  text.reserve(JSON_BUFF_DIMENSION);
}

void loop() 
{
  timeClient.update();

  Timer();
  makehttpRequest();
  watering();

  delay(1000);
}
