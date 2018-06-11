
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

  EthInit();      //init Ethernet with DHCP
  delay(500);
  IOexpInit();    //init I2C IO expander(s)
  mqttConnect();  //init MQTT publisher, subscriber

  #ifdef DEBUG
    Serial.println(F("setup:[ThreadsInit]"));
  #endif
  
  //threads definition
  // thread 0
  thread.attach(0, 5000, DHT_Read); //thread 0: get DHT values
  #ifdef DEBUG
    Serial.println(F("setup:[Th: 0, 5000, DHT_Read]"));
  #endif
  
  //thread 1
  thread.attach(1, 100, EvaluateIOStates); //thread 0: get IO expander states
  #ifdef DEBUG
    Serial.println(F("setup:[Th: 1, 100, EvaluateIOStates]"));
  #endif

  //thread 2
  thread.attach(2, 500, mqttLOOP); //thread 2: connect to MQTT
  #ifdef DEBUG
    Serial.println(F("setup:[Th: 2, 1000, mqttLOOP]"));
  #endif

}
/*-------------------------------------------------------*/
void loop() {
  // put your main code here, to run repeatedly:
  //all done in threads - configured in setup
  thread.process();
  delay (100);  
}

/*-------------------------------------------------------*/

 
