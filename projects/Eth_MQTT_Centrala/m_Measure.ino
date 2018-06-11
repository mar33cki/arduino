//DHT sensor
DHT dht(DHTPIN, DHTTYPE); // definicja czujnika
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
      Serial.println();
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
