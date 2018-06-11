// Example 52.3 PCF8591 ADC demo
// http://tronixstuff.com/2013/06/17/tutorial-arduino-and-pcf8591-adc-dac-ic/ Chapter 52
// John Boxall June 2013
#include "Wire.h"
#define PCF8591 (0x90 >> 1) // I2C bus address
#define ADC0 0x00 // control bytes for reading individual ADCs
#define ADC1 0x01
#define ADC2 0x02
#define ADC3 0x03
#define DAC0 0x40
#define ADC 0x04 // control byte - read ADC0 then auto-increment
#define Inc 2
byte value0, value1, value2, value3;

int Dac = 0; //for DAC value
bool rising = 1; //rising of falling Dac value

void setup() 
{
  // put your setup code here, to run once:
 Wire.begin();
 Serial.begin(9600);
}

void loop() 
{
 read_ADC();
 write_DAC(Dac);
 
 //update DAC value
 if (rising)
 {
    Dac = Dac + Inc;
    if (Dac >= 255) rising = 0;
 }
 else if (!rising)
 {
    Dac = Dac - Inc;
    if (Dac <= 1) rising = 1;
 }
 
 SerialDisplay();
 delay(10);
}

void read_ADC()
{
 Wire.beginTransmission(PCF8591); // wake up PCF8591
 Wire.write(ADC); // control byte
 Wire.endTransmission(); // end tranmission
 Wire.requestFrom(PCF8591, 5);
 
 value0=Wire.read();
 value0=Wire.read();
 value1=Wire.read();
 value2=Wire.read();
 value3=Wire.read();  
}

void write_DAC(int j)
{
 Wire.beginTransmission(PCF8591); // wake up PCF8591
 Wire.write(DAC0); // control byte - turn on DAC (binary 1000000)
 Wire.write(j); // value to send to DAC
 Wire.endTransmission(); // end tranmission
}

void SerialDisplay()
{
 Serial.print("value0 (Rp): "); Serial.print(value0); Serial.print(" ");
 Serial.print("value1 (R5): "); Serial.print(value1); Serial.print(" ");
 Serial.print("value2 (R6): "); Serial.print(value2); Serial.print(" ");
 Serial.print("value3: "); Serial.print(value3); Serial.print(" ");
 Serial.print("i: "); Serial.print(Dac);
 Serial.println();
}

