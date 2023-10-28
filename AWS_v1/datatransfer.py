from random import randrange,uniform
import time
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient

AWShost = "a38ng4g7lgwyr7-ats.iot.eu-north-1.amazonaws.com"
AWSport = 8883
rootCaPath = "/home/abdelhamid/Desktop/AWS_GP/AmazonRootCA1.pem"
certPath = "/home/abdelhamid/Desktop/AWS_GP/e1d57696d99123be86537cabd0ceb0793150662e77ad1a7796822f3652a9ebda-private.pem.key"
keyPath = "/home/abdelhamid/Desktop/AWS_GP/e1d57696d99123be86537cabd0ceb0793150662e77ad1a7796822f3652a9ebda-certificate.pem.crt"


myMQTTClient = AWSIoTMQTTClient("ItiGpIDPublisher")
myMQTTClient.configureEndpoint(AWShost, AWSport)
myMQTTClient.configureCredentials(rootCaPath,certPath, keyPath)
myMQTTClient.configureOfflinePublishQueueing(-1) # Infinite offline Publish queueing
myMQTTClient.configureDrainingFrequency(2) # Draining: 2 Hz
myMQTTClient.configureConnectDisconnectTimeout(10) # 10 sec
myMQTTClient.configureMQTTOperationTimeout(5) # 5 sec

print ('Initiating Realtime Data Transfer From Gateway Raspberry Pi...')
myMQTTClient.connect()


while True:
    Temp = int(uniform(20,26))
    print("publish from Rasp.."+str(Temp))
    myMQTTClient.publish(
       topic="RealTimeDataTransfer/Temperature",
       QoS=1,
       payload='{"Temperature:"'+str(Temp)+'"}')
    FuelLevel = int(uniform(0,100))
    print("publish from Rasp.."+str(FuelLevel))
    myMQTTClient.publish(
       topic="RealTimeDataTransfer/Fuellevel",
       QoS=1,
       payload='{"Fuel Level:"'+str(FuelLevel)+'"}')
    Time = int(uniform(500,1000))
    print("publish from Rasp.."+str(Time))
    myMQTTClient.publish(
       topic="RealTimeDataTransfer/Time",
       QoS=1,
       payload='{"Elapsed Time:"'+str(Time)+'"}')   
    
    time.sleep(2)

