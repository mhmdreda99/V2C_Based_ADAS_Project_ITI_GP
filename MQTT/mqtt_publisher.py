import paho.mqtt.publish as publish
from paho import mqtt

# Define the MQTT broker and topic information
mqtt_broker = "7b523ff57b624c40bec79f5c35f6bffa.s2.eu.hivemq.cloud"
mqtt_port = 8883  # Port for MQTT over TLS
mqtt_topic = "encyclopedia/temperature"
username = "yasmen"
password = "Yas12345"

# Set the TLS options for a secure connection
tls_args = {
    "tls_version": mqtt.client.ssl.PROTOCOL_TLS,
}

# Publish a message to the specified MQTT topic
message = "hot"
publish.single(mqtt_topic,message,qos=1,hostname=mqtt_broker,port=mqtt_port,
    auth={"username": username, "password": password},
    tls=tls_args,
)
print("Done")

print("Message published to topic:", mqtt_topic)
