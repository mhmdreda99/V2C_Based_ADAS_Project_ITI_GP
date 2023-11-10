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
    print("Fuel level: " , fuelLevel)
    print("BSA case: " , bsaDiagnose)
    print("Motor temprature: " , motorTemp)
    print("Time sence start: " , runTime)
    ser.reset_input_buffer();
    if motorCase == 1:
        publishToAWS(fuelLevel,bsaDiagnose,motorTemp,runTime)
        print("puplished to cloud")
    else :
        print("not puplished to cloud");
    print (" ")  

#defining AWS credincials
def setAWS_credicials ():
    AWShost = "a38ng4g7lgwyr7-ats.iot.eu-north-1.amazonaws.com"
    AWSport = 8883
    rootCaPath = "/home/abdelhamid/Desktop/AWS_GP/AmazonRootCA1.pem"
    certPath = "/home/abdelhamid/Desktop/AWS_GP/e1d57696d99123be86537cabd0ceb0793150662e77ad1a7796822f3652a9ebda-private.pem.key"
    keyPath = "/home/abdelhamid/Desktop/AWS_GP/e1d57696d99123be86537cabd0ceb0793150662e77ad1a7796822f3652a9ebda-certificate.pem.crt"

    
def configMQTT () :
    global myMQTTClient
    myMQTTClient.configureEndpoint(AWShost, AWSport)
    myMQTTClient.configureCredentials(rootCaPath,certPath, keyPath)
    myMQTTClient.configureOfflinePublishQueueing(-1) # Infinite offline Publish queueing
    myMQTTClient.configureDrainingFrequency(2) # Draining: 2 Hz
    myMQTTClient.configureConnectDisconnectTimeout(10) # 10 sec
    myMQTTClient.configureMQTTOperationTimeout(5) # 5 sec
    print ('Initiating Realtime Data Transfer From Gateway Raspberry Pi...')
    myMQTTClient.connect()



##sending data to AWS
def publishToAWS(FuelLevel,BSAstate,Temperature,Elapsedtime):
    global myMQTTClient
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
    #print (type(Temperature))  
    #print (type(FuelLevel))  
    #print (type(Elapsedtime))  
    #print (type(BSAstate))  



## main
configGPIO()
ser = serial.Serial ("/dev/ttyS0",9600)
motorCase = 0
myMQTTClient = AWSIoTMQTTClient("ItiGpIDPublisher")
#setAWS_credicials()
#configMQTT()
#publishToAWS()  ##will be called in UART ISR


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
    


