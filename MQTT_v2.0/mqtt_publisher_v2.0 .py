import paho.mqtt.publish as publish
from paho import mqtt

# Define the MQTT broker and topic information
mqtt_broker = "7b523ff57b624c40bec79f5c35f6bffa.s2.eu.hivemq.cloud"
mqtt_port = 8883  # Port for MQTT over TLS
mqtt_topic1 = "encyclopedia/temperature"
mqtt_topic2 = "encyclopedia/humidity"
username = "yasmen"
password = "Yas12345"

# Set the TLS options for a secure connection
tls_args = {
    "tls_version": mqtt.client.ssl.PROTOCOL_TLS,
}

# Publish a message to the specified MQTT topic
message = "hot"
# Second message
message2 = "another_message"

publish.single(mqtt_topic1,message,qos=1,hostname=mqtt_broker,port=mqtt_port,
    auth={"username": username, "password": password},
    tls=tls_args,
)
# Publish the second message to the same MQTT topic
publish.single(mqtt_topic2, message2, qos=1, hostname=mqtt_broker, port=mqtt_port,
               auth={"username": username, "password": password},
               tls=tls_args,
)
#print("Done")

print("Message published to topic:", mqtt_topic1)
print("Message published to topic:", mqtt_topic2)