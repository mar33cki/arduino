//MQTT DeviceID, User, Password
const char mqttID[] = "DEV_nodeMCU_Gate";
const char mqttUser[] = "arduino2";
const char mqttPass[] = "fWan7ovS";
//const char mqtt_server[] = "192.168.50.200"; //domain name
IPAddress mqtt_server(192, 168, 50, 200); //IP address


void callback(char* topic, byte* payload, unsigned int length) 
{
  #ifdef DEBUG
    Serial.println();
    Serial.print(F("Message arrived ["));
    Serial.print(topic);
    Serial.print(F("] "));
    for (int i = 0; i < length; i++) 
    {
      Serial.print((char)payload[i]);
    }
    Serial.println();
  #endif

  switch(atoi(topic))
  {
    case MQTT_STOPIC:
      if ((char)payload[0] == '1') 
      {
        digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
        // but actually the LED is on; this is because it is acive low on the ESP-01)
      } 
      else 
      {
        digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
      }
      #ifdef DEBUG
        Serial.println("case: MQTT_STOPIC");
      #endif 
      break;
    
    case MQTT_STOPIC_REL1:
      if ((char)payload[0] == '1') 
      {
        digitalWrite(REL1, LOW); //toggle state
        delay(500);
        digitalWrite(REL1, HIGH);
      
      } 
      else 
      {
        digitalWrite(REL1, HIGH); 
      }
      #ifdef DEBUG
        Serial.println("case: MQTT_STOPIC_REL1");
      #endif
      break;

    case MQTT_STOPIC_REL2:
      if ((char)payload[0] == '1') 
      {
        digitalWrite(REL2, LOW); //toggle state
        delay(500);
        digitalWrite(REL2, HIGH);
      } 
      else 
      {
        digitalWrite(REL2, HIGH);
      }
      #ifdef DEBUG
        Serial.println("case: MQTT_STOPIC_REL2");
      #endif
      break;

//    case default:
//      //unknown topic
//      #ifdef DEBUG
//        Serial.println("case: default/unknown topic");
//      #endif
//      break;
  }
}

/*-------------------------------------------------------*/
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    #ifdef DEBUG
      Serial.print("Attempting MQTT connection...");
    #endif  
    // Attempt to connect
    if (client.connect(mqttID, mqttUser, mqttPass)) {
      #ifdef DEBUG
        Serial.println("connected");
      #endif
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      
      // ... and resubscribe
      client.subscribe("7777"); //MQTT_STOPIC
      client.subscribe("1000"); //MQTT_STOPIC_REL1
      client.subscribe("1001"); //MQTT_STOPIC_REL2
    } 
    else 
    {
      #ifdef DEBUG
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
      #endif
      // Wait 5 seconds before retrying
      delay(5000);
    }
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
