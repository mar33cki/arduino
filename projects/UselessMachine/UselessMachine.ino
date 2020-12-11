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
  int i=0;
  int randNum;
  counter++; //increment counter

  Serial.print(F("OpenSwitch: counter: "));
  Serial.println(counter);

  if(counter < 3)
  {
    // 1
    digitalWrite(LEDPin, LOW);
    myservo.write(180);
    delay(500);
    myservo.write(90);
    digitalWrite(LEDPin, HIGH);
  } 
  else
  { 
    randNum = random(1,7);
    
    switch(randNum)
    {
      case 1:
        digitalWrite(LED_BUILTIN, HIGH);
        myservo.write(180);
        delay(400);
        myservo.write(90);
        digitalWrite(LED_BUILTIN, LOW);
      break;
      
      case 2:  
        // 2
        digitalWrite(LEDPin, LOW);
        myservo.write(180);
        for(i=0; i<3; i++)
        {
          digitalWrite(LEDPin, LOW);
          delay(200);
          digitalWrite(LEDPin, HIGH);
          delay(200);
        }
        delay(1000);
        myservo.write(90);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(300);
        digitalWrite(LED_BUILTIN, LOW);
      break;
    
      case 3:
        myservo.write(0);
        delay(200);
        digitalWrite(LEDPin, LOW);
        myservo.write(180);
        delay(400);
        myservo.write(90);
        delay(200);
        digitalWrite(LEDPin, HIGH);
      break;
    
      case 4:
        myservo.write(0);
        digitalWrite(LED_BUILTIN, HIGH);
        myservo.write(50);
        delay(200);
        digitalWrite(LED_BUILTIN, LOW);
        myservo.write(180);
        delay(400);
        myservo.write(90);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(200);
        digitalWrite(LEDPin, HIGH);
        digitalWrite(LED_BUILTIN, LOW);
      break;
    
      case 5:
        digitalWrite(LED_BUILTIN, HIGH);
        for(i=0; i<3; i++)
        {
            myservo.write(180);
            delay(300);
            myservo.write(90);
            delay(250);
        }
        digitalWrite(LED_BUILTIN, LOW);
      break;
      
      case 6:
        myservo.write(0);
        delay(200);
        myservo.write(90);
        delay(200);
        myservo.write(0);
        delay(200);
        myservo.write(180);
        delay(300);
        myservo.write(90);
        delay(100);
      break;
      
      case 7:      
        myservo.write(0);
        delay(200);
        for(i=0; i<2; i++)
        {
          myservo.write(50);
          delay(200);
          myservo.write(0);
          delay(200);
        }
        myservo.write(180);
        delay(400);
        myservo.write(90);
        delay(100);
      break;
    }
  }
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
  pinMode(LED_BUILTIN, OUTPUT);
  for(int i = 0; i < 2; i++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(300);
    digitalWrite(LED_BUILTIN, LOW);
    delay(300);
  }
  pinMode(SwitchPin, INPUT_PULLUP);

  myservo.attach(ServoPin);  // attaches the servo on pin 9 to the servo object
  //myservo.write(-90);
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
