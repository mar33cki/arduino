/*
 * config, variables
 * 
 */
//headers
#include "DHT.h"
#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
#include <Timers.h> 
#include <PubSubClient.h>

/* constans */
//DHT sensor
#define DHTPIN 2          // numer pinu sygnałowego
#define DHTTYPE DHT11     // typ czujnika (DHT11)
//outputs
#define RELAY1PIN 3
#define RELAY2PIN 4
//threads
#define THREADS 3
#define DEBUG //comment to disable the serial debug
//Publish
#define MQTT_TEMPERATURE 2000
#define MQTT_HUMIDITY 2001
#define MQTT_MOTION 2002

//Subsribe
#define MQTT_RELAY1 1000
#define MQTT_RELAY2 1001

/* variables */
//const int ledPin =  LED_BUILTIN;// the number of the LED pin
long lastReconnectAttempt = 0;

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};
IPAddress server(172, 16, 32, 98);
const char mqttID[] = "DEV_Arduino_1";
const char mqttUser[] = "arduino1";
const char mqttPass[] = "HCfx51Zn";


