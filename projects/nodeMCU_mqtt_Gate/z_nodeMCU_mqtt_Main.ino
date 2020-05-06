void HeartBit()
{
  #ifdef DEBUG
    Serial.println(F("executing HeartBit"));
  #endif
}
/*-------------------------------------------------------*/
void setup() 
{
  #ifdef DEBUG
    Serial.begin ( 115200 );
    Serial.println(); 
    Serial.println(F("-------------------------------------"));
    Serial.println(F("NodeMCU Gate controller"));
    Serial.println(F("setup: begin"));
  #endif

  //init BMP
  if ( !bmp.begin() ) 
  {
    Serial.println(F("setup: BMP180 NOK!"));
    while(1);
  } 
  else Serial.println(F("setup: BMP180 OK"));

  //init DHT
  dht.begin();
  #ifdef DEBUG
    Serial.println(F("setup: DHT init"));
  #endif

  //init DIO
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(REL1, OUTPUT);
  pinMode(REL2, OUTPUT);
  digitalWrite(REL1, HIGH);
  digitalWrite(REL2, HIGH);

  /* setup wifi */
    /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);  
  WiFi.begin ( ssid, password );
  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) 
  {
    digitalWrite(LED_BUILTIN, HIGH); //toggle LED_BUILTIN while connecting
    delay ( 500 ); 
    digitalWrite(LED_BUILTIN, LOW); //leave LED_BUILTIN light
    #ifdef DEBUG
      Serial.print ( "." );
    #endif  
  }
  // WiFi connexion is OK
  #ifdef DEBUG
    Serial.println ( F("setup: [WiFi]") ); 
    Serial.print ( F("Connected to ") ); Serial.println ( ssid );
    Serial.print ( F("IP address: ") ); Serial.println ( WiFi.localIP() );
  #endif

  #ifdef DEBUG
    Serial.println(F("setup:[MQTT init]"));
  #endif
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  #ifdef DEBUG
    Serial.println(F("setup:[ThreadsInit]"));
  #endif
  //threads definition
//  thread.attach(0, 500, mqttLOOP); //thread 2: connect to MQTT
//  #ifdef DEBUG
//    Serial.println(F("setup:[Th: 0, 500, mqttLOOP]"));
//  #endif
  
  thread.attach(1, 30000, Sensors_Read); //thread 0: get DHT, BMP values
  #ifdef DEBUG
    Serial.println(F("setup:[Th: 1, 30000, Sensors_Read]"));
  #endif

//  thread.attach(2, 100, HeartBit); //thread 2, 100, HeartBit
//  #ifdef DEBUG
//    Serial.println(F("setup:[Th: 2, 100, HeartBit]"));
//  #endif

}
/*-------------------------------------------------------*/
void loop() 
{
  // put your main code here, to run repeatedly:

  thread.process();
  
  if (!client.connected()) {
    reconnect();
  }
  #ifdef DEBUG
    static int count = 0;
    if(count %10 != 0) {
    Serial.print(F("."));}
    count ++;
  #endif
  
  client.loop();
  delay(100);
}
