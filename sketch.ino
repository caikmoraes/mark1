void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  float temp = getTemperature();
  Serial.print("Temperatura = ");
  Serial.print(temp);
  Serial.println("\xC2\xB0");
  delay(2000);
}

float getTemperature(){
  int reading = analogRead(A0);  
  float voltage = reading * (3300 / 1024);
  float temperatureC = (voltage - 500) / 10; 
  return temperatureC;
}
