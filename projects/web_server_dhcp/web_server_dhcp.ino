// Arduino demo sketch for testing the DHCP client code
//
// Original author: Andrew Lindsay
// Major rewrite and API overhaul by jcw, 2011-06-07
//
// Copyright: GPL V2
// See http://www.gnu.org/licenses/gpl.html

#include <EtherCard.h>

static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[700];

const char page1[]  =
"HTTP/1.0 503 Service Unavailable\r\n"
"Content-Type: text/html\r\n"
"Retry-After: 600\r\n"
"\r\n"
"<html>"
  "<head><title>"
    "Temperature web service"
  "</title></head>"
  "<body>"
    "<h3>The room temperature is:</h3>"
    "<p><em>"
      ;
      
const char page2[]  =
      "Celcius<br />"
      "Done by mar33cki"
    "</em></p>"
  "</body>"
"</html>"
;

const char page3[] =
"HTTP/1.0 503 Service Unavailable\r\n"
"Content-Type: text/html\r\n"
"Retry-After: 600\r\n"
"\r\n"
"<html>"
  "<head><title>"
    "Service Temporarily Unavailable"
  "</title></head>"
  "<body>"
    "<h3>This service is currently unavailable</h3>"
    "<p><em>"
      "The main server is currently off-line.<br />"
      "Please try again later."
    "</em></p>"
  "</body>"
"</html>"
;

#define AnalogReferenceLevel 1100 // analog reference level in mV, INTERNAL = 1100
//#define AnalogReferenceLevel 5000 // analog reference level in mV, DEFAULT = 5000
#define AnalogResolution 1024

//analog input
int sensorT1 = A0;    // select the input pin for the temp sensor

//dynamic values
volatile unsigned long sensor1Value = 0;  // variable to store the value coming from the sensor
volatile unsigned long sensor1ValueRaw = 0;  // variable to store the value coming from the sensor


void setup () {
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

void loop () 
{
  char page[300] = {0};
  
  // wait for an incoming TCP packet, but ignore its contents
  if (ether.packetLoop(ether.packetReceive())) 
  {
    ReadADC();
    unsigned long temp = sensor1Value;
    Serial.print("Temperature: ");
    Serial.println(temp);

    
   //sprintf(page,"%s",page3);   
   //memcpy((char *)page, page3, sizeof(page3));
    /*
    memcpy((char *)page, page1, sizeof(page1));
    temp = sensor1Value;
    memcpy((char *)(page+sizeof(page1)), &temp, sizeof(sensor1Value));
    memcpy((char *)(page+sizeof(page1)+sizeof(sensor1Value)), page2, sizeof(page2));
   */
   /*
    Serial.println("page:");
    Serial.println(page);             //Serial.println("");
    */
   
    
    //Serial.print("sizeof(page)=");  Serial.println(sizeof(page));
//    Serial.print("sizeof(page1)="); Serial.println(sizeof(page1));
//    Serial.print("sizeof(page2)="); Serial.println(sizeof(page2));
//    Serial.print(page1);
//    Serial.println(page2);

    //page3[sizeof(page3) +1] = '\0';
    
    memcpy_P(ether.tcpOffset(), page3, sizeof page3);
    ether.httpServerReply(sizeof page3 - 1);
    
  }
}

//--------------------------------------
void ReadADC()
{
  // read the value from the sensor LM35, and scale to Celcius Degree
  sensor1ValueRaw = analogRead(sensorT1); 
  //sensor1Value = sensor1ValueRaw/4; //scale to Celcius Degree
  sensor1Value = CaculateCelcius(sensor1ValueRaw);
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
