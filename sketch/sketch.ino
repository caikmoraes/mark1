int pinoSensorLuz = A0;               
int valorLuz = 0;            
 
void setup()
{
     Serial.begin(9600);
     pinMode(LED_BUILTIN, OUTPUT);     
}
 
void loop()
{
  valorLuz = analogRead(pinoSensorLuz);     
  Serial.println(valorLuz);
  if(valorLuz < 1010)
  {                
    digitalWrite(LED_BUILTIN,LOW);
  }
  else
  {                    
    digitalWrite(LED_BUILTIN,HIGH);
  }
  delay(100);                   
}
