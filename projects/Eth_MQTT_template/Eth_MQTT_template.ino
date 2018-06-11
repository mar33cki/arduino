/*
 * config, defines, variables
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


// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
const byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};
IPAddress server(172, 16, 32, 98);

//MQTT DeviceID, User, Password
const char mqttID[] = "DEV_Arduino_1";
const char mqttUser[] = "arduino1";
const char mqttPass[] = "HCfx51Zn";

/*-------------------------------------------------------*/
Timers <THREADS> thread; //definicja watkow
/*-------------------------------------------------------*/
//functions defs
//void EthInit();
//void callback(char* topic, byte* payload, unsigned int length);
//void mqttLOOP();
//void mqttConnect();
//void sendMqtt(int topic, int value);
//void sendMqtt(int topic, float value);
//void ftoa(float Value, char* Buffer);
//void DHT_Read();
//void LED_Blink();



