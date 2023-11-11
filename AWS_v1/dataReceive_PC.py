from random import randrange,uniform
import time
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient

def DispTemp(self,params,packet):
		print("Temperature: "+str(packet.payload))
def DispFuelLevel(self,params,packet):
		print("Fuel Level: "+str(packet.payload))
def DispTime(self,params,packet):
		print("Elapised Time: "+str(packet.payload))
		print("\n")
		
AWShost = "a38ng4g7lgwyr7-ats.iot.eu-north-1.amazonaws.com"
AWSport = 8883
rootCaPath = "/home/mostafa/Desktop/AWS_GP/AmazonRootCA1.pem"
certPath = "/home/mostafa/Desktop/AWS_GP/e1d57696d99123be86537cabd0ceb0793150662e77ad1a7796822f3652a9ebda-private.pem.key"
keyPath = "/home/mostafa/Desktop/AWS_GP/e1d57696d99123be86537cabd0ceb0793150662e77ad1a7796822f3652a9ebda-certificate.pem.crt"	
		
myMQTTClient = AWSIoTMQTTClient("ItiGpIDSubscriber") #Creating an Object 
myMQTTClient.configureEndpoint(AWShost, AWSport)#Providing End point from AWS URL
myMQTTClient.configureCredentials(rootCaPath,certPath,keyPath) #Providing Ceritficates and private keys
myMQTTClient.configureOfflinePublishQueueing(-1) # Infinite offline Publish queueing
myMQTTClient.configureDrainingFrequency(2) # Draining: 2 Hz
myMQTTClient.configureConnectDisconnectTimeout(10) # Configure connect/disconnect timeout
myMQTTClient.configureMQTTOperationTimeout(5) # MQTT operation timeout

print ('Initiating Realtime Data Receive From Gateway Raspberry Pi...')
myMQTTClient.connect() #connect to the broker 


while True:
	myMQTTClient.subscribe("RealTimeDataTransfer/Temperature",1,DispTemp)
	myMQTTClient.subscribe("RealTimeDataTransfer/Fuellevel",1,DispFuelLevel)
	myMQTTClient.subscribe("RealTimeDataTransfer/Time",1,DispTime)
	time.sleep(5)
