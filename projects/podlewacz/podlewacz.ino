#define SECOND 1000UL
#define MINUTE (SECOND * 60UL)
#define HOUR (MINUTE * 60UL)

//variables
// constants won't change:
const int RainSensorPin = A0;  //rain sensor
int RainSensor = 0;
const int outPins[] = { 5, 6, 7, 8, 9, 10, 11, 12};       // an array of output Pins, D2,...
int outStates[] = {HIGH, LOW, LOW, LOW, LOW, LOW, LOW, LOW}; //array of output pins states
const int pinCount = 8;           // the number of pins (i.e. the length of the array)

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time outPin was updated

// interval at which to switch state (milliseconds)
const unsigned long interval = 1;   //1 [minute]
//const unsigned long interval = 30;   //30 [minute]

//--------------------------------
// set all declared outPins to OFF state
void SetAllOff()
{
  for (int thisPin = 0; thisPin < pinCount; thisPin++)
  {
    digitalWrite(outPins[thisPin], LOW);
  }
  delay(100);
  Serial.println(F("SetAllOff()"));
}
//--------------------------------
//void SetPinState(int Pin; int State)
//{
//  digitalWrite(Pin, state);
//  delay(50);
//  Serial.print(F("Pin: ")); Serial.print(Pin);Serial.print(F(" : "));  Serial.println(state);
//  
//}
//--------------------------------
void setup() {
  // put your setup code here, to run once:
  // initialize the serial communication:
  Serial.begin(9600);
  //input pin(s)
  Serial.println(F("setup()"));
  pinMode(RainSensor, INPUT_PULLUP);
  // use a for loop to initialize each pin as an output:
  for (int thisPin = 0; thisPin < pinCount; thisPin++) 
  {
    pinMode(outPins[thisPin], OUTPUT);
  }
  SetAllOff();
  //set 1st output ON
//  digitalWrite(outPins[0], outStates[0]);
//  delay(50);
//  Serial.print(F("outPin[0] : "));  Serial.print(outPins[0]); Serial.print(F(" : ")); Serial.println(outStates[0]);
}
//--------------------------------
void loop() 
{
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
  static int thisPin = 0; //current pin
  int UpdateStates = 0;

  //rain sensor DO: high - no rain, low - rain
  RainSensor = digitalRead(RainSensorPin);
  Serial.print(F("RainSensor: ")); Serial.println(RainSensor);
  //if(RainSensor == HIGH) //no rain, normal work - iterate
  if(RainSensor == LOW) //no rain, emulate work - iterate
  {
    //check elapsed time 
    Serial.print(F("Millis: ")); Serial.print(currentMillis - previousMillis); Serial.print(F("; interval: ")); Serial.println(interval*MINUTE);
    if (currentMillis - previousMillis >= interval*MINUTE) 
    {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      UpdateStates = 1;
      //Serial.print(F("UpdateStates: ")); Serial.println(UpdateStates);
    }
    if (thisPin == 0 || UpdateStates == 1)
    {
      //evaluate current pin state
      if(outStates[thisPin] == LOW) outStates[thisPin] = HIGH;
      else outStates[thisPin] = LOW;
      
      //set all outPins OFF
      SetAllOff();
      // turn thisPin ON:
      digitalWrite(outPins[thisPin], outStates[thisPin]);
      delay(50);
      Serial.print(F("outPin[")); Serial.print(thisPin);Serial.print(F("] : "));  Serial.print(outPins[thisPin]); Serial.print(F(" : ")); Serial.println(outStates[thisPin]);
     
      UpdateStates = 0;
      
      //evaluate thisPin value
      if(thisPin >= pinCount) thisPin = 0; //reset to zero
      else thisPin++;  //increment
      Serial.print(F("thisPin: ")); Serial.println(thisPin);
     
    }
  }
  else thisPin = 0; //rain, do nothing
  delay(1000);  //remove
}
