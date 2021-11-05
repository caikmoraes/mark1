import paho.mqtt.client as mqtt

mqttClient = mqtt.Client()
mqttClient.connect("mqtt.eclipseprojects.io", 1883)
mqttClient.publish("caikmoraes/control", "LOW")
mqttClient.loop(2)