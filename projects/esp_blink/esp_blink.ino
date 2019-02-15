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

int Trig = D1;   // Numer pinu wyzwolenia
int Echo = D2;   // Numer pinu odpowiedzi

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

void setup() {
  // put your setup code here, to run once:
  // Konfiguracja pinów
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
//  pinMode(4, OUTPUT);
  Serial.begin(115200);
  Serial.println(F("hello world"));
}

void loop() 
{
  // put your main code here, to run repeatedly:
  int  Distance;  // Odległość w centymetrach

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  Serial.println(F("digitalWrite:D4:HIGH"));

  Distance = MeasureDistanceAvg();
  Serial.print(F("Distance: ")); Serial.println(Distance);

//  digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
//  delay(1000);              // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  Serial.println(F("digitalWrite:D4:LOW"));
//  digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second

}
