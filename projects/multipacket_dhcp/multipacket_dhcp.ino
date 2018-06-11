#include <EtherCard.h>
#define TCP_FLAGS_FIN_V 1 //as declared in net.h
#define TCP_FLAGS_ACK_V 0x10 //as declared in net.h
//static byte myip[] = { 192,168,0,66 };
//static byte gwip[] = { 192,168,0,250 };
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[900]; // tcp ip send and receive buffer

const char pageA[] PROGMEM =
"HTTP/1.0 200 OK\r\n"
"Content-Type: text/html\r\n"
"\r\n"
"<html>"
  "<head><title>"
    "Arduino Nano WebServer"
  "</title></head>"
  "<body>"
    "<a href='/'>Start here</a><br>"
;
const char pageD[] PROGMEM =
      "<h3>AI0 measurement:</h3>"
    "<p><em>"
      "Room temperature: "
    ;
const char pageE[] PROGMEM =
    "'C</em></p>"
;
const char pageF[] PROGMEM ="."
;

//analog input
int sensorT1 = A0;    // select the input pin for the temp sensor
#define AnalogReferenceLevel 1100 // analog reference level in mV, INTERNAL = 1100
//#define AnalogReferenceLevel 5000 // analog reference level in mV, DEFAULT = 5000
#define AnalogResolution 1024


void setup () 
{
  //analog reference  built-in reference, equal to 1.1 volts on the ATmega168 or ATmega328 
  analogReference(INTERNAL);
  
  Serial.begin(57600);
  Serial.println(F("\n[testDHCP]"));

  Serial.print("MAC: ");
  for (byte i = 0; i < 6; ++i) {
    Serial.print(mymac[i], HEX);
    if (i < 5)
      Serial.print(':');
  }
  Serial.println();
  
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println(F("Failed to access Ethernet controller"));

  Serial.println(F("Setting up DHCP"));
  if (!ether.dhcpSetup())
    Serial.println(F("DHCP failed"));
  
  ether.printIp("My IP: ", ether.myip);
  ether.printIp("Netmask: ", ether.netmask);
  ether.printIp("GW IP: ", ether.gwip);
  ether.printIp("DNS IP: ", ether.dnsip);
}

void loop(){
    word pos = ether.packetLoop(ether.packetReceive());
    volatile int sensor1Value = 0, origValue = 0;  // variable to store the value coming from the sensor
    char buf[3];

    
    // check if valid tcp data is received
    if (pos) {
        char* data = (char *) Ethernet::buffer + pos;
        if (strncmp("GET / ", data, 6) == 0) {
                                   
            ether.httpServerReplyAck(); // send ack to the request
            
            memcpy_P(ether.tcpOffset(), pageA, sizeof pageA); // send first packet and not send the terminate flag
            ether.httpServerReply_with_flags(sizeof pageA - 1,TCP_FLAGS_ACK_V);

            memcpy_P(ether.tcpOffset(), pageD, sizeof pageD); // send fourth packet and not send the terminate flag
            ether.httpServerReply_with_flags(sizeof pageD - 1,TCP_FLAGS_ACK_V);

            origValue = ReadADC(sensorT1);  //measure
            sensor1Value = origValue;       //copy value
            sensor1Value = sensor1Value/10; //take intiger value
            i2str(sensor1Value, buf);       //convert to str
            Serial.print(F("buf: ")); Serial.print(buf);
                        
            memcpy(ether.tcpOffset(), buf, sizeof buf); // send fourth packet and not send the terminate flag
            ether.httpServerReply_with_flags(sizeof buf - 1,TCP_FLAGS_ACK_V);

            //display '.' separator
            memcpy_P(ether.tcpOffset(), pageF, sizeof pageF); // send fourth packet and not send the terminate flag
            ether.httpServerReply_with_flags(sizeof pageF - 1,TCP_FLAGS_ACK_V);

            //calculate float value
            sensor1Value = origValue%10; 
            if(sensor1Value == 0) { buf[0] = '0'; buf[1] = NULL;}
            else i2str(sensor1Value, buf);
            Serial.print(F(".")); Serial.println(buf); 
                        
            memcpy(ether.tcpOffset(), buf, sizeof buf); // send fourth packet and not send the terminate flag
            ether.httpServerReply_with_flags(sizeof buf - 1,TCP_FLAGS_ACK_V);
            
            memcpy_P(ether.tcpOffset(), pageE, sizeof pageE); // send fiveth packet and send the terminate flag
            ether.httpServerReply_with_flags(sizeof pageE - 1,TCP_FLAGS_ACK_V|TCP_FLAGS_FIN_V); }
        else
        {
            ether.httpServerReplyAck(); // send ack to the request
            memcpy_P(ether.tcpOffset(), pageA, sizeof pageA);//only the first part will sended 
            ether.httpServerReply_with_flags(sizeof pageA - 1,TCP_FLAGS_ACK_V|TCP_FLAGS_FIN_V);
        }
  }
}

//--------------------------------------
int ReadADC(int AI)
{
  unsigned long ValueRaw = 0, Value = 0;
  // read the value from the sensor LM35, and scale to Celcius Degree
  ValueRaw = analogRead(AI); 
  //sensor1Value = sensor1ValueRaw/4; //scale to Celcius Degree
  Value = CaculateCelcius(ValueRaw);
  return (int) Value;
}
//--------------------------------------
unsigned long CaculateCelcius (unsigned long RawValue)
{
 unsigned long TemperatureCelcius = 0;
 
 //calulate temperature in Celcius
 TemperatureCelcius = (RawValue *  AnalogReferenceLevel);
 TemperatureCelcius = TemperatureCelcius / AnalogResolution;
 //TemperatureCelcius = TemperatureCelcius / 10;
  
 return TemperatureCelcius;
}
//--------------------------------------
char *i2str(int i, char *buf)
{
  byte l=0;
  if(i<0) buf[l++]='-';
  boolean leadingZ=true;
  for(int div=10000, mod=0; div>0; div/=10)
  {
    mod=i%div;
    i/=div;
    if(!leadingZ || i!=0){
       leadingZ=false;
       buf[l++]=i+'0';
    }
    i=mod;
  }
  buf[l]=0;
  return buf;
}
