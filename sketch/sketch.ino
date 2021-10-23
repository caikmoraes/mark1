#include <Stepper.h> 

const int stepsPerRevolution = 200;
Stepper myStepper(stepsPerRevolution, D0,D1,D2,D3); 

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  myStepper.setSpeed(100);
  Serial.begin(9600);
}

void loop() {
  float temp = getTemperature();
//  Serial.print("Temperatura = ");
//  Serial.print(temp);
//  Serial.println("\xC2\xB0");
  Serial.println("Girando");
  myStepper.step(5000);
  Serial.println("terminou");
  delay(2000);
}

float getTemperature(){
  int reading = analogRead(A0);  
  float voltage = reading * (3300 / 1024);
  float temperatureC = (voltage - 500) / 10; 
  return temperatureC;
}
