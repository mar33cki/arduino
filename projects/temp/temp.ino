/* YourDuino.com Example Software Sketch
20 character 4 line I2C Display
ANOTHER NEW TYPE Marked "LCM1602 IIC  A0 A1 A2"
A0-A1-A2 are grounded so I2C Address is 0x20  
terry@yourduino.com */
/*-----( Import needed libraries )-----*/ 
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>  // F Malpartida's NewLiquidCrystal library
//Download: https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move original LiquidCrystal library elsewhere, copy this in it's place

/*-----( Declare Constants )-----*/
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

#define sensorCount 3

//analog input
//int sensors[] = {A0, A1, A2];
#define AnalogReferenceLevel 1100 // analog reference level in mV, INTERNAL = 1100
//#define AnalogReferenceLevel 5000 // analog reference level in mV, DEFAULT = 5000
#define AnalogResolution 1024

//digital input
int sw1Pin = 2;
int sw2Pin = 3;

char c_buff[16];
int sensorValues[sensorCount];  // variable to store the value coming from the sensor

/*-----( Declare objects )-----*/  
LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

void setup() 
{
  //analog reference  built-in reference, equal to 1.1 volts on the ATmega168 or ATmega328 
  analogReference(INTERNAL);

  //debug serial
  Serial.begin(9600);
  Serial.println(F("\n[temp]"));
  
  // put your setup code here, to run once:
  lcd.begin (16,2);  // initialize the lcd 
  // Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(LED_ON);

  //digital inputs
  pinMode(sw1Pin, INPUT_PULLUP);
  pinMode(sw2Pin, INPUT_PULLUP);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  Measure_Display();
  delay(1000);
}
//--------------------------------------
int Measure_Display()
{
  Serial.println(F("Measure_Display: "));

  char buf[16];
  int i; //loop iterator
  int sw1Value, sw2Value;
  int lcdPositions[] = {0, 4, 8};

  //reset the display
  lcd.clear();
  lcd.home();
  lcd.backlight();  //Backlight ON if under program control

  //clean buffer
  clean_c_buff();
  
  for( i = 0; i <= sensorCount; i++)
  {
    sensorValues[i] = ReadADC(i);  //measure
    uiToStr(sensorValues[i], 3, lcdPositions[i]);
  }
  Serial.print(F("c_buff: ")); Serial.println(c_buff);
  lcd.setCursor( 0, 0);
  lcd.print(c_buff);
  //Serial.println();

  sw1Value = digitalRead(sw1Pin);
  Serial.print(F(" sw1:")); Serial.print(sw1Value);
  sw2Value = digitalRead(sw2Pin);
  Serial.print(F(" sw2:")); Serial.print(sw2Value);
  
  Serial.println();
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
 TemperatureCelcius = TemperatureCelcius / 10;
  
 return TemperatureCelcius;
}
//--------------------------------------
void uiToStr(int temp, int t, int k) //temp - liczba, t - il.znakow, k - pozycja kursora
{ 
  int g[]={1,10,100,1000,10000};
  unsigned int temp_mn=g[t-1]; //dzielnik
  int i;
  
  for (i=0;i<t;i++) 
  {
    c_buff[i+k]=(char)temp/temp_mn; //wyluskanie kolejnych znakow
    temp-=(int)temp_mn*c_buff[i+k]; //usuniecie wyzszego znaku
    temp_mn/=10;
    c_buff[i+k]+=48;                //przypisanie kodu cyfr
  }
}
//--------------------------------------
void clean_c_buff() //wypelnia c_buff spacjami, a na ostatniej pozycji znak konca
{
  int i;
  for(i=0; i<15; i++) c_buff[i]=32; //wypelnianie spacjami
  c_buff[15]=0; //na koniec NULL
}
