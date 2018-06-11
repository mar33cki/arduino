#include "DHT_Eth_config.h"
/*-------------------------------------------------------*/
Timers <THREADS> thread; //definicja watkow
/*-------------------------------------------------------*/
//DHT sensor
DHT dht(DHTPIN, DHTTYPE); // definicja czujnika
/*-------------------------------------------------------*/
void ftoa(float Value, char* Buffer);
/*-------------------------------------------------------*/
void callback(char* topic, byte* payload, unsigned int length) 
{
  int data = 0;
  int state = LOW;
  // handle message arrived
  #ifdef DEBUG
    Serial.print(F("callback:Message arrived: ")); Serial.print(topic); Serial.print(F(" => "));
    for (int i=0;i<length;i++) Serial.print((char)payload[i]); 
    Serial.println();
  #endif

  // Allocate the correct amount of memory for the payload copy
  byte* p = (byte*)malloc(length+1);
  // Copy the payload to the new buffer
  memcpy(p,payload,length);
  p[length] = 0;
  data = atoi((char *) p);

  //determine the data
  if(data == 0) state = HIGH;
  else if (data == 1) state = LOW;

  switch(atoi(topic))
  {
    case MQTT_RELAY1:
    {
      digitalWrite(RELAY1PIN, state);
      #ifdef DEBUG
        Serial.print(F("callback:[MQTT_RELAY1]:"));
        Serial.print(MQTT_RELAY1); Serial.print(F(" : ")); Serial.println(state);
      #endif
      break;
    }
    case MQTT_RELAY2:
    {
      digitalWrite(RELAY2PIN, state);
      #ifdef DEBUG
        Serial.print(F("callback:[MQTT_RELAY2]:"));
        Serial.print(MQTT_RELAY2); Serial.print(F(" : ")); Serial.println(state);
      #endif
      break;
    }
 /*   case MQTT_DIMMER:
    {  
  //    mqttBuffer[MQTT_DIMMER_NO].Data = data;
      break;
    }*/
    default:
    break;
  }
  // Free the memory
  free(p);
}
/*-------------------------------------------------------*/
// Initialize the Ethernet client library
EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);
/*-------------------------------------------------------*/
void EthInit()
{  
  #ifdef DEBUG
    Serial.println(F("EthInit:[EthInit]"));
  #endif

  client.setServer(server, 1883);
  client.setCallback(callback);
  
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0)
  { 
    #ifdef DEBUG
      Serial.println(F("EthInit:ERR: Failed to configure Ethernet using DHCP"));
    #endif
  }

  #ifdef DEBUG
    Serial.print(F("EthInit:IP address: "));
    for (byte thisByte = 0; thisByte < 4; thisByte++) 
    {
      // print the value of each byte of the IP address:
      Serial.print(Ethernet.localIP()[thisByte], DEC);
      Serial.print(F("."));   
    }
    Serial.println();
  #endif
  lastReconnectAttempt = 0;
}
/*-------------------------------------------------------*/
void DHT_Read()
{    
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  static float temp, hum;
  static unsigned int count;


  // Sprawdzamy czy są odczytane wartości
  if (isnan(temperature) || isnan(humidity))
  {
    // Jeśli nie, wyświetlamy informację o błędzie
    #ifdef DEBUG
      Serial.println(F("DHT_Read:ERR: DHT fail!"));
    #endif
  } 
  else
  {
    if (humidity != hum || temperature != temp)
    {
      // Jeśli tak, wyświetlamy wyniki pomiaru
      #ifdef DEBUG
        Serial.println();
        Serial.print(F("DHT_Read:"));
        Serial.print(count); Serial.print(F(". "));
        Serial.print(F("Humidity: ")); Serial.print(humidity); Serial.print(F(" % "));
        Serial.print(F("Temperature: ")); Serial.print(temperature); Serial.println(F(" *C"));
      #endif
      
      sendMqtt(MQTT_TEMPERATURE, (float)temperature);
      sendMqtt(MQTT_HUMIDITY, (float)humidity);
    }
  }
  //keep last readings
  temp = temperature;
  hum = humidity;
  count++;
}
/*-------------------------------------------------------*/
void LED_Blink()
{
  static int LEDstate = LOW;
  
  if(LEDstate == LOW) LEDstate = HIGH;
  else LEDstate = LOW;
  
  digitalWrite(LED_BUILTIN, LEDstate); 
}
/*-------------------------------------------------------*/
void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RELAY1PIN, OUTPUT);
  pinMode(RELAY2PIN, OUTPUT);
  digitalWrite(RELAY1PIN, HIGH);
  digitalWrite(RELAY2PIN, HIGH);
  
  
  #ifdef DEBUG
    Serial.begin(57600);
    Serial.println(F("\nsetup:[DHT Eth with Timer]\n"));
    Serial.println(F("setup:[DHTInit]"));
  #endif
  dht.begin();            // inicjalizacja czujnika

  EthInit();
  delay(1000);
  mqttConnect();

  #ifdef DEBUG
    Serial.println(F("setup:[ThreadsInit]"));
  #endif
  //threads definition
  thread.attach(0, 5000, DHT_Read); //thread 0: get DHT values
  #ifdef DEBUG
    Serial.println(F("setup:[Th: 0, 5000, DHT_Read]"));
  #endif
//  thread.attach(1, 1000, LED_Blink); //thread 0: get DHT values
//  #ifdef DEBUG
//    Serial.println(F("[Th: 1, 1000, LED_Blink]"));
//  #endif
  thread.attach(2, 500, mqttLOOP); //thread 2: connect to MQTT
  #ifdef DEBUG
    Serial.println(F("setup:[Th: 2, 1000, mqttLOOP]"));
  #endif

}
/*-------------------------------------------------------*/
void loop() {
  // put your main code here, to run repeatedly:

  thread.process();
  delay (100);  
}
/*-------------------------------------------------------*/
void mqttLOOP()
{
    client.loop();
    if(client.connected())
    {
      #ifdef DEBUG
        Serial.print(F("."));
      #endif
    }
    else
    {
      #ifdef DEBUG
        Serial.println(F("mqttLOOP: Disconnected"));
      #endif
      mqttConnect();
    }
}
/*-------------------------------------------------------*/
void mqttConnect()
{
  char topicChar[6];
  //
  //if (client.connect("DEV_Arduino_1", "arduino1", "HCfx51Zn")) 
  if (client.connect(mqttID, mqttUser, mqttPass))
  {
    client.publish("outTopic","hello world from arduino1");
    #ifdef DEBUG
      Serial.println(F("mqttConnect:[MQTT Connected]"));
      Serial.println(F("MQTT.Publish: outTopic => hello world from arduino1"));
    #endif  
    
    //subscribe topic(s)
    //MQTT_RELAY1
    itoa(MQTT_RELAY1, topicChar, 10);
    client.subscribe(topicChar);
    #ifdef DEBUG
      Serial.print(F("mqttConnect:[MQTT Subsribe topic: ")); 
      Serial.print(topicChar); 
      Serial.print(F("]"));
    #endif  
    //MQTT_RELAY2
    itoa(MQTT_RELAY2, topicChar, 10);
    client.subscribe(topicChar);
    #ifdef DEBUG
      Serial.print(F("mqttConnect:[MQTT Subsribe topic: ")); 
      Serial.print(topicChar); 
      Serial.print(F("]"));
    #endif  
  }
  else
  {
    #ifdef DEBUG
      Serial.println(F("mqttConnect:ERR: MQTT not connected!"));
    #endif  
  }
}
/*-------------------------------------------------------*/
void sendMqtt(int topic, int value)
{
  char topicChar[6];
  char dataChar[6];
  itoa(topic, topicChar, 10);
  itoa(value, dataChar, 10);
  client.publish(topicChar, dataChar);
      
  #ifdef DEBUG
    Serial.println();
    Serial.print(F("sendMqttInt:Sending MQTT: "));
    Serial.print(topicChar);
    Serial.print(F(" => "));
    Serial.println(dataChar);
  #endif
}
/*-------------------------------------------------------*/
void sendMqtt(int topic, float value)
{
  char topicChar[6];
  char dataChar[8];
  itoa(topic, topicChar, 10);
  ftoa(value, dataChar);
  client.publish(topicChar, dataChar);
      
  #ifdef DEBUG
    Serial.println();
    Serial.print(F("sendMqttFloat:Sending MQTT: "));
    Serial.print(topicChar);
    Serial.print(F(" => "));
    Serial.println(dataChar);
  #endif
}
/**************************************************
 *
 *    ftoa - converts float to string
 *
 ***************************************************
 *
 *    This is a simple implemetation with rigid
 *    parameters:
 *            - Buffer must be 8 chars long
 *            - 3 digits precision max
 *            - absolute range is -524,287 to 524,287 
 *            - resolution (epsilon) is 0.125 and
 *              always rounds down
 **************************************************/
 void ftoa(float Value, char* Buffer)
 {
     union
     {
         float f;
     
         struct
         {
             unsigned int    mantissa_lo : 16;
             unsigned int    mantissa_hi : 7;    
             unsigned int     exponent : 8;
             unsigned int     sign : 1;
         };
     } helper;
     
     unsigned long mantissa;
     signed char exponent;
     unsigned int int_part;
     char frac_part[3];
     int i, count = 0;
     
     helper.f = Value;
     //mantissa is LS 23 bits
     mantissa = helper.mantissa_lo;
     mantissa += ((unsigned long) helper.mantissa_hi << 16);
     //add the 24th bit to get 1.mmmm^eeee format
     mantissa += 0x00800000;
     //exponent is biased by 127
     exponent = (signed char) helper.exponent - 127;
     
     //too big to shove into 8 chars
     if (exponent > 18)
     {
         Buffer[0] = 'I';
         Buffer[1] = 'n';
         Buffer[2] = 'f';
         Buffer[3] = '\0';
         return;
     }
     
     //too small to resolve (resolution of 1/8)
     if (exponent < -3)
     {
         Buffer[0] = '0';
         Buffer[1] = '\0';
         return;
     }
     
     count = 0;
     
     //add negative sign (if applicable)
     if (helper.sign)
     {
         Buffer[0] = '-';
         count++;
     }
     
     //get the integer part
     int_part = mantissa >> (23 - exponent);    
     //convert to string
     itoa(int_part, &Buffer[count], 10);
     
     //find the end of the integer
     for (i = 0; i < 8; i++)
         if (Buffer[i] == '\0')
         {
             count = i;
             break;
         }        
 
     //not enough room in the buffer for the frac part    
     if (count > 5)
         return;
     
     //add the decimal point    
     Buffer[count++] = '.';
     
     //use switch to resolve the fractional part
     switch (0x7 & (mantissa  >> (20 - exponent)))
     {
         case 0:
             frac_part[0] = '0';
             frac_part[1] = '0';
             frac_part[2] = '0';
             break;
         case 1:
             frac_part[0] = '1';
             frac_part[1] = '2';
             frac_part[2] = '5';            
             break;
         case 2:
             frac_part[0] = '2';
             frac_part[1] = '5';
             frac_part[2] = '0';            
             break;
         case 3:
             frac_part[0] = '3';
             frac_part[1] = '7';
             frac_part[2] = '5';            
             break;
         case 4:
             frac_part[0] = '5';
             frac_part[1] = '0';
             frac_part[2] = '0';            
             break;
         case 5:
             frac_part[0] = '6';
             frac_part[1] = '2';
             frac_part[2] = '5';            
             break;
         case 6:
             frac_part[0] = '7';
             frac_part[1] = '5';
             frac_part[2] = '0';            
             break;
         case 7:
             frac_part[0] = '8';
             frac_part[1] = '7';
             frac_part[2] = '5';                    
             break;
     }
     
     //add the fractional part to the output string
     for (i = 0; i < 3; i++)
         if (count < 7)
             Buffer[count++] = frac_part[i];
     
     //make sure the output is terminated
     Buffer[count] = '\0';
 }
 
