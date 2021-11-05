// ---Imports---
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Stepper.h>

// ---WIFI---
WiFiClient espClient;
const char* ssid = "VIVOFIBRA-7FF6";
const char* password = "c662de7ff6";

// ---MQTT---
PubSubClient client(espClient);
const char* mqtt_server = "mqtt.eclipseprojects.io";
const char* mqttTopic = "caikmoraes/luminosity";
const char* mqttSubscribeTopic = "caikmoraes/control";
const char* mqttClientId = "caikmoraes-mark1";

// ---NODEMCU SETUP---
int pinoSensorLuz = A0;               
int valorLuz = 0;  
bool light = true;

// ---STEPPER MOTOR---
const int stepsPerRevolution = 500;
const double volta = 2047;
int currentState = 0;
int oldState = 0;
Stepper myStepper(stepsPerRevolution, D8,D6,D7,D5);

// ---FUNCTIONS---

void setup_wifi() {
  
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(10);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    client.connect(mqttClientId);
  }
  delay(10);
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.println("chegou mensagem");
  String value = String((char*) payload);

  Serial.print("COMANDO: ");
  Serial.println(value);
  delay(10);
}

void publishData(){
  Serial.print("Luminosidade: ");
  Serial.println(valorLuz);
  client.publish(mqttTopic, String(valorLuz).c_str(), true);
  Serial.println("Publicou mensagem");
  delay(10);
}
 
void setup()
{
     Serial.begin(9600);
     pinMode(LED_BUILTIN, OUTPUT);     
     setup_wifi();
     client.setServer(mqtt_server, 1883);
     client.setCallback(callback);
     client.subscribe(mqttSubscribeTopic);
     myStepper.setSpeed(60);
}

void openCurtain(){
  for(int i = 0; i < 3; i++){
    myStepper.step(volta/3);
    delay(1);  
  }
}

void closeCurtain(){
  for(int i = 0; i < 3; i++){
    myStepper.step(-volta/3);
    delay(1);  
  }
}
 
void loop()
{
  delay(10);
  if(!client.connected()){
    reconnect();
  }
  valorLuz = analogRead(pinoSensorLuz);
  if(valorLuz < 1000)
  {                
    digitalWrite(LED_BUILTIN, light);
    currentState = 1;
    if(oldState != currentState){
      oldState = currentState;
      openCurtain();
    }
  }
  else
  {                    
    digitalWrite(LED_BUILTIN, !light);
    currentState = 0;
    if(oldState != currentState){
      oldState = currentState;
      closeCurtain();
    }
  }
  publishData();
  delay(1000);                   
}
