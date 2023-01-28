//const int analogPin = A0;
//const int digitalPin = D2;
#define analogPIN 35
#define digitalPIN 34

void setup() {
  Serial.begin(9600);
  Serial.println("Program has started.");
  //pinMode(analogPin, INPUT);
  //pinMode(digitalPIN, INPUT);
}

void loop() {
  Serial.print("The analog value is: ");
  Serial.println(analogRead(analogPIN));

  Serial.print("The digital value is: ");
  Serial.println(digitalRead(digitalPIN));

  if(digitalRead(digitalPIN) == HIGH)
    Serial.println("CO detected.");
  else
    Serial.println("CO not detected.");

  delay(1000);
}
