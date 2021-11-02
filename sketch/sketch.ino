// ---Imports---
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// ---WIFI---
WiFiClient espClient;
const char* ssid = "VIVOFIBRA-7FF6";
const char* password = "c662de7ff6";

// ---MQTT---
PubSubClient client(espClient);
const char* mqtt_server = "mqtt.eclipseprojects.io";
const char* mqttTopic = "caikmoraes/luminosity";
const char* mqttClientId = "caikmoraes-mark1";

// ---NODEMCU SETUP---
int pinoSensorLuz = A0;               
int valorLuz = 0;  

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

void publishData(){
  Serial.print("Luminosidade: ");
  Serial.println(valorLuz);
  client.publish(mqttTopic, String(valorLuz).c_str(), true);
}
 
void setup()
{
     Serial.begin(9600);
     pinMode(LED_BUILTIN, OUTPUT);     
     setup_wifi();
     client.setServer(mqtt_server, 1883);
}
 
void loop()
{
  if(!client.connected()){
    reconnect();
  }
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
  publishData();
  delay(1000);                   
}
