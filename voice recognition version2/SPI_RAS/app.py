# Import necessary modules and libraries
import RPi.GPIO as GPIO
import speech_recognition as sr 
import spi_communication as spi
import time

# Define the GPIO pin for the push button
BUTTON_GPIO = 16 

# Initialize the Speech Recognition module
r = sr.Recognizer()
mic = sr.Microphone()

# Function to recognize speech using the microphone
def recognize_speech():
    with mic as source:
        try:
            # Listen for audio input and recognize it using Google's speech recognition
            audio = r.listen(source, timeout = 5.0)
            words = r.recognize_google(audio)
            print(words)
            return words
        except:
            print("No Answer")
            return None

if __name__== '__main__':
    # Set the GPIO mode and configure the push button pin
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(BUTTON_GPIO, GPIO.IN, pull_up_down=GPIO.PUD_UP)

# Main loop  
while True:
    if not GPIO.input(BUTTON_GPIO):
        print("Start Talking")
        words = recognize_speech() # Start listening for speech 
    else:
        words = None
        
    # If speech is recognized, send the corresponding SPI command
    if words != None:
        if words == "open system":
            spi.send_spi_command(0x5)
        elif words == "shut down system":
            spi.send_spi_command(0x6)
        elif words == "AC open":
            spi.send_spi_command(0x4)
        elif words == "AC stop":
            spi.send_spi_command(0x1)
        elif words == "motor open":
            spi.send_spi_command(0x2)
        elif words == "motor stop":
            spi.send_spi_command(0x3)
        else:
            print("Wrong input try again")
    
 # End of the code