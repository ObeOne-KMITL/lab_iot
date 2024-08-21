import paho.mqtt.client as mqtt
import time

mqttc = mqtt.Client()
mqttc.connect("broker.mqttdashboard.com", 1883)

while True:
        mqttc.publish("test/topic1", "Hello")
        time.sleep(2)
