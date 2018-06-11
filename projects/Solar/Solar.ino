/* YourDuino.com Example Software Sketch
16 character 2 line I2C Display
ANOTHER NEW TYPE Marked "LCM1602 IIC  A0 A1 A2"
A0-A1-A2 are grounded so I2C Address is 0x20
terry@yourduino.com
MASZ change from 20x4 to 16x2*/
/*-----( Import needed libraries )-----*/
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>  // F Malpartida's NewLiquidCrystal library
//Download: https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move original LiquidCrystal library elsewhere, copy this in it's place
#include <EEPROM.h>
#include <avr/wdt.h>

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

//--------------------------------------
//EEPROM default data
#define DEFAULT_DIFF_ON       5
#define DEFAULT_DIFF_OFF      2
#define DEFAULT_PUMP_MODE     0
#define DEFAULT_HOLIDAY_MODE  0
#define DEFAULT_LIMIT_TEMP_L  10
#define DEFAULT_LIMIT_TEMP_H  80
//--------------------------------------

//--------------------------------------
// display const chars
const char string_helloWorld1[] PROGMEM = "Termostat :) 1.2";
const char string_defaults1[] PROGMEM   = "Default loaded.";
  
const char string_sensor1Value[] PROGMEM =            "Temp 1 ";
const char string_sensor2Value[] PROGMEM =            "Temp 2 ";
const char string_diffOn[] PROGMEM =           "Roz.zal";
const char string_diffOff[] PROGMEM =          "Roz.wyl";
//const char string_limitTL[] PROGMEM =          "Tmin   ";
//const char string_limitTH[] PROGMEM =          "Tmax   ";
const char string_holiday[] PROGMEM =          "Tr.wak.";
const char string_pump[] PROGMEM =             "Pompa  ";

const char string_onAutomatic[] PROGMEM =      " zal.(A)";
const char string_offAutomatic[] PROGMEM =     " wyl.(A)";
const char string_onManual[] PROGMEM =         " zal.(M)";
const char string_offManual[] PROGMEM =        " wyl.(M)";

const char string_yes[] PROGMEM =              " tak    ";
const char string_no[] PROGMEM =               " nie    ";

const char string_filler3[] PROGMEM =          "   ";
//--------------------------------------
/* CONFIGURATION */
#define DISPLAY_TIMER_MAX             30
#define MAX_DIFF                      30
//#define MAX_LIMIT_TEMP                110

/* DO NOT MODIFY THOSE DEFINES */
#define CYCLE_TEMP_1                  0
#define CYCLE_TEMP_2                  1
#define CYCLE_HIST_ON                 2
#define CYCLE_HIST_OFF                3
//#define CYCLE_TEMP_LIMIT_L            4
//#define CYCLE_TEMP_LIMIT_H            5
#define CYCLE_HOLIDAY                 4
#define CYCLE_PUMP                    5
#define NUMBER_OF_CYCLES              6

#define PUMP_MODE_AUTO                0
#define PUMP_MODE_MAN_ON              1
#define PUMP_MODE_MAN_OFF             2
#define PUMP_MODE_MAX                 2
//--------------------------------------
//PIN configuration
//analog input
int sensorT1 = A0;    //AD0, select the input pin for the temp sensor 1 - SOLAR
int sensorT2 = A1;    //AD1, select the input pin for the temp sensor 2 - BOILER BOTTOM
int sensorT3 = A2;    //AD2, select the input pin for the temp sensor 3 - BOILER TOP
//digital output
int pumpSolar = 2;    //PD2, select the pin for the relay1 - SOLAR:BOILER pump
int pumpHEater = 3;   //PD3, select the pin for the relay2 - ELECTRIC HEATER
int pumpEMERGENCY = 4;   //PD4, select the pin for the relay2 - EMERGENCY PUMP when overheated
//digital input
int button1 = 8;  //button 1 PB0/D8
int button2 = 9;  //button 2 PB1/D9

//dynamic values
volatile int sensor1Value = 0;  // variable to store the value coming from the sensor 1 - SOLAR, previously sensor1Value
volatile int sensor2Value = 0;  // variable to store the value coming from the sensor 2 - BOILER BOTTOM, previously sensor2Value
volatile int sensor3Value = 0;  // variable to store the value coming from the sensor 3 - BOILER TOP
volatile int pumpSolarValue = 0;     // variable to store the value coming for pump output - SOLAR:BOILER, previously pumpSolarValue
volatile int pumpHEaterValue = 0;    // variable to store the value coming for pump output - ELECTRIC HEATER
volatile int pumpEMERGENCYValue = 0; // variable to store the value coming for pump output - EMERGENCY PUMP when overheated

/* System variables */
int diffOn = 5;         // temperature difference to set LED/relay on
int diffOff = 1;        // temperature difference to set LED/relay off
int pumpMode = 0;       // pump mode (Auto, Manual On/Off)
int holidayMode = 0;    // normal (0)/holiday (1) mode 
int limitTempL = 1;     // temperature difference low limit
int limitTempH = 5;     // temperature difference high limit
int heaterTempMin = 35; //min temperature when heater relay should be on
int heaterTempMax = 65; //max temperature when heater relay should be off
volatile unsigned char cycle = 0;


/* Helpers */
volatile unsigned char displayTimer = 0;
volatile unsigned char refreshLcd = 0;
volatile unsigned char prevButtonPressed = 0;
volatile unsigned char saveToEepromPending = 0;

/*-----( Declare objects )-----*/
LiquidCrystal_I2C  lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);


void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  //serial debug
  Serial.begin(57600);
  Serial.println(F("Entering Setup"));
  //LCD section
  lcd.begin (16, 2); // initialize the lcd
  // Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(LED_ON);
  // Reset the display
  lcd.clear();
  lcd.home();
  lcd.print(string_helloWorld1);
  lcd.setBacklight(LED_OFF);
  Serial.println(F("Setup.print"));
  delay(1000);

  //Digital Output
  pinMode(pumpSolar, OUTPUT);
  pinMode(pumpHEaterValue, OUTPUT);
  pinMode(pumpEMERGENCYValue, OUTPUT);
  Serial.println(F("Setup.DO"));
  //Digital Input
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  Serial.println(F("Setup.DI"));
}// END Setup

//--------------------------------------
void loop()   /*----( LOOP: RUNS OVER AND OVER AGAIN )----*/
{
  /*
  ReadADC();
  
  // Print our characters on the LCD
  lcd.backlight();  //Backlight ON if under program control
  lcd.setCursor(1, 0); //Start at character 1 on line 0
  lcd.print("ADC values ['C]:");
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print(sensor1Value);
  lcd.setCursor(5, 1);
  lcd.print(sensor2Value);
  delay(500);
*/
  Serial.println(F("Loop.enter"));
  EPROMconfigurationLoad();
    
  wdt_enable(WDTO_500MS); // Enable Watch Dog Timer
  Serial.println(F("Loop.Enable WatchDogTimer"));
  
  sei(); // Enable global interrupts
  Serial.println(F("Loop.Enable global interrupts"));
  
  while(1)
  {
    wdt_reset(); // Reset Watch Dog Timer
    Serial.println(F("Loop.while.wdt_reset"));
  
  if( refreshLcd )
    {
      refreshLcd = 0;
      showLcd();
      Serial.println(F("Loop.while.refreshLcd"));
    }
    
    checkButtons();
    ReadADC();
    if( saveToEepromPending == 1 )
      EPROMConfigurationSave();
    
    // Computing part
    if( pumpMode == PUMP_MODE_AUTO )
    {
      uint8_t diff = sensor2Value > sensor1Value ? sensor2Value - sensor1Value : sensor1Value - sensor2Value; // Absolute value of difference

      if( holidayMode == 0 ) 
      {
        // Holiday mode OFF
        // Note that sensor1Value and sensor2Value are unsigned
        
        if( sensor1Value >= sensor2Value && diff >= diffOn )
          pumpSolarValue = 1;
          
        if( sensor1Value >= sensor2Value && diff <= diffOff )
          pumpSolarValue = 0;
          
        if( sensor1Value <= sensor2Value )
          pumpSolarValue = 0;
      }
      else
      {
        // Holiday mode ON
        if( diff >= diffOn )
          pumpSolarValue = 1;
        
        if( diff <= diffOff )
          pumpSolarValue = 0;
          
      }
    }

    // Handling limit situations
   /*remove v1.2
    if( sensor1Value > limitTempH )
      pumpSolarValue = 1;
    if( sensor1Value < limitTempL )
      pumpSolarValue = 1;
    if( sensor2Value > limitTempH )
      pumpSolarValue = 1;
    if( sensor2Value < limitTempL )
      pumpSolarValue = 1;
    */

    if( pumpMode == PUMP_MODE_MAN_ON )
      pumpSolarValue = 1;
    if( pumpMode == PUMP_MODE_MAN_OFF )
      pumpSolarValue = 0;
      
    // Action part
    if( pumpSolarValue )
      digitalWrite(pumpSolar, HIGH);
    else
      digitalWrite(pumpSolar, LOW);
  }
  delay(5000);
} // END Loop
//--------------------------------------
/*
void Display()
{
  
}*/
//--------------------------------------
void checkButtons(void)
{
  Serial.println(F("checkButtons"));
  uint8_t buttonPressed = 0;
  if( digitalRead(button1) == 0 ) // Button 1 pressed 
    buttonPressed = 1;
  if( digitalRead(button2) == 0 ) // Button 2 pressed 
    buttonPressed = 2;

    Serial.print(F("button1:"));
    Serial.println(digitalRead(button1));
    Serial.print(F("button2:"));
    Serial.println(digitalRead(button2));
  // Handle case when both two buttons are pressed -> loading defaults and reset
  if ( ( button1 == 0 ) && ( button2 == 0 ) )
  {
    EPROMconfigurationWriteDefaults();
    EPROMconfigurationLoad();
    lcd.setCursor(1,0);
    lcd.print(string_defaults1);
    _delay_ms(1000);

    software_Reboot();
  }
    
  if( prevButtonPressed == buttonPressed ) // To force that every click causes an action - not hold
    return;
  
  if( cycle == CYCLE_HIST_ON && buttonPressed == 1 && diffOn < MAX_DIFF )
    diffOn++;
  if( cycle == CYCLE_HIST_ON && buttonPressed == 2 && diffOn > 0 )
    diffOn--;
    
  if( cycle == CYCLE_HIST_OFF && buttonPressed == 1 && diffOff < (diffOn-1) )
  diffOff++;
  if( cycle == CYCLE_HIST_OFF && buttonPressed == 2 && diffOff > 0 )
  diffOff--;
  
  /* removed 1.2
  if( cycle == CYCLE_TEMP_LIMIT_L && buttonPressed == 1 && limitTempL < MAX_LIMIT_TEMP-1 )
    limitTempL++;
  if( cycle == CYCLE_TEMP_LIMIT_L && buttonPressed == 2 && limitTempL > 0 )
    limitTempL--;
  
  if( cycle == CYCLE_TEMP_LIMIT_H && buttonPressed == 1 && limitTempH < MAX_LIMIT_TEMP-1 )
    limitTempH++;
  if( cycle == CYCLE_TEMP_LIMIT_H && buttonPressed == 2 && limitTempH > 0 )
    limitTempH--;
  */


  if( cycle == CYCLE_HOLIDAY && buttonPressed != 0 )
    holidayMode ^= 0x01;
    
  if( cycle == CYCLE_PUMP && buttonPressed == 1 && pumpMode < PUMP_MODE_MAX )
    pumpMode++;
  if( cycle == CYCLE_PUMP && buttonPressed == 2 && pumpMode > 0 )
    pumpMode--;

  if( buttonPressed != 0 )
  {
    displayTimer = 0; // Do not change LCD mode for a time after button was pressed
    refreshLcd = 1; // And refresh LCD to reflect current value on display
    saveToEepromPending = 1;
  }
  else saveToEepromPending = 0;
  prevButtonPressed = buttonPressed;
}
//--------------------------------------
void software_Reboot()
{
  Serial.print(F("software_Reboot"));
  wdt_enable(WDTO_15MS);
  while(1)
  {
  }
}
//--------------------------------------
void showLcd( void )
{
  Serial.print(F("showLcd"));
  Serial.print("showLcd.cycle:");
  Serial.println(cycle);
  switch( cycle )
  {
    case CYCLE_TEMP_1:
      lcd.setCursor(1,0);
      lcd.print(string_sensor1Value);
      lcd.setCursor(0,1);
      lcd.print(' ');
      lcd.print( sensor1Value, 2 );
      lcd.print( 0xDF );
      lcd.print( 'C' );
      lcd.print(string_filler3);
      break;
    case CYCLE_TEMP_2:
      lcd.setCursor(1,0);
      lcd.print(string_sensor2Value);
      lcd.setCursor(0,1);
      lcd.print(' ');
      lcd.print( sensor1Value, 2 );
      lcd.print( 0xDF );
      lcd.print( 'C' );
      lcd.print(string_filler3);
      break;
    case CYCLE_HIST_ON:
      lcd.setCursor(1,0);
      lcd.print(string_diffOn);
      lcd.setCursor(0,1);
      lcd.print(' ');
      lcd.print( diffOn, 2 );
      lcd.print( 0xDF );
      lcd.print( 'C' );
      lcd.print(string_filler3);
      break;
    case CYCLE_HIST_OFF:
      lcd.setCursor(1,0);
      lcd.print(string_diffOff);
      lcd.setCursor(0,1);
      lcd.print(' ');
      lcd.print( diffOff, 2 );
      lcd.print( 0xDF );
      lcd.print( 'C' );
      lcd.print(string_filler3);
      break;
  /* removed v1.2
    case CYCLE_TEMP_LIMIT_L:
      lcd.setCursor(1,0);
      lcd.print(string_limitTL);
      lcd.setCursor(0,1);
      lcd.print(' ');
      lcd.print( limitTempL, 3 );
      lcd.print( 0xDF );
      lcd.print( 'C' );
      lcd.print(string_filler3);
      break;
    case CYCLE_TEMP_LIMIT_H:
      lcd.setCursor(1,0);
      lcd.print(string_limitTH);
      lcd.setCursor(0,1);
      lcd.print(' ');
      lcd.print( limitTempH, 3 );
      lcd.print( 0xDF );
      lcd.print( 'C' );
      lcd.print(string_filler3);
      break;
    */
    case CYCLE_HOLIDAY:
      lcd.setCursor(1,0);
      lcd.print(string_holiday);
      lcd.setCursor(0,1);
      if( holidayMode == 1 )
        lcd.print(string_yes);
      else
        lcd.print(string_no);
      break;
    case CYCLE_PUMP:
      lcd.setCursor(1,0);
      lcd.print(string_pump);
      lcd.setCursor(0,1);
      if( pumpMode == PUMP_MODE_AUTO && pumpSolar == 1 )
        lcd.print(string_onAutomatic );
      if( pumpMode == PUMP_MODE_AUTO && pumpSolar == 0 )
        lcd.print(string_offAutomatic );
      if( pumpMode == PUMP_MODE_MAN_ON )
        lcd.print(string_onManual);
      if( pumpMode == PUMP_MODE_MAN_OFF )
        lcd.print(string_offManual);
      break;
    default:
      lcd.setCursor(1,0);
      lcd.print('?');
      break;
  } // end of switch
}
//--------------------------------------
void ReadADC()
{
  Serial.print(F("ReadADC"));
  // read the value from the sensor LM35, and scale to Celcius Degree
  sensor1Value = analogRead(sensorT1)/4; //scale to Celcius Degree
  sensor2Value = analogRead(sensorT2)/4; //scale to Celcius Degree
  Serial.print(F("sensor1Value:"));
  Serial.println(sensor1Value);
  Serial.print(F("sensor2Value:"));
  Serial.println(sensor2Value);
}
//--------------------------------------
//EEPROM operations
void EPROMConfigurationSave()
{
 Serial.print(F("EPROMConfigurationSave"));
 EEPROM.write( 0x10, diffOn );
 EEPROM.write( 0x11, diffOff );
 EEPROM.write( 0x12, pumpMode );
 EEPROM.write( 0x13, holidayMode );
 EEPROM.write( 0x14, limitTempL );
 EEPROM.write( 0x15, limitTempH );
 EEPROM.write( 0x16, heaterTempMin );
 EEPROM.write( 0x17, heaterTempMax );
}

void EPROMconfigurationWriteDefaults( void )
{
  Serial.print(F("EPROMconfigurationWriteDefaults"));
  EEPROM.write( 0x10, DEFAULT_DIFF_ON );
  EEPROM.write( 0x11, DEFAULT_DIFF_OFF );
  EEPROM.write( 0x12, DEFAULT_PUMP_MODE );
  EEPROM.write( 0x13, DEFAULT_HOLIDAY_MODE );
  EEPROM.write( 0x14, DEFAULT_LIMIT_TEMP_L );
  EEPROM.write( 0x15, DEFAULT_LIMIT_TEMP_H );
}

void EPROMconfigurationLoad( void )
{
  Serial.print(F("EPROMconfigurationLoad"));
  diffOn = EEPROM.read( 0x10 );
  diffOff = EEPROM.read( 0x11 );
  pumpMode = EEPROM.read( 0x12 );
  holidayMode = EEPROM.read( 0x13 );
  limitTempL = EEPROM.read( 0x14 );
  limitTempH = EEPROM.read( 0x15 );
}
//--------------------------------------
