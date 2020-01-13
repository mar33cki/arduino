/*
 * BMP085/BMP180 example
 * NodeMCU  | BMP180
 *  3V3     | 3V3 
 *  GND     | GND
 *  D1/SCL  | D1/SCL
 *  D2/SDA  | D2/SDA
 */
#include <Wire.h>
#include <Adafruit_BMP085.h>
 
Adafruit_BMP085 bmp;
 
void setup()
{
  Serial.begin(9600);
 
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
    // Odczytujemy temperaturę
    Serial.print("Temperatura = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
    
    // Odczytujemy cisnienie
    Serial.print("Cisnienie = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");
    
 
    // Obliczamy wysokosc dla domyslnego cisnienia przy pozimie morza
    // p0 = 1013.25 millibar = 101325 Pascal
    Serial.print("Wysokosc = ");
    Serial.print(bmp.readAltitude());
    Serial.println(" metrow");
    
    // Jesli znamy aktualne cisnienie przy poziomie morza,
    // mozemy dokladniej wyliczyc wysokosc, padajac je jako parametr
    Serial.print("Rzeczywista wysokosc = ");
    Serial.print(bmp.readAltitude(102520));
    Serial.println(" metrow");
    
    Serial.println();
    delay(500);
}
