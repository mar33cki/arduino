/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 * 
 * mar33cki updates: define codes, enum, value2code
 */

#include <IRremote.h>

//IR remote hex codes
#define IRval_1 0xFFA25D
#define IRval_2 0xFF629D
#define IRval_3 0xFFE21D
#define IRval_4 0xFF22DD
#define IRval_5 0xFF02FD
#define IRval_6 0xFFC23D
#define IRval_7 0xFFE01F
#define IRval_8 0xFFA857
#define IRval_9 0xFF906F
#define IRval_STAR 0xFF6897
#define IRval_0 0xFF9867  
#define IRval_HASH 0xFFB04F
#define IRval_UP  0xFF18E7
#define IRval_LEFT  0xFF10EF
#define IRval_RIGHT 0xFF5AA5
#define IRval_DOWN  0xFF4AB5
#define IRval_OK  0xFF38C7
#define IRval_err 0xFFFFFFFF
enum IR{IR_1, IR_2, IR_3, IR_4, IR_5, IR_6, IR_7, IR_8, IR_9, IR_0, IR_STAR, IR_HASH, IR_UP, IR_LEFT, IR_RIGHT, IR_DOWN, IR_OK, IR_ERR};
IR IRCode;
char str_code[10];

int RECV_PIN = 2;

IRrecv irrecv(RECV_PIN);

decode_results results;

char value2code(uint64_t value)
{
  switch (value)
  {
    case IRval_1: { IRCode = IR_1; strcpy(str_code, "1"); } break;
    case IRval_2: { IRCode = IR_2; strcpy(str_code, "2"); } break;
    case IRval_3: { IRCode = IR_3; strcpy(str_code, "3"); } break;
    case IRval_4: { IRCode = IR_4; strcpy(str_code, "4"); } break;
    case IRval_5: { IRCode = IR_5; strcpy(str_code, "5"); } break;
    case IRval_6: { IRCode = IR_6; strcpy(str_code, "6"); } break;
    case IRval_7: { IRCode = IR_7; strcpy(str_code, "7"); } break;
    case IRval_8: { IRCode = IR_8; strcpy(str_code, "8"); } break;
    case IRval_9: { IRCode = IR_9; strcpy(str_code, "9"); } break;
    case IRval_0: { IRCode = IR_0; strcpy(str_code, "0"); } break;
    case IRval_STAR: { IRCode = IR_STAR; strcpy(str_code, "STAR"); } break;
    case IRval_HASH: { IRCode = IR_HASH; strcpy(str_code, "HASH"); } break;
    case IRval_UP: { IRCode = IR_UP; strcpy(str_code, "UP"); } break;
    case IRval_LEFT: { IRCode = IR_LEFT; strcpy(str_code, "LEFT"); } break;
    case IRval_RIGHT: { IRCode = IR_RIGHT; strcpy(str_code, "RIGHT"); } break;
    case IRval_DOWN: { IRCode = IR_DOWN; strcpy(str_code, "DOWN"); } break;
    case IRval_OK: { IRCode = IR_OK; strcpy(str_code, "OK"); } break;
    case IRval_err: { IRCode = IR_ERR; strcpy(str_code, "ERR"); } break;
  }

}

void setup()
{
  Serial.begin(115200);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.print(F("Enabled IRin, IR pin:"));
  Serial.println(RECV_PIN);
}

void loop() {
  if (irrecv.decode(&results)) 
  {
//    Serial.println(results.value, HEX);
    value2code(results.value);
    if(IRCode != IR_ERR)
    {
      Serial.print(results.value, HEX);
      Serial.print(F("; code: "));
      Serial.print(IRCode);
      Serial.print(F(" -> "));
      Serial.println(str_code);  
    }
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}
