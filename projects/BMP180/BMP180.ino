/*
 * BMP085/BMP180 example
 * NodeMCU  | BMP180
 *  3V3     | 3V3 
 *  GND     | GND
 *  D1/SCL  | D1/SCL
 *  D2/SDA  | D2/SDA
 */
#include <Wire.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>

#define DHTTYPE   DHT22       // DHT type (DHT11, DHT22)
#define DHTPIN    D3          // DHT Pin
 
//NodeMCU pinout
#define LED_BUILTIN D4
#define D0 16
#define D1 5 // I2C Bus SCL (clock)
#define D2 4 // I2C Bus SDA (data)
#define D3 0
#define D4 2 // Same as "LED_BUILTIN", but inverted logic
#define D5 14 // SPI Bus SCK (clock)
#define D6 12 // SPI Bus MISO
#define D7 13 // SPI Bus MOSI
#define D8 15 // SPI Bus SS (CS)
#define D9 3 // RX0 (Serial console)
#define D10 1 // TX0 (Serial console)

Adafruit_BMP085 bmp;
 
//float   t = 0 ;
//float   h = 0 ;
//float   p = 0;
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(115200);
 
  // Jako parametr mozemy podav dokladnosc - domyslnie 3
  // 0 - niski pobór energii - najszybszy pomiar
  // 1 - standardowy pomiar
  // 2 - wysoka precyzja
  // 3 - super wysoka precyzja - najwolniejszy pomiar
  if (!bmp.begin())
  {
    Serial.println("Nie odnaleziono czujnika BMP085 / BMP180");
    while (1) {}
  }
}
 
void loop()
{
    Serial.print(F("BMP180: "));
    // Odczytujemy temperaturę
    Serial.print(F("Temperatura = "));
    Serial.print(bmp.readTemperature());
    Serial.print(F(" *C; "));
    
    // Odczytujemy cisnienie
    Serial.print(F("Cisnienie = "));
    Serial.print(bmp.readPressure());
    Serial.print(F(" Pa; "));
    
 
    // Obliczamy wysokosc dla domyslnego cisnienia przy pozimie morza
    // p0 = 1013.25 millibar = 101325 Pascal
//    Serial.print("Wysokosc = ");
//    Serial.print(bmp.readAltitude());
//    Serial.print(" m; ");
    
    // Jesli znamy aktualne cisnienie przy poziomie morza,
    // mozemy dokladniej wyliczyc wysokosc, padajac je jako parametr
//    Serial.print("Rzeczywista wysokosc = ");
//    Serial.print(bmp.readAltitude(102520));
//    Serial.println(" metrow");
    
    //Serial.println();

    Serial.print(F(" DHT22: "));
    Serial.print(F("Temperatura = "));
    Serial.print(dht.readTemperature());
    Serial.print(F(" *C; "));
    Serial.print(F("Wilgotnosc = "));
    Serial.print(dht.readHumidity());
    Serial.println(F(" %; "));

    delay(5000);
}
