import socket
import csv
import os
from datetime import datetime

# UDP setup
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(('0.0.0.0', 3333))  # listen on all interfaces, port 3333

# File name
filename = "sensor.csv"

# Check if file exists
file_exists = os.path.isfile(filename)

# Open CSV file for appending
with open(filename, mode="a", newline="") as file:
    writer = csv.writer(file)

    # Write header if file is new
    if not file_exists:
        writer.writerow(["date", "time", "temperature", "humidity", "tvoc", "eco2", "mq2", "mq7", "mq9", "mq135"])

    print("Listening for ESP32 sensor data...")

    while True:
        # Receive data from ESP32
        data, addr = s.recvfrom(1024)
        decoded_data = data.decode("utf-8").strip()
        print(f"Received: {decoded_data}")

        # Split values (ESP32 must send comma-separated)
        values = decoded_data.split(",")

        # Add current date & time
        current_date = datetime.now().strftime("%Y-%m-%d")
        current_time = datetime.now().strftime("%H:%M:%S")

        # Save row to CSV
        writer.writerow([current_date, current_time] + values)
        file.flush()  # make sure data is written immediately
