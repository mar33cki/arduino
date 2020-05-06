//make all measurements here
//sendMqtt(MQTT_PTOPIC, mqtt_data);

void Sensors_Read()
{
  static float t_dht_p = 0, h_dht_p = 0, p_bmp_p = 0, t_bmp_p = 0; //previous states

  #ifdef DEBUG
    Serial.println(F("executing Sensors_Read"));
  #endif
     
  // DHT readings
  t_dht = dht.readTemperature();
  h_dht = dht.readHumidity();
  // BMP readings
  p_bmp = bmp.readPressure(); 
  p_bmp /= 100; //scale to hPa
  t_bmp = bmp.readTemperature();

   if(isnan(h_dht))
   {
    #ifdef DEBUG
      Serial.println(F("DHT.Humidity: fail, "));
    #endif
   }
  else
  {
    if(h_dht != h_dht_p)            //if value change
    {
      sendMqtt(MQTT_PTOPIC_HUMIDITY, h_dht); //send data
      h_dht_p = h_dht;              //keep last value
      #ifdef DEBUG
        Serial.print(F("DHT.Humidity: ")); Serial.print(h_dht); Serial.println(F(", "));
      #endif
    }    
  }
  
  if(isnan(t_dht)) Serial.println(F("DHT.Temperature: fail, "));
  else
  {
    if(t_dht != t_dht_p)        //if value change
    {
      sendMqtt(MQTT_PTOPIC_TEMPERATURE, t_dht); //send data
      t_dht_p = t_dht;              //keep last value
      #ifdef DEBUG
        Serial.print(F("DHT.Temperature: ")); Serial.print(t_dht); Serial.println(F(", "));
      #endif
    }
    
  }

    //temperature already stored from DHT
//  if(isnan(t_bmp)) Serial.println(F("BMP.Temperature: fail, "));
//  else
//  {
//    Serial.print(F("BMP.Temperature: ")); Serial.print(t_bmp); Serial.println(F(", "));
//  }
  
  if(isnan(p_bmp)) Serial.println(F("BMP.Pressure: fail, "));
  else 
  {
    if(p_bmp != p_bmp_p)        //if value change
    {
      sendMqtt(MQTT_PTOPIC_PRESSURE, p_bmp); //send data
      p_bmp_p = p_bmp;              //keep last value
      #ifdef DEBUG
        Serial.print(F("BMP.Pressure: ")); Serial.print(p_bmp); Serial.println(F(", "));
      #endif
    }
  }
}
