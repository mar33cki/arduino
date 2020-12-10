/*
 * this is to control useless box 
 * on Arduino nano, where:
 * switch: Pin 2
 * LED: Pin 3
 * servo: pin 9
 */
#include <Servo.h>

#define LEDPin 3
#define ServoPin 9
#define SwitchPin 2

bool switchstate, switchchange;
int counter = 0;
int servoSteps = 90;

Servo myservo;  // create servo object to control a servo

//-------------------------------------
// this function check the Switch state
void CheckSwitch()
{
  static bool previousstate;
  
  switchstate = digitalRead(SwitchPin); //check actual state of switch

  if(!switchstate)
  {
    switchchange = true;

    Serial.print(F("CheckSwitch: switchstate: "));
    Serial.print(switchstate);
    Serial.print(F(" switchchange: "));
    Serial.println(switchchange);
  }
//  if(switchstate != previousstate) switchchange = true; //check if switch changed
  else switchchange = false; //switch did not changed

  
  
  previousstate = switchstate; //save actual state for next occurance
}
//-------------------------------------
void OpenSwitch()
{
  counter++; //increment counter

  Serial.print(F("OpenSwitch: counter: "));
  Serial.println(counter);


 // myservo.write(val);                  // sets the servo position according to the scaled value
  digitalWrite(LEDPin, LOW);
  myservo.write(servoSteps);
  delay(500);
  myservo.write(-servoSteps);
  digitalWrite(LEDPin, HIGH);
}
//-------------------------------------
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("=========================="));
  Serial.println(F("Useless machine"));

  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, HIGH);
  pinMode(SwitchPin, INPUT_PULLUP);

  myservo.attach(ServoPin);  // attaches the servo on pin 9 to the servo object
  myservo.write(-90);
}

void loop() {
  // put your main code here, to run repeatedly:
  CheckSwitch();
  if(switchchange)
  {
    OpenSwitch();
  }

  delay(100);
}
