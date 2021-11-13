// ---Imports---
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Stepper.h>

// ---WIFI---
WiFiClient espClient;
// const char* ssid = "VIVOFIBRA-7FF6";
const char* ssid = "testeMark1";
// const char* password = "c662de7ff6";
const char* password = "para1234";

// ---MQTT---
PubSubClient client(espClient);
const char* mqtt_server = "broker.mqtt-dashboard.com";
const char* mqttPublishTopic = "caikmoraes/luminosity";
const char* mqttSubscribeTopic = "caikmoraes/controller";
const char* mqttClientId = "caikmoraes-mark1";


// ---NODEMCU SETUP---
int pinoSensorLuz = A0;               
int valorLuz = 0;  
bool light = true;

// ---STEPPER MOTOR---
const int stepsPerRevolution = 500;
const double volta = 2047;
const int ciclo = 17;
const int stepperSpeed = 60;
int currentState = 0;
int oldState = 0;
bool manual = false;
Stepper myStepper(stepsPerRevolution, D8,D6,D7,D5);

// ---FUNCTIONS---

void setup()
{
     Serial.begin(9600);
     pinMode(LED_BUILTIN, OUTPUT);     
     setup_wifi();
     client.setServer(mqtt_server, 1883);
     client.setCallback(callback);
     myStepper.setSpeed(stepperSpeed);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  valorLuz = analogRead(pinoSensorLuz);
  if(valorLuz < 1000 && !manual)
  {                
    digitalWrite(LED_BUILTIN, light);
    closeCurtain();
  }
  if(valorLuz >= 1000 && !manual)
  {                    
    digitalWrite(LED_BUILTIN, !light);
    openCurtain();
  }
  publishData();
  delay(1000);                   
}

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
  delay(1000);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // ... and resubscribe
      client.subscribe(mqttSubscribeTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  
  if ((char)payload[0] == '1' || (char)payload[0] == '0') {
    String payloadMsg = String((char)payload[0]);
    int msg = payloadMsg.toInt();
    if(msg == 1 && currentState == 0) {
//    if(msg == 1) {
      closeCurtain();
      manual = true;
    } 
    else if(msg == 0 && currentState == 1) {
//    else if(msg == 0) {
      openCurtain();
      manual = true;
    }
    
  } else if((char)payload[0] == 'a') {
    Serial.println("Automating");
    manual = false;
  } else {
    Serial.println("Invalid command.");
  }
}

void publishData(){
  Serial.print("LUMINOSIDADE: ");
  Serial.println(valorLuz);
  client.publish(mqttPublishTopic, String(valorLuz).c_str(), true);
  delay(10);
}

void openCurtain(){
  currentState = 0;
  if(oldState != currentState){
    oldState = currentState;
    Serial.println("ABRINDO");
    for(int j = 0; j < ciclo; j++) {
      Serial.print("Volta ");
      Serial.println(j + 1);
      for(int i = 0; i < 3; i++){
        myStepper.step(-volta/3);
        delay(1);  
      } 
    }  
  }
}

void closeCurtain(){
  currentState = 1;
  if(oldState != currentState){
    oldState = currentState;
    Serial.println("FECHANDO");
    for(int j = 0; j < ciclo; j++) {
      Serial.print("Volta ");
      Serial.println(j + 1);
      for(int i = 0; i < 3; i++){
        myStepper.step(volta/3);
        delay(1);  
      } 
    }  
  }
}
 
