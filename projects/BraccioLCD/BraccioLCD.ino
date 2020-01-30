 /*
  testBraccio90.ino

 testBraccio90 is a setup sketch to check the alignment of all the servo motors
 This is the first sketch you need to run on Braccio
 When you start this sketch Braccio will be positioned perpendicular to the base
 If you can't see the Braccio in this exact position you need to reallign the servo motors position

 Created on 18 Nov 2015
 by Andrea Martino

 This example is in the public domain.
 */

#include <Braccio.h>
#include <Servo.h>
#include <LiquidCrystal.h>

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_rot;
Servo wrist_ver;
Servo gripper;
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 16, 15, 4, 14, 2);
// Define constants and variables
const int inputPin = A0;  // buttons array analog input 
uint16_t inputValue = 0;   // value read from buttons array
int M1_val = 90;
int M[]={90,90,95,97,90,70,100};
int M_index=0;

void setup() {  
  lcd.begin(16, 2);              // set up the LCD's number of columns and rows
  //Initialization functions and set up the initial position for Braccio
  //All the servo motors will be positioned in the "safety" position:
  //Base (M1):90 degrees
  //Shoulder (M2): 45 degrees
  //Elbow (M3): 180 degrees
  //Wrist vertical (M4): 180 degrees
  //Wrist rotation (M5): 90 degrees
  //gripper (M6): 10 degrees
  lcd.print("COME ON!!!");
  Braccio.begin();
  lcd.setCursor(0, 1); 
  
}

void loop() {
  /*
   Step Delay: a milliseconds delay between the movement of each servo.  Allowed values from 10 to 30 msec.
   M1=base degrees. Allowed values from 0 to 180 degrees
   M2=shoulder degrees. Allowed values from 15 to 165 degrees
   M3=elbow degrees. Allowed values from 0 to 180 degrees
   M4=wrist vertical degrees. Allowed values from 0 to 180 degrees
   M5=wrist rotation degrees. Allowed values from 0 to 180 degrees
   M6=gripper degrees. Allowed values from 10 to 73 degrees. 10: the toungue is open, 73: the gripper is closed.
  */
  
  // the arm is aligned upwards  and the gripper is closed
                     //(step delay, M1, M2, M3, M4, M5, M6);
  Braccio.ServoMovement(1,180-M[0], M[1], M[2], M[3], M[4], M[5]);  
	CheckButtons();
  lcd.setCursor(0, 0);
  if (M_index==6)
    lcd.print("Speed:    ");
  else{
    lcd.print("Motor:");
    lcd.setCursor(6, 0);
    lcd.print(M_index+1);
    lcd.print("         ");
  }
  lcd.setCursor(0, 1);
  lcd.print(M[M_index]);
  lcd.print("         ");
}


void CheckButtons(){
// check buttons status
  inputValue = analogRead(inputPin);
  if(inputValue < 100 && inputValue >= 0) {inputValue = 1; Press1();}
  else if(inputValue < 250 && inputValue > 150) {inputValue = 2; Press2();}
  else if(inputValue < 470 && inputValue > 370) {inputValue = 3; Press3();}
  else if(inputValue < 670 && inputValue > 570) {inputValue = 4; Press4();}
  else if(inputValue < 870 && inputValue > 770) {inputValue = 5; Press5();}
  else if(inputValue <= 1023 && inputValue > 950) inputValue = 0;

}

void Press1(){
  M[M_index]-=1;
  if (M[M_index]<0)
    M[M_index]=0;
   delay(180-M[6]);
}

void Press2(){
  M[M_index]+=1;
  if (M[M_index]>180)
    M[M_index]=180;
  if (M[5]>70)
    M[5]=70;
  
  delay(180-M[6]);
}

void Press5(){
 M[0]=90;
 M[1]=90;
 M[2]=95;
 M[3]=97;
 M[4]=90;
 M[5]=70;
 
}

void Press4(){
 M_index++; 
 if (M_index>6)
  M_index=0;
 delay(500);
}

void Press3(){
 M_index--; 
 if (M_index<0)
  M_index=6;
 delay(500);
}
