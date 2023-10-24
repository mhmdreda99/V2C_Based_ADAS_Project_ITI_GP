from tkinter import *
import RPi.GPIO as GPIO
from time import sleep
import serial


def read_UART(pinNum):
    ser.reset_input_buffer();
    ##reciving data from uart
    recived_data = ser.read(size = 10)
    ###checking data 
    print(recived_data)
    ser.reset_input_buffer();
    
    
    


GPIO.setmode(GPIO.BCM)

GPIO.setup(2, GPIO.OUT) # LED pin set as output
GPIO.output(2,GPIO.LOW)

GPIO.setup(3,GPIO.IN,pull_up_down=GPIO.PUD_UP)
GPIO.add_event_detect(3, GPIO.RISING, callback=read_UART,bouncetime = 100)  

ser = serial.Serial ("/dev/ttyS0",9600)


while(1):
    pass
