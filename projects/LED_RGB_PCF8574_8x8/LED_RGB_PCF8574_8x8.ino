/*
 * This is to control 8x8 LED RGB matrix
 * with 4 PCF8574 modules: RED, GREEN, BLUE, GROUND
 */

#include <Wire.h>    // Required for I2C communication
#include "PCF8574.h" // Required for PCF8574

/** PCF8574 instance */
PCF8574 exp_R, exp_G, exp_B, exp_GND;

static int row[8] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80};
static int col[8] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80};

void exp_init()
{
  int i;

  /* Start I2C bus and PCF8574 instance */
  exp_R.begin(0x20);
  exp_G.begin(0x21);
  exp_B.begin(0x22);
  exp_GND.begin(0x23);
  
  //configure outputs
  for( i = 0; i < 8; i++) 
  {
    exp_R.pinMode(i, OUTPUT);
    exp_G.pinMode(i, OUTPUT);
    exp_B.pinMode(i, OUTPUT);
    exp_GND.pinMode(i, OUTPUT);
  }
  Serial.println(F("exp_x.pinMode - OUTPUT"));
  //configure default state(s)
  WriteDefaultStates();
}

void WriteDefaultStates()
{
  int i;
  for( i = 0; i < 8; i++) 
  {
    exp_R.digitalWrite(i, LOW);
    exp_G.digitalWrite(i, LOW);
    exp_B.digitalWrite(i, LOW);
    exp_GND.digitalWrite(i, LOW);
  }
  Serial.println(F("WriteDefaultStates"));
}

void OneByOne()
{
  int i, j;
  for( i=0; i<8; i++) //column
  {
    exp_GND.digitalWrite(col[i], HIGH);
    for( j=0; j<8; j++)
    {
      exp_R.digitalWrite(row[j], HIGH);
      delay(200);
      exp_R.digitalWrite(row[j], LOW);
      exp_G.digitalWrite(row[j], HIGH);
      delay(200);
      exp_G.digitalWrite(row[j], LOW);
      exp_B.digitalWrite(row[j], HIGH);
      delay(200);
      exp_B.digitalWrite(row[j], LOW);      
    }    
    exp_GND.digitalWrite(col[i], LOW);
    delay(200);
  }
  Serial.println(F("OneByOne"));
}

void AllColor(int color)
{
  int i, j;
  for( i=0; i<8; i++)
  {
    exp_GND.digitalWrite(col[i], HIGH);
    if(color == 1 || color == 0) exp_R.digitalWrite(row[i], HIGH);
    if(color == 2 || color == 0) exp_G.digitalWrite(row[i], HIGH);
    if(color == 3 || color == 0) exp_B.digitalWrite(row[i], HIGH);
  }
  Serial.print(F("AllColor: "));Serial.println(color);
}


void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  exp_init();
}

void loop() 
{
  int a;
  // put your main code here, to run repeatedly:
  OneByOne();
  delay(100);
  WriteDefaultStates();
  
  for( a=0; a<4; a++)
  {
    AllColor(a);
    delay(500);
    WriteDefaultStates();
  }
  
  

}
