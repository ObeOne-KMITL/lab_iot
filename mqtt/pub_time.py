import paho.mqtt.client as mqtt
import time

publish_start_time = None

mqttc = mqtt.Client()
mqttc.connect("broker.mqttdashboard.com", 1883)

while True:
        publish_start_time = time.time()

        result = mqttc.publish("test/topic1", "Hello", qos=0)
        if result.rc == mqtt.MQTT_ERR_SUCCESS:
                elapsed_time = time.time() - publish_start_time
                print("Message published successfully")
                print(f"Time taken to send message and receive acknowledgment: {elapsed_time:.6f} seconds")

        else:
                print(f"Failed to publish message. Error code: {result.rc}")

        time.sleep(2)
