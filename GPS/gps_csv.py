import folium
import csv
import serial
import time
import string
import pynmea2
from time import sleep

while True:
    port = "/dev/ttyAMA0"
    ser = serial.Serial(port, baudrate=9600, timeout=0.5)
    dataout = pynmea2.NMEAStreamReader()

    try:
        while True:
            # Read longitude and latitude from the sensor
            newdata = ser.readline()

            if newdata[0:6] == b'$GPRMC':
                newmsg = pynmea2.parse(newdata.decode('utf-8'))
                lat = newmsg.latitude
                lng = newmsg.longitude
                gps = f"Latitude={lat} and Longitude={lng}"
                print(gps)
                break  # Exit the inner loop after the first valid GPS data

    except KeyboardInterrupt:
        ser.close()

    # Data to append
    data_to_append = [lat, lng]

    # CSV file to append to
    csv_file = "gps_readings.csv"

    # Open the CSV file in append mode
    with open(csv_file, mode='a', newline='\n') as file:
        writer = csv.writer(file)

        # Write the data to the CSV file
        writer.writerow(data_to_append)

    # Create a map centered around the first GPS reading
    m = folium.Map(location=[lat, lng], zoom_start=15)

    # Add markers for each GPS reading
    with open(csv_file, mode='r') as file:
        reader = csv.reader(file)
        next(reader)  # Skip the header row
        for row in reader:
            latitude, longitude = map(float, row)
            folium.Marker(location=[latitude, longitude]).add_to(m)

    # Add a line connecting the GPS readings
    with open(csv_file, mode='r') as file:
        reader = csv.reader(file)
        next(reader)  # Skip the header row
        coordinates = [(float(row[0]), float(row[1])) for row in reader]
        folium.PolyLine(locations=coordinates, color='blue').add_to(m)

    # Save the map to an HTML file
    m.save('gps_map.html')
    sleep(1)


