
void setup() 
{
  // put your setup code here, to run once:
  #ifdef DEBUG
    Serial.begin ( 115200 );
  #endif

  pinMode(LED_BUILTIN, OUTPUT);

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
    Serial.println ( F("") ); 
    Serial.print ( F("Connected to ") ); Serial.println ( ssid );
    Serial.print ( F("IP address: ") ); Serial.println ( WiFi.localIP() );
  #endif

  mqttConnect();

  #ifdef DEBUG
    Serial.println(F("setup:[ThreadsInit]"));
  #endif
  //threads definition
  thread.attach(0, 500, mqttLOOP); //thread 2: connect to MQTT
  #ifdef DEBUG
    Serial.println(F("setup:[Th: 0, 1000, mqttLOOP]"));
  #endif
//  thread.attach(1, 5000, DHT_Read); //thread 0: get DHT values
//  #ifdef DEBUG
//    Serial.println(F("setup:[Th: 1, 5000, DHT_Read]"));
//  #endif


}

void loop() 
{
  // put your main code here, to run repeatedly:
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}
