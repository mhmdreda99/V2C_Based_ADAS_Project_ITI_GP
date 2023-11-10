from tkinter import *
import RPi.GPIO as GPIO
from time import sleep
import serial


def read_UART(pinNum):
    ser.reset_input_buffer();
    ##reciving data from uart
    recived_data = ser.read(size = 9)
    ##converting data from byte to list
    recived_data = list(recived_data)
    #recived_data.reverse()
    #returning data from ASCI to string
    for i in range(len(recived_data)):
        recived_data[i] = str(recived_data[i]-48)
        
    ##distributing values to variables
    fuelLevel = 	recived_data[0]
    bsaDiagnose = 	recived_data[1]
    SmotorTemp = 	"".join(recived_data[2:5])
    motorTemp = int(SmotorTemp)
    
    SrunTime = 		"".join(recived_data[5:9])
    runTime = int(SrunTime)
    
    print("Fuel level: " , fuelLevel)
    print("BSA case: " , bsaDiagnose)
    print("Motor temprature: " , motorTemp)
    print("Time sence start: " , runTime)
   
    ser.reset_input_buffer();
    
    
    


GPIO.setmode(GPIO.BCM)

GPIO.setup(2, GPIO.OUT) # LED pin set as output
GPIO.output(2,GPIO.LOW)

GPIO.setup(3,GPIO.IN,pull_up_down=GPIO.PUD_UP)
GPIO.add_event_detect(3, GPIO.RISING, callback=read_UART,bouncetime = 100)  

ser = serial.Serial ("/dev/ttyS0",9600)


while(1):
    pass
