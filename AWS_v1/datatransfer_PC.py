from random import randrange,uniform
import time
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient

myMQTTClient = AWSIoTMQTTClient("ItiGpIDPublisher")
myMQTTClient.configureEndpoint("a38ng4g7lgwyr7-ats.iot.eu-north-1.amazonaws.com", 8883)

myMQTTClient.configureCredentials("/home/mostafa/Desktop/AWS_GP/AmazonRootCA1.pem", "/home/mostafa/Desktop/AWS_GP/e1d57696d99123be86537cabd0ceb0793150662e77ad1a7796822f3652a9ebda-private.pem.key", "/home/mostafa/Desktop/AWS_GP/e1d57696d99123be86537cabd0ceb0793150662e77ad1a7796822f3652a9ebda-certificate.pem.crt")

myMQTTClient.configureOfflinePublishQueueing(-1) # Infinite offline Publish queueing
myMQTTClient.configureDrainingFrequency(2) # Draining: 2 Hz
myMQTTClient.configureConnectDisconnectTimeout(10) # 10 sec
myMQTTClient.configureMQTTOperationTimeout(5) # 5 sec
print ('Initiating Realtime Data Transfer From PC...')
myMQTTClient.connect()


while True:
    randNumber = uniform(20,26)
    print("publish from Rasp.."+str(randNumber))
    myMQTTClient.publish(
       topic="RealTimeDataTransfer/Temperature",
       QoS=1,
       payload='{"Temperature":"'+str(randNumber)+'"}')
    time.sleep(2)

