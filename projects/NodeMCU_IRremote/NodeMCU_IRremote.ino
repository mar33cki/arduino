/*
 * IRremoteESP8266: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * This is very simple teaching code to show you how to use the library.
 * If you are trying to decode your Infra-Red remote(s) for later replay,
 * use the IRrecvDumpV2.ino example code instead of this.
 * An IR detector/demodulator must be connected to the input kRecvPin.
 * Copyright 2009 Ken Shirriff, http://arcfn.com
 * Example circuit diagram:
 *  https://github.com/crankyoldgit/IRremoteESP8266/wiki#ir-receiving
 * Changes:
 *   Version 0.2 June, 2017
 *     Changed GPIO pin to the same as other examples.
 *     Used our own method for printing a uint64_t.
 *     Changed the baud rate to 115200.
 *   Version 0.1 Sept, 2015
 *     Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009
 */

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

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
#define IRval_err 0xFFFFFFFFFFFFFFFF
enum IR{IR_1, IR_2, IR_3, IR_4, IR_5, IR_6, IR_7, IR_8, IR_9, IR_0, IR_STAR, IR_HASH, IR_UP, IR_LEFT, IR_RIGHT, IR_DOWN, IR_OK, IR_ERR};
IR IRCode;
char str_code[10];

// An IR detector/demodulator is connected to GPIO pin 14(D5 on a NodeMCU
// board).
const uint16_t kRecvPin = 14;

IRrecv irrecv(kRecvPin);
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

void setup() {
  Serial.begin(115200);
  irrecv.enableIRIn();  // Start the receiver
  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);
  Serial.println();
  Serial.print("IRrecvDemo is now running and waiting for IR message on Pin ");
  Serial.println(kRecvPin);
}

void loop() {
  if (irrecv.decode(&results)) {
    // print() & println() can't handle printing long longs. (uint64_t)
    value2code(results.value);
    if(IRCode != IR_ERR)
    {
      serialPrintUint64(results.value, HEX);
      Serial.print(F("; code: "));
      Serial.print(IRCode);
      Serial.print(F(" -> "));
      Serial.println(str_code);  
    }    
    irrecv.resume();  // Receive the next value
  }
  delay(100);
}
