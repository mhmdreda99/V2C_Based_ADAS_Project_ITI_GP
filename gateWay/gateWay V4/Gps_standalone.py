import csv
import serial
import time
import string
import pynmea2
from time import sleep

port = '/dev/ttyUSB0'
ser = serial.Serial(port,baudrate = 9600,timeout=0.5)
while True :
    dataout = pynmea2.NMEAStreamReader()
    
    try:
            while True:
                    #Read Latitdue and Longitude from sensor
                    newdata = ser.readline()
                    if newdata[0:6] == b'$GPRMC':
                            newmsg = pynmea2.parse(newdata.decode('utf-8'))
                            lat = newmsg.latitude
                            print(lat)
                            lng = newmsg.longitude
                            gps = f"Latitude={lat} and Longitude={lng}"
                            print(gps)
                            break #Exit the inner Loop after the first valid GPS data
    except keyboardInterrupt:
            ser.close()
    """        
    data_to_append = [lat,lng] #this data to be send
    
    csv_file = "gps.txt"
    
    with open(csv_file,mode = 'a',newline='\n') as file:
            writer = csv.writer(file)
            #write the data to CSV file
            writer.writerow(data_to_append)
            """
    gpsLat_File = open("GPS_lat.txt","w")
    gpsLat_File.write(str(lat))
    print("Latitude is:", lat)
    gpsLat_File.close 
    
    gpsLng_File = open("GPS_lng.txt","w")
    gpsLng_File.write(str(lng))
    print("langitude is:", lng)
    gpsLng_File.close       
    sleep(5)
