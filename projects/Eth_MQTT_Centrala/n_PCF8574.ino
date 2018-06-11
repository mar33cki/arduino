/*
 * PCF8574 IO expander used as Digital Inputs expander
 */

PCF8574 IOexp0, IOexp1, IOexp2;
byte IOexp0States[DINo], IOexp1States[DINo], IOexp2States[DINo];

void IOexpInit()
{
  int i;
  #ifdef DEBUG 
    Serial.println(F("IOexpInit:start."));
  #endif
  
  //expander 0
  IOexp0.begin(0x27);       //address
  IOexp0.pinMode(3, OUTPUT); //open collector output
  //pull-up inputs
  for ( i = 0; i < DINo; i++) { if(i!=3) { IOexp0.pinMode(i, INPUT); IOexp0.pullUp(i); } }
  IOexp0.digitalWrite(3, LOW);  //default state
  #ifdef DEBUG 
    Serial.println(F("IOexpInit: expander 0 done."));
  #endif

  //expander 1
  IOexp1.begin(0x26);       //address
  IOexp1.pinMode(3, OUTPUT); //open collector output
  //pull-up inputs
  for ( i = 0; i < DINo; i++) { if(i!=3) { IOexp1.pinMode(i, INPUT); IOexp1.pullUp(i); } }
  IOexp1.digitalWrite(3, LOW);  //default state
  #ifdef DEBUG 
    Serial.println(F("IOexpInit: expander 1 done."));
  #endif
  
  //expander 2
  IOexp2.begin(0x3D);       //address
  IOexp2.pinMode(3, OUTPUT); //open collector output
  //pull-up inputs
  for ( i = 0; i < DINo; i++) { if(i!=3) { IOexp2.pinMode(i, INPUT); IOexp2.pullUp(i); } }
  IOexp2.digitalWrite(3, LOW);  //default state
  #ifdef DEBUG 
    Serial.println(F("IOexpInit: expander 2 done."));
  #endif

  #ifdef DEBUG
    Serial.println(F("IOexpInit:done."));
  #endif
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
  int i, channel;
//  int start, finish;
//  start = micros();
  
  static byte IOexp0States_old[DINo],IOexp1States_old[DINo], IOexp2States_old[DINo];
  byte AlarmOccured = 0;
  GetIOexpStates();

//  #ifdef DEBUG {
//    Serial.print(F("IOexp0States:")); for(i=0; i <DINo; i++) Serial.print(IOexp0States[i]); 
//    Serial.print(F(" IOexp0States_old:")); for(i=0; i <DINo; i++) Serial.print(IOexp0States_old[i]); 
//    Serial.print(F(" IOexp1States:")); for(i=0; i <DINo; i++) Serial.print(IOexp1States[i]); 
//    Serial.print(F(" IOexp1States_old:")); for(i=0; i <DINo; i++) Serial.print(IOexp1States_old[i]); 
//    Serial.print(F(" IOexp2States:")); for(i=0; i <DINo; i++) Serial.print(IOexp2States[i]); 
//    Serial.print(F(" IOexp2States_old:")); for(i=0; i <DINo; i++) Serial.print(IOexp2States_old[i]); 
//  Serial.println(); }

  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); //toggle LED
  
  //evaluate state change(s) and their values
  for (i = 0; i < DINo; i++) if(IOexp0States[i] != IOexp0States_old[i]) if(IOexp0States[i] == 0) 
  { 
    #ifdef DEBUG
       Serial.println();
       Serial.print(F("EvaluateIOStates:StateChange:0.")); Serial.print(i); Serial.println();
    #endif
//    AlarmOccured = 1;
    channel = i;
    sendMqtt(MQTT_MOTION_CHANNEL, (int)channel);
    sendMqtt(MQTT_MOTION_DETECTION, 1);
  }
  for (i = 0; i < DINo; i++) if(IOexp1States[i] != IOexp1States_old[i]) if(IOexp1States[i] == 0) 
  { 
    #ifdef DEBUG
       Serial.println();
       Serial.print(F("EvaluateIOStates:StateChange:1.")); Serial.print(i); Serial.println();
    #endif
//    AlarmOccured = 1;
    channel = 10 + i;
    sendMqtt(MQTT_MOTION_CHANNEL, (int)channel);
    sendMqtt(MQTT_MOTION_DETECTION, 1);
  }
  for (i = 0; i < DINo; i++) if(IOexp2States[i] != IOexp2States_old[i]) if(IOexp2States[i] == 0) 
  { 
    #ifdef DEBUG
       Serial.println();
       Serial.print(F("EvaluateIOStates:StateChange:2.")); Serial.print(i); Serial.println();
    #endif
//    AlarmOccured = 1;
    channel = 20 + i;
    sendMqtt(MQTT_MOTION_CHANNEL, (int)channel);
    sendMqtt(MQTT_MOTION_DETECTION, 1);
  }

//  if(AlarmOccured = 1)
//  {
//    //send data (MessageString) to mqtt
//    sendMqtt(MQTT_MOTION_DETECTION, 1);
//    sendMqtt(MQTT_MOTION_CHANNEL, (int)channel);
//    AlarmOccured = 0;
//  }
 
  //copy current states to static variable
  copyArrays(IOexp0States, IOexp0States_old, DINo);  
  copyArrays(IOexp1States, IOexp1States_old, DINo);  
  copyArrays(IOexp2States, IOexp2States_old, DINo);  
}
//-------------------------------------
//function to copy arrays
void copyArrays(byte* src, byte* dst, int len) {
    memcpy(dst, src, sizeof(src[0])*len);
}
//-------------------------------------
