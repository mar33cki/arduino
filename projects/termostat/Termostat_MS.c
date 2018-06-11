/*
 * Termostat_MS.c
 *
 * Created: 09/05/2014 11:14:48
 *  Author: btomeczk
 *  Version: 1.1 modify checkbuttons() - add else saveToEepromPending = 0;
 *  Version: 1.2 - branch from 1.0 - remove Tmin, Tmax
 */ 
 /*
 * Termostat
 *
 * Created: 20/05/2016 15:02
 *  Author: btomeczk/masz
 *  Version: 1.0 - initial version, ported from Termostat_MS.c v.1.2
 */ 

/*
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include "libHd44780.h"
#include "libAdc.h"
#include "configuration.h"
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

#define RELAY_OFF         PORTB &= ~( 1 << 5 ) // Turn off relay
#define RELAY_ON          PORTB |= ( 1 << 5 )  // Turn on relay

/* System variables */
volatile uint8_t cycle = 0;
volatile uint16_t temp1 = 30;
volatile uint16_t temp2 = 45;
volatile uint8_t pumpWorking = 0;
// Configurable variables are defined in configuration.c

/* Helpers */
volatile uint8_t displayTimer = 0;
volatile uint8_t refreshLcd = 0;
volatile uint8_t prevButtonPressed = 0;
volatile uint8_t saveToEepromPending = 0;

ISR(TIMER2_OVF_vect)
{
  cli();
  TCNT2 = 0x00;
  
  if( displayTimer++ >= DISPLAY_TIMER_MAX )
  {
    displayTimer = 0;
    cycle++;
    if( cycle >= NUMBER_OF_CYCLES )
    cycle = 0;
    refreshLcd = 1;  
  }
  
  sei();
}

void checkButtons(void)
{
  uint8_t buttonPressed = 0;
  if( ( PINB & ( 1 << 0 ) ) == 0 ) // Button 1 pressed 
    buttonPressed = 1;
  if( ( PINB & ( 1 << 1 ) ) == 0 ) // Button 2 pressed 
    buttonPressed = 2;
    
  // Handle case when both two buttons are pressed -> loading defaults and reset
  if( ( ( PINB & ( 1 << 0 ) ) == 0 ) && ( ( PINB & ( 1 << 1 ) ) == 0 ) )
  {
    configurationWriteDefaults();
    configurationLoad();
    lcd_gotoxy(1,0);
    lcd_puts_p(string_defaults1);
    lcd_gotoxy(0,1);
    lcd_puts_p(string_defaults2);
    _delay_ms(1000);

    RESET_WHOLE_CPU;
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

void readAdc(void)
{
  cli();
  temp1 = adc_read(0) / 4;
  temp2 = adc_read(1) / 4;
  sei();
}

void showLcd( void )
{
  switch( cycle )
  {
    case CYCLE_TEMP_1:
      lcd_gotoxy(1,0);
      lcd_puts_p(string_temp1);
      lcd_gotoxy(0,1);
      lcd_putc(' ');
      lcd_putNumber( temp1, 2 );
      lcd_putc( 0xDF );
      lcd_putc( 'C' );
      lcd_puts_p(string_filler3);
      break;
    case CYCLE_TEMP_2:
      lcd_gotoxy(1,0);
      lcd_puts_p(string_temp2);
      lcd_gotoxy(0,1);
      lcd_putc(' ');
      lcd_putNumber( temp2, 2 );
      lcd_putc( 0xDF );
      lcd_putc( 'C' );
      lcd_puts_p(string_filler3);
      break;
    case CYCLE_HIST_ON:
      lcd_gotoxy(1,0);
      lcd_puts_p(string_diffOn);
      lcd_gotoxy(0,1);
      lcd_putc(' ');
      lcd_putNumber( diffOn, 2 );
      lcd_putc( 0xDF );
      lcd_putc( 'C' );
      lcd_puts_p(string_filler3);
      break;
    case CYCLE_HIST_OFF:
      lcd_gotoxy(1,0);
      lcd_puts_p(string_diffOff);
      lcd_gotoxy(0,1);
      lcd_putc(' ');
      lcd_putNumber( diffOff, 2 );
      lcd_putc( 0xDF );
      lcd_putc( 'C' );
      lcd_puts_p(string_filler3);
      break;
	/* removed v1.2
    case CYCLE_TEMP_LIMIT_L:
      lcd_gotoxy(1,0);
      lcd_puts_p(string_limitTL);
      lcd_gotoxy(0,1);
      lcd_putc(' ');
      lcd_putNumber( limitTempL, 3 );
      lcd_putc( 0xDF );
      lcd_putc( 'C' );
      lcd_puts_p(string_filler3);
      break;
    case CYCLE_TEMP_LIMIT_H:
      lcd_gotoxy(1,0);
      lcd_puts_p(string_limitTH);
      lcd_gotoxy(0,1);
      lcd_putc(' ');
      lcd_putNumber( limitTempH, 3 );
      lcd_putc( 0xDF );
      lcd_putc( 'C' );
      lcd_puts_p(string_filler3);
      break;
	  */
    case CYCLE_HOLIDAY:
      lcd_gotoxy(1,0);
      lcd_puts_p(string_holiday);
      lcd_gotoxy(0,1);
      if( holidayMode == 1 )
        lcd_puts_p(string_yes);
      else
        lcd_puts_p(string_no);
      break;
    case CYCLE_PUMP:
      lcd_gotoxy(1,0);
      lcd_puts_p(string_pump);
      lcd_gotoxy(0,1);
      if( pumpMode == PUMP_MODE_AUTO && pumpWorking == 1 )
        lcd_puts_p(string_onAutomatic );
      if( pumpMode == PUMP_MODE_AUTO && pumpWorking == 0 )
        lcd_puts_p(string_offAutomatic );
      if( pumpMode == PUMP_MODE_MAN_ON )
        lcd_puts_p(string_onManual);
      if( pumpMode == PUMP_MODE_MAN_OFF )
        lcd_puts_p(string_offManual);
      break;
    default:
      lcd_gotoxy(1,0);
      lcd_putc('?');
      break;
  } // end of switch
}

int main(void)
{
  /* Configuration of IO lines (buttons and relay) */
  DDRB &= ~( 1 << 0 | ( 1 << 1 ) ); // Buttons as inputs
  PORTB |= ( 1 << 0 | ( 1 << 1 ) ); // Enable pull-ups
  RELAY_OFF;
  DDRB |= ( 1 << 5 ); //Relay as output
  
  /* Configure timer2 for timing of changes on LCD */
  TCCR2 |= ( ( 1 << CS22 ) | ( 1 << CS21 ) | ( 1 << CS20 ) );
  TCNT2 = 0x00;
  TIMSK |= ( 1 << TOIE2 );
  
  /* Configure ADC */
  adc_initialize(ERefSource_Internal);
  
  /* LCD initialization and startup line */
  lcd_init( LCD_DISP_ON_CURSOR_BLINK );
  lcd_gotoxy(1,0);
  lcd_puts_p(string_helloWorld1);
  lcd_gotoxy(0,1);
  lcd_puts_p(string_helloWorld2);
  _delay_ms(1000);
  lcd_init(LCD_DISP_ON_CURSOR_BLINK);
  
  configurationLoad();
  
  wdt_enable(WDTO_500MS); // Enable Watch Dog Timer
  
  sei(); // Enable global interrupts
  
  while(1)
  {
    wdt_reset(); // Reset Watch Dog Timer
	
	if( refreshLcd )
    {
      refreshLcd = 0;
      showLcd();
    }
    
    checkButtons();
    readAdc();
    if( saveToEepromPending == 1 )
      configurationSave();
    
    // Computing part
    if( pumpMode == PUMP_MODE_AUTO )
    {
      uint8_t diff = temp2 > temp1 ? temp2 - temp1 : temp1 - temp2; // Absolute value of difference

      if( holidayMode == 0 ) 
      {
        // Holiday mode OFF
        // Note that temp1 and temp2 are unsigned
        
        if( temp1 >= temp2 && diff >= diffOn )
          pumpWorking = 1;
          
        if( temp1 >= temp2 && diff <= diffOff )
          pumpWorking = 0;
          
        if( temp1 <= temp2 )
          pumpWorking = 0;
      }
      else
      {
        // Holiday mode ON
        if( diff >= diffOn )
          pumpWorking = 1;
        
        if( diff <= diffOff )
          pumpWorking = 0;
          
      }
    }

    // Handling limit situations
   /*remove v1.2
    if( temp1 > limitTempH )
      pumpWorking = 1;
    if( temp1 < limitTempL )
      pumpWorking = 1;
    if( temp2 > limitTempH )
      pumpWorking = 1;
    if( temp2 < limitTempL )
      pumpWorking = 1;
	  */

    if( pumpMode == PUMP_MODE_MAN_ON )
      pumpWorking = 1;
    if( pumpMode == PUMP_MODE_MAN_OFF )
      pumpWorking = 0;
      
    // Action part
    if( pumpWorking )
      RELAY_ON;
    else
      RELAY_OFF;
  }
}

