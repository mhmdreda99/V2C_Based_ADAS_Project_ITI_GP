from random import randrange,uniform
import time
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient
import json
AWShost = "a38ng4g7lgwyr7-ats.iot.eu-west-1.amazonaws.com"
AWSport = 8883
rootCaPath = "/home/abdelhamid/Desktop/AWS_GP/AmazonRootCA1.pem"
certPath = "/home/abdelhamid/Desktop/AWS_GP/2e196845390830820aff33422c4955748da5d82bc90ab280c351c6b541bcf1fa-private.pem.key"
keyPath = "/home/abdelhamid/Desktop/AWS_GP/2e196845390830820aff33422c4955748da5d82bc90ab280c351c6b541bcf1fa-certificate.pem.crt"


myMQTTClient = AWSIoTMQTTClient("ItiGpIDPublisher")
myMQTTClient.configureEndpoint(AWShost, AWSport)
myMQTTClient.configureCredentials(rootCaPath,certPath, keyPath)
myMQTTClient.configureOfflinePublishQueueing(-1) # Infinite offline Publish queueing
myMQTTClient.configureDrainingFrequency(2) # Draining: 2 Hz
myMQTTClient.configureConnectDisconnectTimeout(10) # 10 sec
myMQTTClient.configureMQTTOperationTimeout(5) # 5 sec

print ('Initiating Realtime Data Transfer From Gateway Raspberry Pi...')
myMQTTClient.connect()
BSAstate = 'Error' 

while True:
    Temperature = int(uniform(20,26))
    FuelLevel = int(uniform(0,100))
    Elapsedtime = int(uniform(500,1000))
    print("Raspberry publishing:"+str(Temperature))
    myMQTTClient.publish(
    topic="RealTime/Data",
    QoS=1,
    payload=json.dumps({
    'Temperature': Temperature,
    'FuelLevel': FuelLevel,
    'Elapsedtime': Elapsedtime,
    'BSA_state':BSAstate
    })
    )  

    time.sleep(2)
