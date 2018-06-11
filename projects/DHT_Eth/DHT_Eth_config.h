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

/* constans */
//DHT sensor
#define DHTPIN 2          // numer pinu sygnałowego
#define DHTTYPE DHT11     // typ czujnika (DHT11)
//threads
#define THREADS 3
#define DEBUG //comment to disable the serial debug

/* variables */
//const int ledPin =  LED_BUILTIN;// the number of the LED pin

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};



