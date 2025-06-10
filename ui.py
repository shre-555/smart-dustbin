import tkinter as tk
from tkinter import ttk
import serial
import time

# Serial port setup (change "COM3" to the correct port for your Arduino)
SERIAL_PORT = "COM4"  # Example: "COM4" on Windows, "/dev/ttyUSB0" on Linux
BAUD_RATE = 9600

try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)  # Give the connection a moment to start
except serial.SerialException:
    ser = None
    print("Error: Could not connect to Arduino. Check the port.")

# GUI Setup
root = tk.Tk()
root.title("Smart Dustbin Fill Level")
root.geometry("400x250")

# Label
title_label = tk.Label(root, text="Dustbin Fill Level", font=("Arial", 16))
title_label.pack(pady=10)

# Progress Bar
progress = ttk.Progressbar(root, orient="horizontal", length=300, mode="determinate")
progress.pack(pady=20)

# Status Label
status_label = tk.Label(root, text="Waiting for data...", font=("Arial", 12))
status_label.pack()

def update_fill_level():
    if ser:
        ser.write(b'\n')  # Request data
        line = ser.readline().decode('utf-8').strip()
        if line.startswith("Distance:"):
            try:
                distance = int(line.split(":")[1])
                max_distance = 30  # Assume bin is empty at 30cm, full at 0cm
                fill_percentage = max(0, min(100, 100 - (distance / max_distance) * 100))
                progress["value"] = fill_percentage
                
                if fill_percentage >= 80:
                    status_label.config(text=f"Warning! Bin is {int(fill_percentage)}% full", fg="red")
                else:
                    status_label.config(text=f"Bin is {int(fill_percentage)}% full", fg="green")
            except ValueError:
                pass  # Ignore incorrect data
    root.after(1000, update_fill_level)  # Update every second

# Start updating the fill level# Start updating the fill level\update_fill_level()

# Run GUI
root.mainloop()

# Close serial connection on exit
if ser:
    ser.close()