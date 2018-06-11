#include "DHT_Eth_config.h"

Timers <THREADS> thread; //definicja watkow

//DHT sensor
DHT dht(DHTPIN, DHTTYPE); // definicja czujnika

// Initialize the Ethernet client library
EthernetClient client;

void EthInit()
{  
  #ifdef DEBUG
    Serial.println(F("[EthInit]"));
  #endif
  
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0)
  { 
    #ifdef DEBUG
      Serial.println(F("Failed to configure Ethernet using DHCP"));
    #endif
  }

  #ifdef DEBUG
    Serial.print(F("IP address: "));
    for (byte thisByte = 0; thisByte < 4; thisByte++) 
    {
      // print the value of each byte of the IP address:
      Serial.print(Ethernet.localIP()[thisByte], DEC);
      Serial.print(F("."));   
    }
    Serial.println();
  #endif
}

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
      Serial.println(F("DHT fail!"));
    #endif
  } 
  else
  {
    if (humidity != hum || temperature != temp)
    {
      // Jeśli tak, wyświetlamy wyniki pomiaru
      #ifdef DEBUG
        Serial.print(count); Serial.print(F(". "));
        Serial.print(F("Humidity: ")); Serial.print(humidity); Serial.print(F(" % "));
        Serial.print(F("Temperature: ")); Serial.print(temperature); Serial.println(F(" *C"));
      #endif
    }
  }
  //keep last readings
  temp = temperature;
  hum = humidity;
  count++;
}

void LED_Blink()
{
  static int LEDstate = LOW;
  
  if(LEDstate == LOW) LEDstate = HIGH;
  else LEDstate = LOW;
  
  digitalWrite(LED_BUILTIN, LEDstate); 
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  
  #ifdef DEBUG
    Serial.begin(57600);
    Serial.println(F("\n[DHT Eth with Timer]\n"));
  
    Serial.println(F("[DHTInit]"));
  #endif
  dht.begin();            // inicjalizacja czujnika

  EthInit();

  #ifdef DEBUG
    Serial.println(F("[ThreadsInit]"));
  #endif
  //threads definition
  thread.attach(0, 5000, DHT_Read); //thread 0: get DHT values
  #ifdef DEBUG
    Serial.println(F("[Th: 0, 5000, DHT_Read]"));
  #endif
  thread.attach(1, 1000, LED_Blink); //thread 0: get DHT values
  #ifdef DEBUG
    Serial.println(F("[Th: 1, 1000, LED_Blink]"));
  #endif

}

void loop() {
  // put your main code here, to run repeatedly:

  thread.process();
  delay (100);
}
