#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Timers.h> 
#include <PubSubClient.h>

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

#define DEBUG //comment to disable the serial debug

//threads
#define THREADS 3

//declare MQTT Publish topics here
#define MQTT_PTOPIC 5555

//declare MQTT Subsribe topics here
#define MQTT_STOPIC 7777

/*-------------------------------------------------------*/
Timers <THREADS> thread; //definicja watkow
/*-------------------------------------------------------*/

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;
