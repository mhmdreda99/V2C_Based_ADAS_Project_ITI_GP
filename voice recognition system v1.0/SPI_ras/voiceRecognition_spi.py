import time 
import spidev
import speech_recognition as sr 

#speech Recognition Initialization
r = sr.Recognizer()
mic = sr.Microphone()

#Spi Initialization
bus = 0
device = 1
spi = spidev.SpiDev()
spi.open(bus, device)
spi.max_speed_hz = 500000
spi.mode = 0

while True:
    print("Start Talking")
    
    with mic as source:
        audio = r.listen(source)
    words = r.recognize_google(audio)
    print(words)
    
    if words == "open system":
        spi.xfer2([0x5])
       
    elif words == "shut down system":
        spi.xfer2([0x6])
        
    elif words == "open AC":
        spi.xfer2([0x4])
        
    elif words == "stop AC":
        spi.xfer2([0x1])
        
    elif words == "open motor":
        spi.xfer2([0x2])
        
    elif words == "stop motor":
        spi.xfer2([0x3])
    