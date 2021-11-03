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
const char* mqttControlTopic = "caikmoraes/control";
const char* mqttClientId = "caikmoraes-mark1";

// ---NODEMCU SETUP---
int pinoSensorLuz = A0;               
int valorLuz = 0;  
bool light = true;

// ---STEPPER MOTOR---
const int stepsPerRevolution = 500;
Stepper myStepper(stepsPerRevolution, D8,D6,D7,D5);

// ---FUNCTIONS---

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
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
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    client.connect(mqttClientId);
  }
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  
  Serial.println("CHEGOU MENSAGEM NO TOPICO DE CONTROLE");
  String currentTopic;
  for(int i = 0; i < length; i++){
    char c = (char)topic[i];
    currentTopic += c;
  }
  if(currentTopic.equals(mqttControlTopic)){
    String msg;
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }
    
    if (msg.equals("L"))
    {
      light = true;
      Serial.println("Configuração LIGHT");
    }
    
    if (msg.equals("D"))
    {
      light = false;     
      Serial.println("Configuração DARK");
    }
  }
}

void publishData(){
  Serial.print("Luminosidade: ");
  Serial.println(valorLuz);
  client.publish(mqttTopic, String(valorLuz).c_str(), true);
  Serial.println("Publicou mensagem");
}
 
void setup()
{
     Serial.begin(9600);
     pinMode(LED_BUILTIN, OUTPUT);     
     setup_wifi();
     client.setServer(mqtt_server, 1883);
     client.setCallback(callback);
     myStepper.setSpeed(60);
}
 
void loop()
{
  if(!client.connected()){
    reconnect();
  }
  valorLuz = analogRead(pinoSensorLuz);
  if(valorLuz < 1010)
  {                
    digitalWrite(LED_BUILTIN, light);
  }
  else
  {                    
    digitalWrite(LED_BUILTIN, !light);
  }
  publishData();
  myStepper.step(2048);
  delay(1000);                   
}
