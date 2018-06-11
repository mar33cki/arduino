#include <FastIO.h>
#include <I2CIO.h>
#include <LCD.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal_SR.h>
#include <LiquidCrystal_SR2W.h>
#include <LiquidCrystal_SR3W.h>
#include <EEPROM.h>
#include "configuration.h"

/*
 * Termostat
 *
 * Created: 20/05/2016 15:02
 *  Author: btomeczk/masz
 *  Version: 1.0 - initial version, ported from Termostat_MS.c v.1.2
 */ 

const char string_helloWorld1[] PROGMEM = "Termost";
const char string_helloWorld2[] PROGMEM =  "at :)";
const char string_defaults1[] PROGMEM   = "Default";
const char string_defaults2[] PROGMEM   = " loaded.";
  
const char string_temp1[] PROGMEM =            "Temp 1 ";
const char string_temp2[] PROGMEM =            "Temp 2 ";
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

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
