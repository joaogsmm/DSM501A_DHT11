//DHT11
#include "DHT.h"
 
#define DHTPIN A1 // saida de dados do sensor
#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE);

//DSM501A
//Pin 1 Vout1 control.
//Pin 2 Vout2 output(PWM). SAIDA DE DADOS
//Pin 3 VCC.
//Pin 4 Vout1 output (PWM). SAIDA DE DADOS-
//Pin 5 GND.

int pin = 8; // saida de dados do sensor
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 1000;//sampe 1s ;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;
const int verde = 5; //pino led verde
const int amarelo = 6; //pino led amarelo
const int vermelho = 7; //pino led VERMELHO
//LEDS 5V
//SENSORES 5V 

void setup() {
  Serial.begin(9600);

  pinMode(8,INPUT);
  pinMode(verde,OUTPUT);
  pinMode(amarelo,OUTPUT);
  pinMode(vermelho,OUTPUT);
  
  //DSM501A
  pinMode(8,INPUT);
  starttime = millis();//get the current time;

  //DHT11
    Serial.println("DHT11 test!");
  dht.begin();
}

void loop() {
  //DSM501A
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;

  if ((millis()-starttime) > sampletime_ms)//if the sample time == 30s
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
    Serial.print("concentration = ");
    Serial.print(concentration);
    Serial.print(" 0.01cf  -  ");
    if (concentration < 25000) // LED VERDE 
    { 
  digitalWrite(verde, HIGH);
  digitalWrite(amarelo, LOW);
  digitalWrite(vermelho, LOW);
  }
    if (concentration > 30000 && concentration < 40000) // LED AMARELO 
    {
  digitalWrite(verde, LOW);
  digitalWrite(amarelo, HIGH);
  digitalWrite(vermelho, LOW);
    }
    
      if (concentration > 45000) // LED VERMELHO 
      {
  digitalWrite(verde, LOW);
  digitalWrite(amarelo, LOW);
  digitalWrite(vermelho, HIGH);
  }
    
    lowpulseoccupancy = 0;
    starttime = millis();
  }

//DHT11
    float h = dht.readHumidity();
  float t = dht.readTemperature();
  // testa se retorno é valido, caso contrário algo está errado.
  if (isnan(t) || isnan(h)) 
  {
    Serial.println("Failed to read from DHT");
  } 
  else
  {
    Serial.print("Umidade: ");
    Serial.print(h);
    Serial.print(" %t");
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println(" *C");
  }
  delay(1000);
}
