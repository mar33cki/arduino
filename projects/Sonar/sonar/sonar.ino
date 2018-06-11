#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>  // F Malpartida's NewLiquidCrystal library
//Download: https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads


// This are different than earlier/different I2C LCD displays
#define I2C_ADDR    0x20  // Define I2C Address for the PCF8574T 
//---(Following are the PCF8574 pin assignments to LCD connections )----
// This are different than earlier/different I2C LCD displays
#define BACKLIGHT_PIN  3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7
#define  LED_OFF  1
#define  LED_ON  0


int Trig = 2;   // Numer pinu wyzwolenia
int Echo = 3;   // Numer pinu odpowiedzi
 

/*-----( Declare objects )-----*/
LiquidCrystal_I2C  lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);


void setup() {
  // put your setup code here, to run once:
  // Inicjalizacja portu szeregowego
  Serial.begin(9600);
 
  // Konfiguracja pinów
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);


   //LCD section
  lcd.begin (16, 2); // initialize the lcd
  // Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(LED_OFF);
  lcd.backlight();  //Backlight ON if under program control
  // Reset the display
  lcd.clear();
  lcd.setCursor(0, 0); //Start at character 1 on line 0 
  delay(500);
  lcd.home();
  
}

void loop() 
{
  // put your main code here, to run repeatedly:
  
  int  Distance;  // Odległość w centymetrach
    
  Distance = MeasureDistanceAvg();
  DebugData(Distance);
  DisplayLcd(Distance);

  // Opóźnienie kolejnego pomiaru
  delay(1000);

 
     
}

void DisplayLcd(int dist)
{
  // Print our characters on the LCD
  //lcd.clear();
  //
  lcd.setCursor(0, 0); //Start at character 1 on line 0 
  lcd.print("Dist [cm]: ");
  lcd.print(dist); 
}
//-------------------------------------------
// takes few measurements, returns average
int MeasureDistanceAvg()
{
  int DistanceAvg = 0;
  int samples = 5;
  
  for(int i=0; i<samples; i++) 
  {
     DistanceAvg = DistanceAvg + MeasureDistance();     
  }
  DistanceAvg = DistanceAvg/ samples;
  return DistanceAvg;
}

//-------------------------------------------
// Measure single distance in [cm]
int MeasureDistance()
{
  int dist; //odleglosc w cm
  long EchoTime;  // Czas trwania sygnału ECHO
 // Ustawiamy TRIG w stan niski na 2us
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
 
  // Ustawiamy TRIG w stan wysoki na 10us
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
 
  // Ustawiamy TRIG w stan niski - rozpoczynamy pomiar
  digitalWrite(Trig, LOW);
 
  // Odczytujamy czas trwania stanu wysokiego na pinie ECHO
  EchoTime = pulseIn(Echo, HIGH);
 
  // Obliczamy odległość
  dist = EchoTime / 58;
  return dist;
}

//-------------------------------------------
void DebugData(int Distance)
{
    Serial.print("Odleglosc [cm]: ");
    Serial.println(Distance);
}

