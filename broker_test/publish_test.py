import paho.mqtt.client as mqtt

mqttClient = mqtt.Client()
mqttClient.connect("broker.mqtt-dashboard.com", 1883)
mqttClient.publish("caikmoraes/controller", "VAI")