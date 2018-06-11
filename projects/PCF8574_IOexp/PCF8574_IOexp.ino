#include <PCF8574.h>
#include <Wire.h>
#include <Timers.h> 

//no wire needed, pin on pull-up, to disable jump wire pin to GND
#define DEBUG_ON_PIN 3
#define DEBUG_ON 1 

#define THREADS 3
Timers <THREADS> thread; //definicja watkow

#define DINo 8
PCF8574 IOexp0, IOexp1, IOexp2;
byte IOexp0States[DINo], IOexp1States[DINo], IOexp2States[DINo];
int DebugOn = 0;
String MessageString;

// constants won't change. Used here to set a pin number :
const int ledPin =  LED_BUILTIN;// the number of the LED pin

void IOexpInit()
{
  int i;
  if(DebugOn == DEBUG_ON) Serial.println("IOexpInit start.");
  
  //expander 0
  IOexp0.begin(0x27);       //address
  IOexp0.pinMode(3, OUTPUT); //open collector output
  //pull-up inputs
  for ( i = 0; i < DINo; i++) { if(i!=3) { IOexp0.pinMode(i, INPUT); IOexp0.pullUp(i); } }
  IOexp0.digitalWrite(3, LOW);  //default state
  if(DebugOn == DEBUG_ON) Serial.println("IOexpInit: expander 0 done.");

  //expander 1
  IOexp1.begin(0x26);       //address
  IOexp1.pinMode(3, OUTPUT); //open collector output
  //pull-up inputs
  for ( i = 0; i < DINo; i++) { if(i!=3) { IOexp1.pinMode(i, INPUT); IOexp1.pullUp(i); } }
  IOexp1.digitalWrite(3, LOW);  //default state
  if(DebugOn == DEBUG_ON) Serial.println("IOexpInit: expander 1 done.");
  
  //expander 2
  IOexp2.begin(0x3D);       //address
  IOexp2.pinMode(3, OUTPUT); //open collector output
  //pull-up inputs
  for ( i = 0; i < DINo; i++) { if(i!=3) { IOexp2.pinMode(i, INPUT); IOexp2.pullUp(i); } }
  IOexp2.digitalWrite(3, LOW);  //default state
  if(DebugOn == DEBUG_ON) Serial.println("IOexpInit: expander 2 done.");

  if(DebugOn == DEBUG_ON) Serial.println("IOexpInit done.");
}
//-------------------------------------
void GetIOexpStates()
{
  int index=0;  

  //IO expander 0
  for (index = 0; index <DINo; index++) if(index!=3) IOexp0States[index] = IOexp0.digitalRead(index);
  
  //IO expander 1
  for (index = 0; index <DINo; index++) if(index!=3) IOexp1States[index] = IOexp1.digitalRead(index);
  
  //IO expander 2
  for (index = 0; index <DINo; index++) if(index!=3) IOexp2States[index] = IOexp2.digitalRead(index);
}
//-------------------------------------
void EvaluateIOStates()
{
  int i;
//  int start, finish;
//  start = micros();
  
  static byte IOexp0States_old[DINo],IOexp1States_old[DINo], IOexp2States_old[DINo];
  byte AlarmOccured = 0;
  GetIOexpStates();

//  if(DebugOn == DEBUG_ON) {
//    Serial.print("IOexp0States:"); for(i=0; i <DINo; i++) Serial.print(IOexp0States[i]); 
//    Serial.print(" IOexp0States_old:"); for(i=0; i <DINo; i++) Serial.print(IOexp0States_old[i]); 
//    Serial.print(" IOexp1States:"); for(i=0; i <DINo; i++) Serial.print(IOexp1States[i]); 
//    Serial.print(" IOexp1States_old:"); for(i=0; i <DINo; i++) Serial.print(IOexp1States_old[i]); 
//    Serial.print(" IOexp2States:"); for(i=0; i <DINo; i++) Serial.print(IOexp2States[i]); 
//    Serial.print(" IOexp2States_old:"); for(i=0; i <DINo; i++) Serial.print(IOexp2States_old[i]); 
//  Serial.println(); }

  digitalWrite(ledPin, !digitalRead(ledPin)); //toggle LED
  
  //evaluate state change(s) and their values
  for (i = 0; i < DINo; i++) if(IOexp0States[i] != IOexp0States_old[i]) if(IOexp0States[i] == 0) 
  { MessageString = String("Alarm: 0.") + String(i);
    AlarmOccured = 1;
    Serial.println(MessageString);
  }
  for (i = 0; i < DINo; i++) if(IOexp1States[i] != IOexp1States_old[i]) if(IOexp1States[i] == 0) 
  { MessageString = String("Alarm: 1.") + String(i);
    AlarmOccured = 1;
    Serial.println(MessageString);
  }
  for (i = 0; i < DINo; i++) if(IOexp2States[i] != IOexp2States_old[i]) if(IOexp2States[i] == 0) 
  { MessageString = String("Alarm: 2.") + String(i);
    AlarmOccured = 1;
    Serial.println(MessageString);
  }

  if(AlarmOccured = 1)
  {
    //send data (MessageString) to mqtt
  }
 
  //copy current states to static variable
  copyArrays(IOexp0States, IOexp0States_old, DINo);  
  copyArrays(IOexp1States, IOexp1States_old, DINo);  
  copyArrays(IOexp2States, IOexp2States_old, DINo);  

//  finish = micros() - start;
//  Serial.print("EvaluateIOStates time in us: "); Serial.println(finish);
}
//-------------------------------------
//function to copy arrays
void copyArrays(byte* src, byte* dst, int len) {
    memcpy(dst, src, sizeof(src[0])*len);
}
//-------------------------------------
void setup() 
{
  pinMode(ledPin, OUTPUT);
  
  pinMode(DEBUG_ON_PIN, INPUT_PULLUP);
  DebugOn = digitalRead(DEBUG_ON_PIN);

  Serial.begin(9600);
  Serial.println("\n\nI2C IO expander");
  Serial.print("DebugOn: "); Serial.println(DebugOn);
  Serial.print("DEBUG_ON: "); Serial.println(DEBUG_ON);
  if(DebugOn == DEBUG_ON) Serial.println("DebugOn == DEBUG_ON");
  else  Serial.println("DebugOn != DEBUG_ON");
  
  //init I2C IO expander(s)
  IOexpInit();

  if(DebugOn == DEBUG_ON) Serial.println("Setup: thread.attach.");
  thread.attach(0, 100, EvaluateIOStates); //thread 0: get IO expander states
}

//-------------------------------------

void loop() 
{
  // put your main code here, to run repeatedly:

  thread.process();
  delay (100);
}
