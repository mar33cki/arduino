//MQTT DeviceID, User, Password
const char mqttID[] = "DEV_nodeMCU_1";
const char mqttUser[] = "user";
const char mqttPass[] = "pass";
const char mqtt_server[] = "mqttserver"; //domain name
//IPAddress mqtt_server(172, 16, 32, 98); //IP address

void mqttConnect()
{
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  #ifdef DEBUG
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) 
    {
      Serial.print((char)payload[i]);
    }
    Serial.println();
  #endif

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    #ifdef DEBUG
      Serial.print("Attempting MQTT connection...");
    #endif  
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      #ifdef DEBUG
        Serial.println("connected");
      #endif
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
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
