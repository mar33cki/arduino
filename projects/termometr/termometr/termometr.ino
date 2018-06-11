#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>  // F Malpartida's NewLiquidCrystal library
//Download: https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads

// This are different than earlier/different I2C LCD displays
#define I2C_ADDR    0x20  // Define I2C Address for the PCF8574T 
//---(Following are the PCF8574 pin assignments to LCD connections )----
// This are different than earlier/different I2C LCD displays
#define BACKLIGHT_PIN  3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7
#define  LED_OFF  1
#define  LED_ON  0

#define AnalogReferenceLevel 1100 // analog reference level in mV, INTERNAL = 1100
//#define AnalogReferenceLevel 5000 // analog reference level in mV, DEFAULT = 5000
#define AnalogResolution 1024

//analog input
int sensorT1 = A0;    // select the input pin for the temp sensor
int ledPin = 13;      // select the pin for the LED


/*-----( Declare objects )-----*/
LiquidCrystal_I2C  lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

void setup() 
{
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);

  //analog reference  built-in reference, equal to 1.1 volts on the ATmega168 or ATmega328 
  analogReference(INTERNAL);

 //LCD section
  lcd.begin (16, 2); // initialize the lcd
  // Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(LED_OFF);
  // Reset the display
  lcd.clear();
  delay(500);
  lcd.home();

  //debug
  Serial.begin(9600);
  Serial.print("Temperature\n");
}

void loop() 
{
  // put your main code here, to run repeatedly:
  //dynamic values
  volatile int sensor1Value = 0;  // variable to store the value coming from the sensor
  
  
  // turn the ledPin on
  digitalWrite(ledPin, HIGH);
  
  sensor1Value = ReadADC(sensorT1);
  LCDdisplay(sensor1Value);
  Serialprint(sensor1Value);

  // turn the ledPin off:
  digitalWrite(ledPin, LOW);
  
  delay(1000);
}
//--------------------------------------
void Serialprint(unsigned int value)
{
  //debug print
  Serial.print("001:");       //identifier
  Serial.print("Temp:");      //name
  Serial.print(value); //value
  Serial.println(";\r");      //new line
  
}
//--------------------------------------
void LCDdisplay(unsigned int value)
{
   // Print our characters on the LCD
  lcd.backlight();  //Backlight ON if under program control
  //lcd.clear();
  lcd.setCursor(0, 0); //Start at character 1 on line 0
  
  lcd.print("Temp:        ");
  //delay(500);
  lcd.setCursor(6, 0);
  lcd.print(value);
  //lcd.print("'C");
  //lcd.setBacklight(LED_OFF);
}
//--------------------------------------
int ReadADC(int AI)
{
  unsigned long ValueRaw = 0, Value = 0;
  // read the value from the sensor LM35, and scale to Celcius Degree
  ValueRaw = analogRead(AI); 
  //sensor1Value = sensor1ValueRaw/4; //scale to Celcius Degree
  Value = CaculateCelcius(ValueRaw);
  return (int) Value;
}
//--------------------------------------
unsigned long CaculateCelcius (unsigned long RawValue)
{
 unsigned long TemperatureCelcius = 0;
 
 //calulate temperature in Celcius
 TemperatureCelcius = (RawValue *  AnalogReferenceLevel);
 TemperatureCelcius = TemperatureCelcius / AnalogResolution;
 //TemperatureCelcius = TemperatureCelcius / 10;
  
 return TemperatureCelcius;
}
//--------------------------------------
