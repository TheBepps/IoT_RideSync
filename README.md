# IoT Bike Ride Logger with Energy Harvesting [RideSync]
# Project Overview
This project is an IoT-based solution to log data from bike rides, utilizing multiple energy-harvesting techniques and low-power BLE transmission. The system collects data from sensors to monitor GPS location, acceleration, suspension motion, and other key parameters. Data is transmitted to a smartphone and uploaded to a cloud server for storage and visualization.

# Features
Energy Harvesting: Collect energy from multiple sources such as wheel rotation, suspension motion, and thermal gradients (e.g., brake and contact points).
Data Collection: 
 Log data from various sensors, including:
 GPS coordinates
 Acceleration
 Suspension motion
Low Power Communication: Transmit data using Bluetooth Low Energy (BLE) to a smartphone.
Data Visualization: Display collected data on a smartphone app (developed using MIT App Inventor) and upload it to a cloud server via MQTT for further analysis.
Hybrid Energy Strategy: A system that prioritizes energy sources based on availability, using energy-harvesting ICs (e.g., LTC3108).
 
