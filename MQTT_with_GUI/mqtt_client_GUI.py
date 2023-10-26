import paho.mqtt.client as paho
import tkinter as tk

# Global variables to store messages
received_messages = {"temperature": None, "humidity": None}

def on_connect(client, userdata, flags, rc, properties=None):
    print("Client connected with result code %s." % rc)

def on_message(client, userdata, msg):
    print(msg.topic + " " + str(msg.qos) + " " + str(msg.payload))
    if msg.topic == "encyclopedia/temperature":
        received_messages["temperature"] = msg.payload.decode('utf-8') if msg.payload is not None else ""
    elif msg.topic == "encyclopedia/humidity":
        received_messages["humidity"] = msg.payload.decode('utf-8') if msg.payload is not None else ""
    update_buttons_text()

# Function to update the text of the buttons
def update_buttons_text():
    temperature_button.config(text="Display Temperature Status")
    humidity_button.config(text="Display Humidity Status")

# Function to update the message label when the Temperature button is clicked
def update_temperature():
    message_label.config(text=received_messages["temperature"] if received_messages["temperature"] is not None else "No Data")

# Function to update the message label when the Humidity button is clicked
def update_humidity():
    message_label.config(text=received_messages["humidity"] if received_messages["humidity"] is not None else "No Data")

# Create a client
client = paho.Client(client_id="")
client.on_connect = on_connect
client.on_message = on_message
client.username_pw_set("yasmen", "Yas12345")
client.tls_set()  # Use default TLS settings
client.connect("7b523ff57b624c40bec79f5c35f6bffa.s2.eu.hivemq.cloud", 8883)
client.subscribe("encyclopedia/temperature", qos=1)
client.subscribe("encyclopedia/humidity", qos=1)

# Create the GUI window
window = tk.Tk()
window.title("MQTT Client")
window.geometry("400x200")  # Set window size

# Create buttons to display Temperature and Humidity status
temperature_button = tk.Button(window, text="Display Temperature Status", command=update_temperature, bg="lightblue")
humidity_button = tk.Button(window, text="Display Humidity Status", command=update_humidity, bg="lightblue")
temperature_button.pack(padx=10, pady=5)
humidity_button.pack(padx=10, pady=5)

# Create a label to display the selected message
message_label = tk.Label(window, text="", wraplength=350, bg="lightyellow")
message_label.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

# Start the MQTT client loop in a separate thread
client.loop_start()

# Start the GUI main loop
window.mainloop()
