#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Timers.h> 
#include <PubSubClient.h>
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

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;

#define DEBUG //comment to disable the serial debug

//declare MQTT Publish topics here
#define MQTT_PTOPIC 5555
#define MQTT_PTOPIC_TEMPERATURE 2000
#define MQTT_PTOPIC_HUMIDITY 2001
#define MQTT_PTOPIC_PRESSURE 5558

//declare MQTT Subsribe topics here
#define MQTT_STOPIC       7777
#define MQTT_STOPIC_REL1  1000
#define MQTT_STOPIC_REL2  1001

/*-------------------------------------------------------*/
//threads
#define THREADS 3
Timers <THREADS> thread; //definicja watkow
/*-------------------------------------------------------*/

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;
