/*
 * configuration.h
 *
 * Created: 2014-05-10 13:13:31
 *  Author: Bartek
 */ 


#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

//#include <avr/io.h>
//#include <avr/wdt.h>

#define DEFAULT_DIFF_ON       5
#define DEFAULT_DIFF_OFF      2
#define DEFAULT_PUMP_MODE     0
#define DEFAULT_HOLIDAY_MODE  0
//#define DEFAULT_LIMIT_TEMP_L  10
//#define DEFAULT_LIMIT_TEMP_H  90

#ifndef RESET_WHOLE_CPU
#define RESET_WHOLE_CPU wdt_enable(WDTO_30MS); while(1) {}
#endif

extern volatile unsigned char diffOn;
extern volatile unsigned char diffOff;
extern volatile unsigned char pumpMode;
extern volatile unsigned char holidayMode;
//extern volatile unsigned char limitTempL;
//extern volatile unsigned char limitTempH;

void configurationSave( void );
void configurationWriteDefaults( void );
void configurationLoad( void );

#endif /* CONFIGURATION_H_ */
