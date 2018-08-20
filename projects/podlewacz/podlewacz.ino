
//variables
// constants won't change:
const int RainSensor = A0;  //rain sensor
const int outPins[] = { 5, 6, 7, 8, 9, 10, 11, 12};       // an array of output Pins, D2,...
int outStates[] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW}; //array of output pins states
const int pinCount = 8;           // the number of pins (i.e. the length of the array)

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time outPin was updated

// interval at which to switch state (milliseconds)
const unsigned long interval = 6000;   //1min: 1min * 60s * 1000ms = 6 000 ms
//const unsigned long interval = 1800000;   //30min: 30min * 60s * 1000ms = 1 800 000 ms

//--------------------------------
// set all declared outPins to OFF state
void SetAllOff()
{
  for (int thisPin = 0; thisPin < pinCount; thisPin++)
  {
    digitalWrite(outPins[thisPin], LOW);
  }
  delay(100);
}
//--------------------------------

void setup() {
  // put your setup code here, to run once:
  //input pin(s)
  pinMode(RainSensor, INPUT);
  // use a for loop to initialize each pin as an output:
  for (int thisPin = 0; thisPin < pinCount; thisPin++) 
  {
    pinMode(outPins[thisPin], OUTPUT);
  }
  SetAllOff();
}
//--------------------------------
void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
  static int thisPin = 0; //current pin

  //rain sensor DO: high - no rain, low - rain
  if(RainSensor == HIGH) //no rain, normal work - iterate
  {
    //check elapsed time 
    if (currentMillis - previousMillis >= interval) 
    {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
  
      //evaluate current pin state
      if(outStates[thisPin] == LOW) outStates[thisPin] = HIGH;
      else outStates[thisPin] = LOW;
      
      //set all outPins OFF
      SetAllOff();
      // turn thisPin ON:
      digitalWrite(outPins[thisPin], outStates[thisPin]);
      delay(50);
  
      //evaluate thisPin value
      if(thisPin >= pinCount) thisPin = 0; //reset to zero
      else thisPin++;  //increment
    }
    else thisPin = 0; //rain, do nothing
  }
}
