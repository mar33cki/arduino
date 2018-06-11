/*
  Analog Input
 Demonstrates analog input by reading an analog sensor on analog pin 0 and
 turning on and off a light emitting diode(LED)  connected to digital pin 13.
 The amount of time the LED will be on and off depends on
 the value obtained by analogRead().

 The circuit:
 * Potentiometer attached to analog input 0
 * center pin of the potentiometer to the analog pin
 * one side pin (either one) to ground
 * the other side pin to +5V
 * LED anode (long leg) attached to digital output 13
 * LED cathode (short leg) attached to ground

 * Note: because most Arduinos have a built-in LED attached
 to pin 13 on the board, the LED is optional.


 Created by David Cuartielles
 modified 30 Aug 2011
 By Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/AnalogInput

 */

int sensorT1 = A0;    // select the input pin for the temp sensor 1
int sensorT2 = A1;    // select the input pin for the temp sensor 2
int ledPin = 13;      // select the pin for the LED/relay
int sensor1Value = 0;  // variable to store the value coming from the sensor 1
int sensor2Value = 0;  // variable to store the value coming from the sensor 2

int DiffOn = 5; // temperature difference to set LED/relay on
int DiffOff = 1; // temperature difference to set LED/relay off

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // read the value from the sensor:
  sensor1Value = analogRead(sensorT1);
  sensor2Value = analogRead(sensorT2);
  
  if(sensor1Value - sensor2Value > DiffOn)
  {
    // turn the ledPin on
    digitalWrite(ledPin, HIGH);
  }
  else if (sensor1Value - sensor2Value < DiffOff)
  {
    digitalWrite(ledPin, LOW);
  }
  // stop the program for <sensorValue> milliseconds:
  delay(1000);
}
