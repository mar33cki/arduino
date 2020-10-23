/*
  WriteMultipleFields
  
  Description: Writes values to fields 1,2,3,4 and status in a single ThingSpeak update every 20 seconds.
  
  Hardware: ESP8266 based boards
  
  !!! IMPORTANT - Modify the secrets.h file for this project with your network connection and ThingSpeak channel details. !!!
  
  Note:
  - Requires ESP8266WiFi library and ESP8622 board add-on. See https://github.com/esp8266/Arduino for details.
  - Select the target hardware from the Tools->Board menu
  - This example is written for a network using WPA encryption. For WEP or WPA, change the WiFi.begin() call accordingly.
  
  ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize, and 
  analyze live data streams in the cloud. Visit https://www.thingspeak.com to sign up for a free account and create a channel.  
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the README.md folder where the library was installed.
  See https://www.mathworks.com/help/thingspeak/index.html for the full ThingSpeak documentation.
  
  For licensing information, see the accompanying license file.
  
  Copyright 2018, The MathWorks, Inc.
*/

#include "ThingSpeak.h"
#include "secrets.h"
#include <ESP8266WiFi.h>
#include "DHT.h"

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

//DHT sensor
#define DHTPIN D1          // numer pinu sygnałowego
#define DHTTYPE DHT22     // typ czujnika (DHT11)

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Initialize our values
int updt = 0;
String myStatus = "";

//DHT sensor
DHT dht(DHTPIN, DHTTYPE); // definicja czujnika

void DHT_Read()
{    
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  static float temp, hum;
  static unsigned int count;
  updt = 0;


  // Sprawdzamy czy są odczytane wartości
  if (isnan(temperature) || isnan(humidity))
  {
    // Jeśli nie, wyświetlamy informację o błędzie
      Serial.println(F("DHT fail!"));
  } 
  else
  {      
    if (humidity != hum || temperature != temp)
    {
      // Jeśli tak, wyświetlamy wyniki pomiaru
        Serial.print(count); Serial.print(F(". "));
        Serial.print(F("Humidity: ")); Serial.print(humidity); Serial.print(F(" % "));
        Serial.print(F("Temperature: ")); Serial.print(temperature); Serial.println(F(" *C"));
        ThingSpeak.setField(1, temperature);
        ThingSpeak.setField(2, humidity);
        updt = 1;
    }
  }
  //keep last readings
  temp = temperature;
  hum = humidity;
  count++;
}

void setup() {
  Serial.begin(115200);  // Initialize serial

  WiFi.mode(WIFI_STA); 
  dht.begin();            // inicjalizacja czujnika
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print(F("Attempting to connect to SSID: "));
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(F("."));
      delay(5000);     
    } 
    Serial.println(F("\nConnected."));
  }

  // set the status
//  ThingSpeak.setStatus(myStatus);

  DHT_Read();
  
  // write to the ThingSpeak channel
  if(updt == 1)
  {
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if(x == 200){
      Serial.println(F("Channel update successful."));
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
  }  
  delay(300000); // Wait 5min to update the channel again
}
