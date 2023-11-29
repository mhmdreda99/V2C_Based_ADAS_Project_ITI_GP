import RPi.GPIO as GPIO
import speech_recognition as sr
from gpiozero import AngularServo
import vlc
from time import sleep

GPIO.setwarnings(False)

# GPIO pin definitions
in1 = 17
in2 = 27
en_a = 4

in3 = 5
in4 = 6
en_b = 26

# To use Bradcom GPIO numbers instead of using board pi numbers
GPIO.setmode(GPIO.BCM)

# Setup the output pins
GPIO.setup(in1, GPIO.OUT)
GPIO.setup(in2, GPIO.OUT)
GPIO.setup(en_a, GPIO.OUT)

GPIO.setup(in3, GPIO.OUT)
GPIO.setup(in4, GPIO.OUT)
GPIO.setup(en_b, GPIO.OUT)

# Initialize PWM signals for motor control
power_a = GPIO.PWM(en_a, 100)
power_a.start(50)

power_b = GPIO.PWM(en_b, 100)
power_b.start(50)

# Initialize motors to off state
GPIO.output(in1, GPIO.LOW)
GPIO.output(in2, GPIO.LOW)

GPIO.output(in3, GPIO.LOW)
GPIO.output(in4, GPIO.LOW)

# Initialize servo motor
servo = AngularServo(18, min_pulse_width=0.0006, max_pulse_width=0.0023)

# Initialize VLC media player
p = vlc.MediaPlayer("quran.mp4a",":no- video")

# Initialize speech recognition engine
r = sr.Recognizer()
mic = sr.Microphone()

# Print greeting message
print("hello")

while (True):
    # Listen for audio input from microphone
    with mic as source:
        audio = r.listen(source)
    # Recognize speech from audio input
    words = r.recognize_google(audio)
    print(words)

    if words == "start engine":
        # Open engine motor
        GPIO.output(in1, GPIO.HIGH)
        GPIO.output(in2, GPIO.LOW)
        
    elif words == "stop engine":
        # Stop engine motor
        GPIO.output(in1, GPIO.LOW)
        GPIO.output(in2, GPIO.LOW)

    elif words == "open AC":
        # Open AC motor
        GPIO.output(in3, GPIO.HIGH)
        GPIO.output(in4, GPIO.LOW)
        
    elif words == "stop AC":
        # Stop AC motor 
        GPIO.output(in3, GPIO.LOW)
        GPIO.output(in4, GPIO.LOW)
    
    elif words == "turn on wiper":
        # Move servo motor to swing wiper arm back and forth
        servo.angle = 90
        sleep(2)
        servo.angle = -90
        sleep(2)
        servo.angle = 90
        sleep(2)
        servo.angle = -90
        
    elif words == "play audio":
        # Play audio playback
        p.play()

    elif words == "stop audio":
        # Pause audio playback
        p.pause()

    elif words == "exit":
        # Print goodbye message
        print("...")
        sleep(1)
        print("...")
        sleep(1)
        print("...")
        sleep(1)
        print("Goodbye")
        break
   
    
    
