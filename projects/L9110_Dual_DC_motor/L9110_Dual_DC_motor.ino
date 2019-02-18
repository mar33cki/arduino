/*
 * Display number on seven segment display with Arduino
 * Written by Ahmad Shamshiri for Robojax.com
Date modified: Jun 11, 2018 at 17:20 at Ajax, Ontario, Canada
watch video instruction for this code:https://youtu.be/-g6Q9lSHDzg

Code is provided for free AS IS without warranty. 
You can share this acode as long as you keep the above note intact.
 */

const int A1A = 2;//define pin 2 for A1A
const int A1B = 3;//define pin 3 for A1B

const int B1A = 8;//define pin 8 for B1A
const int B1B = 9;//define pin 9 for B1B

const int SensLPin = A0; //define pin A0 for sensor left
const int SensRPin = A1; //define pin A1 for sensor right
int SensL = 0; //sensor left state
int SensR = 0; //sensor right state

void setup() {
  pinMode(B1A,OUTPUT);// define pin as output
  pinMode(B1B,OUTPUT);
  
  pinMode(A1A,OUTPUT);
  pinMode(A1B,OUTPUT);    

  pinMode(SensLPin,INPUT_PULLUP); //define as inputs
  pinMode(SensRPin,INPUT_PULLUP);
  delay(100);
}

void loop() {

//  motorA('R');// Turn motor A to RIGHT
//  delay(2000);
//  motorA('L');// Turn motor A to LEFT
//  delay(2000);   
//  motorA('O');// Turn motor A OFF
//  delay(2000); 
//    
//  motorB('R');// Turn motor B to RIGHT
//  delay(2000);
//   motorB('L');// Turn motor B to LEFT
//  delay(2000); 
//  motorB('O');// Turn motor B OFF
//  delay(2000); 
   

  SensL = digitalRead(SensLPin);
  SensR = digitalRead(SensRPin);
  
  if(SensL == 1 && SensR == 0)
  {  
    motorA('L');// Turn motor A to LEFT
    motorB('R'); // Turn motor B to RIGHT 
  }
  else if(SensR == 1 && SensL == 0)
  {  
    motorA('R');// Turn motor A to RIGHT
    motorB('L'); // Turn motor B to LEFT 
  }
  else if(SensL == 0 && SensR == 0) //do tylu i w bok
  {
     motorB('O');// Turn motor B OFF
     motorA('L');// Turn motor A to LEFT
     delay(500);
     motorB('L'); // Turn motor B to LEFT
     motorA('O');// Turn motor A OFF
     delay(500);  
     motorA('L');// Turn motor A to LEFT
     delay(400);
  }
  else if(SensL == 1 && SensR == 1)
  {
     motorA('R');// Turn motor A to RIGHT
     motorB('R'); // Turn motor B to RIGHT  
  }

  delay(100);
   
}

/*
 * @motorA
 * activation rotation of motor A
 * d is the direction
 * R = Right
 * L = Left
 * other = STOP
 */
void motorA(char d)
{
  if(d =='R')
  {
    digitalWrite(A1A,LOW);
    digitalWrite(A1B,HIGH); 
  }
  else if (d =='L')
  {
    digitalWrite(A1A,HIGH);
    digitalWrite(A1B,LOW);    
  }
  else
  {
    //Robojax.com L9110 Motor Tutorial
    // Turn motor OFF
    digitalWrite(A1A,LOW);
    digitalWrite(A1B,LOW);    
  }
}// motorA end

/*
 * @motorB
 * activation rotation of motor B
 * d is the direction
 * R = Right
 * L = Left
 * other = STOP
 */
void motorB(char d)
{

    if(d =='R')
    {
      digitalWrite(B1A,LOW);
      digitalWrite(B1B,HIGH); 
    }
    else if(d =='L')
    {
      digitalWrite(B1A,HIGH);
      digitalWrite(B1B,LOW);    
    }
    else
    {
    //Robojax.com L9110 Motor Tutorial
    // Turn motor OFF      
      digitalWrite(B1A,LOW);
      digitalWrite(B1B,LOW);     
    }

}// motorB end  
