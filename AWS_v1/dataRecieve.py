from random import randrange,uniform
import time
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient

def DispTemp(self,params,packet):
		print('Topic: '+ (packet.topic))
		print("Payload: "+str(packet.payload))
		
myMQTTClient = AWSIoTMQTTClient("ItiGpIDSubscriber") #Creating an Object 
myMQTTClient.configureEndpoint("a38ng4g7lgwyr7-ats.iot.eu-north-1.amazonaws.com", 8883)#Providing End point from AWS URL
#Providing Ceritficates and private keys
myMQTTClient.configureCredentials("/home/abdelhamid/Desktop/AWS_GP/AmazonRootCA1.pem", "/home/abdelhamid/Desktop/AWS_GP/e1d57696d99123be86537cabd0ceb0793150662e77ad1a7796822f3652a9ebda-private.pem.key", "/home/abdelhamid/Desktop/AWS_GP/e1d57696d99123be86537cabd0ceb0793150662e77ad1a7796822f3652a9ebda-certificate.pem.crt")

myMQTTClient.configureOfflinePublishQueueing(-1) # Infinite offline Publish queueing
myMQTTClient.configureDrainingFrequency(2) # Draining: 2 Hz
myMQTTClient.configureConnectDisconnectTimeout(10) # 10 sec
myMQTTClient.configureMQTTOperationTimeout(5) # 5 sec
print ('Initiating Realtime Data Receive From Gateway Raspberry Pi...')
myMQTTClient.connect()
myMQTTClient.subscribe("RealTimeDataTrasfer/Temperature",1,DispTemp)

while True:
	time.sleep(5)
