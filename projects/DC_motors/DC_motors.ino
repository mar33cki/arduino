//motors control pins
int leftForward = 3, leftBackward = 5;
int rightForward = 6, rightBackward = 9;
//IR sensors inputs
int inIRLeft = 4, valIRLeft = 0; //pin, value
int inIRRight = 2, valIRRight = 0; //pin, value

//#define LeftStop      0
//#define LeftForward   1
//#define LeftBackward  2
//#define RightStop     3
//#define RightForward  4
//#define RightBackward 5


void ControlWheel(int wheel, int direction, int speed)
{
  //wheel = 0 - right
  if(!wheel && direction)
  {
    digitalWrite(rightForward, LOW);
    analogWrite(rightBackward, speed);
  }
  if(!wheel && !direction)
  {
    digitalWrite(rightForward, speed);
    analogWrite(rightBackward, LOW);
  }

  //wheel = 1 - left
  if(wheel && direction)
  {
    digitalWrite(leftForward, LOW);
    analogWrite(leftBackward, speed);
  }
  if(wheel && !direction)
  {
    digitalWrite(leftForward, speed);
    analogWrite(leftBackward, LOW);
  }

}

void setup() {
  // put your setup code here, to run once:
  //outputs
  pinMode(leftForward, OUTPUT); //sets the pin as output
  pinMode(leftBackward, OUTPUT); //sets the pin as output
  pinMode(rightForward, OUTPUT); //sets the pin as output
  pinMode(rightBackward, OUTPUT); //sets the pin as output

  //inputs
  pinMode(inIRLeft, INPUT);
  pinMode(inIRRight, INPUT);

  //serial
  Serial.begin(9600);      // open the serial port at 9600 bps:
}

void loop() {
  // put your main code here, to run repeatedly:

  valIRLeft = digitalRead(inIRLeft);
  Serial.print("valIRLeft: "); Serial.print(valIRLeft); Serial.print("  ");

  valIRRight = digitalRead(inIRRight);
  Serial.print("valIRRight: "); Serial.println(valIRRight);

  if(valIRRight) 
  {
    ControlWheel(1, 1, 127);  //left wheel go forward
    Serial.println("left wheel go forward");   
    delay(100);
  }
  else
  {
    ControlWheel(1, 0, 127);  //left wheel go backward
    Serial.println("left wheel go backward");
    delay(100);
  }

  if(valIRLeft) 
  {    
    ControlWheel(0, 1, 127);  //left wheel go forward
    Serial.println("left wheel go forward");   
    delay(100);
  }
  else
  {
    ControlWheel(0, 0, 127);  //left wheel go backward
    Serial.println("left wheel go backward");
    delay(100);
  }

  //delay(3000);

//  ControlWheel(1, 1, 127);  //left wheel go forward
//  Serial.println("ControlWheel(1, 1, 127)");
//  ControlWheel(0, 1, 127);    //right wheel go forward
//  Serial.println("ControlWheel(0, 0, 0)");
//  
//  delay(3000);
//  
//  ControlWheel(1, 1, 0);  //left wheel stop
//  Serial.println("ControlWheel(1, 1, 0)");
//  ControlWheel(0, 1, 0);    //right wheel stop
//  Serial.println("ControlWheel(0, 0, 0)");
//  


}
