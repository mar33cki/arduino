/*
 * configuration.c
 *
 * Created: 2014-05-10 13:12:23
 *  Author: Bartek
 *  v1.1 ported to arduino - MASZ, 2016.05.23
 */ 

#include "configuration.h"
#include <EEPROM.h>
//#include <avr/eeprom.h>

#define ADDR0 0x10

volatile unsigned char diffOn = 3;
volatile unsigned char diffOff = 7;
volatile unsigned char pumpMode = 0;
volatile unsigned char holidayMode = 0;
//volatile unsigned char limitTempL = 0;
//volatile unsigned char limitTempH = 0;

void configurationSave( void )
{
  
  EEPROM.write( ADDR0, diffOn );
  EEPROM.write( 0x11, diffOff );
  EEPROM.write( 0x12, pumpMode );
  EEPROM.write( 0x13, holidayMode );
  //EEPROM.write( (unsigned char*)0x14, limitTempL );
  //EEPROM.write( (unsigned char*)0x15, limitTempH );
}

void configurationWriteDefaults( void )
{
  EEPROM.write( 0x10, DEFAULT_DIFF_ON );
  EEPROM.write( 0x11, DEFAULT_DIFF_OFF );
  EEPROM.write( 0x12, DEFAULT_PUMP_MODE );
  EEPROM.write( 0x13, DEFAULT_HOLIDAY_MODE );
  //EEPROM.write( (unsigned char*)0x14, DEFAULT_LIMIT_TEMP_L );
  //EEPROM.write( (unsigned char*)0x15, DEFAULT_LIMIT_TEMP_H );
}

void configurationLoad( void )
{
  diffOn = EEPROM.read( 0x10 );
  diffOff = EEPROM.read( 0x11 );
  pumpMode = EEPROM.read( 0x12 );
  holidayMode = EEPROM.read( 0x13 );
  //limitTempL = EEPROM.read( (unsigned char*)0x14 );
  //limitTempH = EEPROM.read( (unsigned char*)0x15 );
}
