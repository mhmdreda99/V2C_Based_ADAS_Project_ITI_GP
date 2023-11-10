import RPi.GPIO as GPIO
import serial
import time
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient
import json
import requests

##config gpio pins
def configGPIO() :
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(3,GPIO.IN,pull_up_down=GPIO.PUD_UP)
    GPIO.add_event_detect(3, GPIO.RISING, callback=read_UART,bouncetime = 100)  

##UART dummy callback 
def read_UART(pinNum):
    global dataCount
    ser.reset_input_buffer();
    ##reciving data from uart
    recived_data = ser.read(size = 9)
    ##converting data from byte to list
    recived_data = list(recived_data)
    #returning data from ASCI to string
    for i in range(len(recived_data)):
        recived_data[i] = str(recived_data[i]-48)

    ##distributing values to variables
    fuelLevel = 	int(recived_data[0])
    bsaDiagnose = 	int(recived_data[1])
    SmotorTemp = 	"".join(recived_data[2:5])
    motorTemp = int(SmotorTemp)
    SrunTime = 		"".join(recived_data[5:9])
    runTime = int(SrunTime)
   
    ser.reset_input_buffer();
    if motorCase == 1:
        print("Fuel level		: " , fuelLevel)
        print("BSA case		: " , bsaDiagnose)
        print("Motor temprature	: " , motorTemp)
        print("Time sence start	: " , runTime)
        print("Data Cout		: " , dataCount)
        publishToAWS(fuelLevel,bsaDiagnose,motorTemp,runTime)
        print("puplished to cloud")
        print (" ")  
       

    


##sending data to AWS
def publishToAWS(FuelLevel,BSAstate,Temperature,Elapsedtime):
    global myMQTTClient
    global dataCount;
    myMQTTClient.publish(
    topic="RealTime/Data",
    QoS=1,
    payload=json.dumps({
    'Temperature': Temperature,
    'FuelLevel': FuelLevel,
    'Elapsedtime': Elapsedtime,
    'BSA_state':BSAstate,
    'No':dataCount
    })
    )  
    dataCountFile = open("dataCountFile.txt","w")
    dataCount +=1 
    dataCountFile.write(str(dataCount))
    dataCountFile.close
    #print (type(Temperature))  
    #print (type(FuelLevel))  
    #print (type(Elapsedtime))  
    #print (type(BSAstate))  



## main
configGPIO()
motorCase = 0
#dataCount 
dataCountFile = open("dataCountFile.txt","r")
dataCount = int(dataCountFile.read())
print("dataCount is:", dataCount)
dataCountFile.close

#config UART
ser = serial.Serial ("/dev/ttyS0",9600)

#config WAS , MQTT
AWShost = "a38ng4g7lgwyr7-ats.iot.eu-west-1.amazonaws.com"
AWSport = 8883
rootCaPath = "/home/abdelhamid/Projects/AWS_FileV1.1/AmazonRootCA1.pem"
certPath = "/home/abdelhamid/Projects/AWS_FileV1.1/2e196845390830820aff33422c4955748da5d82bc90ab280c351c6b541bcf1fa-private.pem.key"
keyPath = "/home/abdelhamid/Projects/AWS_FileV1.1/2e196845390830820aff33422c4955748da5d82bc90ab280c351c6b541bcf1fa-certificate.pem.crt"

myMQTTClient = AWSIoTMQTTClient("ItiGpIDPublisher")
myMQTTClient.configureEndpoint(AWShost, AWSport)
myMQTTClient.configureCredentials(rootCaPath,certPath, keyPath)
myMQTTClient.configureOfflinePublishQueueing(-1) # Infinite offline Publish queueing
myMQTTClient.configureDrainingFrequency(2) # Draining: 2 Hz
myMQTTClient.configureConnectDisconnectTimeout(10) # 10 sec
myMQTTClient.configureMQTTOperationTimeout(5) # 5 sec
print ('Initiating Realtime Data Transfer From Gateway Raspberry Pi...')
myMQTTClient.connect()



while(1):
    
    words = requests.get("https://VoiceRRecognition.pythonanywhere.com") 

    if ((words.text == "b\'1\'") and (motorCase == 0)): 
        # Open engine motor
        print("Start Engine")
        motorCase = 1
        
    elif ((words.text == "b\'0\'") and (motorCase == 1)):
        # Stop engine motor
        print("Stop Engine")
        motorCase = 0
    


