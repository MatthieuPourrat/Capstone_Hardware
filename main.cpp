#include <Arduino.h>

//const int analogMQ = A0;
//const int digitalMQ = D2;
#define analogMQ 35
#define digitalMQ 34
#define digitalDHT 32

float lastTime = 0;
int bitTime, bit = 0;
int dataBuffer[40] = {};
int humidityBuffer[16], temperatureBuffer[16] = {};
int checksumBuffer[8] = {};
int humidity, temperatur = 0.0;

void carbonMonoxide()
{
  Serial.println("---------------Carbon Monoxide--------------");
  Serial.print("The analog value is: ");
  Serial.println(analogRead(analogMQ));

  Serial.print("The digital value is: ");
  Serial.println(digitalRead(digitalMQ));

  if(digitalRead(digitalMQ) == HIGH)
    Serial.println("CO detected.");
  else
    Serial.println("CO not detected.");
  
  Serial.println("---------------------------------------------");

  delay(1000);
}

void temperature()
{
  Serial.println("---------------Temperature-------------------");
  pinMode(digitalDHT, OUTPUT);
  digitalWrite(digitalDHT, HIGH);
  delay(12);
  digitalWrite(digitalDHT, LOW);
  delay(0.04);
  pinMode(digitalDHT, INPUT);

  lastTime = micros();
  while(micros() - lastTime < 80 && digitalRead(digitalDHT) == 0);
  lastTime = micros();
  Serial.println(digitalRead(digitalDHT));
  while(micros() - lastTime < 80 && digitalRead(digitalDHT) == 1);
  Serial.println(digitalRead(digitalDHT));
  for(int i = 0; i < 40; i++)
  {
    lastTime = micros();
    while(micros() - lastTime < 50 && digitalRead(digitalDHT) == 0);
    Serial.println(digitalRead(digitalDHT));
    lastTime = micros();
    while(digitalRead(digitalDHT) == 1){Serial.println("Still 1.");};
    bitTime = micros() - lastTime;
    if(26 < bit < 28)
      bit = 0;
    else
      bit = 1;
    dataBuffer[i] = bit;
  }

  Serial.print("The temperature is: ");

  for(int i = 0; i < 16; i++)
  {
    humidityBuffer[i] = dataBuffer[i];
    humidity = (unsigned)humidityBuffer[i] << 1;
  }
    
  
  for(int i = 16; i < 32; i++)
  {
    temperatureBuffer[i] = dataBuffer[i];
    temperatur = (unsigned)temperatureBuffer[i] << 1;
  }
  for(int i = 32; i < 40; i++)
    checksumBuffer[i] = dataBuffer[i];

  Serial.println(temperatur);

  Serial.println("---------------------------------------------");
  delay(2000);

}

void setup() {
  Serial.begin(9600);
  Serial.println("Program has started.");
  //pinMode(analogMQ, INPUT);
  //pinMode(digitalPIN, INPUT);
}

void loop() {
  carbonMonoxide();
  temperature();

}
