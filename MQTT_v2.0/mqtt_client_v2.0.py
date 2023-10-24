# enable TLS client.tls_set(tls_version=mqtt.client.ssl.PROTOCOL_TLS)
#
# Copyright 2021 HiveMQ GmbH
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
import time
import paho.mqtt.client as paho
from paho import mqtt

# setting callbacks for different events to see if it works, print the message etc.
def on_connect(client, userdata, flags, rc, properties=None):
    print("client connected with the cloud with  %s." % rc)

# with this callback you can see if your publish was successful
def on_publish(client, userdata, Message_ID, properties=None):
    print("Message_ID: " + str(Message_ID))

# print which topic was subscribed to
def on_subscribe(client, userdata, Message_ID,  quality_of_service, properties=None):
    print("Subscribed: " + str(Message_ID) + " " + str(quality_of_service))

# print message, useful for checking if it was successful
def on_message(client, userdata, msg):
    print(msg.topic + " " + str(msg.qos) + " " + str(msg.payload))
    
    if msg.payload == "hot":
       print("Received message #1, do something")
       # Do something


    if msg.payload == "another_message":
       print("Received message #2, do something else")
       # Do something else
    

# using MQTT version 5 here, for 3.1.1: MQTTv311, 3.1: MQTTv31
# userdata is user defined data of any type, updated by user_data_set()
# client_id is the given name of the client
client = paho.Client(client_id="", userdata=None, protocol=paho.MQTTv5)
client.on_connect = on_connect

# enable TLS for secure connection
client.tls_set(tls_version=mqtt.client.ssl.PROTOCOL_TLS)
# set username and password
client.username_pw_set("yasmen", "Yas12345")
# connect to HiveMQ Cloud on port 8883 (default for MQTT)
client.connect("7b523ff57b624c40bec79f5c35f6bffa.s2.eu.hivemq.cloud", 8883)

# setting callbacks, use separate functions like above for better visibility
client.on_subscribe = on_subscribe
client.on_message = on_message
client.on_publish = on_publish

# subscribe to all topics of encyclopedia by using the wildcard "#"
client.subscribe("encyclopedia/#", qos=1)


# a single publish, this can also be done in loops, etc.
client.publish("encyclopedia/temperature", payload="hot", qos=1)
client.publish("encyclopedia/humidity", payload="another_message", qos=1)

# loop_forever for simplicity, here you need to stop the loop manually
# you can also use loop_start and loop_stop
client.loop_forever()