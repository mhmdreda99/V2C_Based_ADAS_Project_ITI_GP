import serial 
import speech_recognition as sr

ser = serial.Serial ("/dev/ttyS0", 9600)
r = sr.Recognizer()
mic = sr.Microphone()

while True:
    print("Start Talking")
    
    with mic as source:
        audio = r.listen(source)
    words = r.recognize_google(audio)
    print(words)
    
    if words == "on":
        ser.write(bytes("O", 'utf-8'))
        
    if words == "off":
        ser.write (bytes("F", 'utf-8'))