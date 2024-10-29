# IoT_RideSync

**Bike Ride Logger with IoT Integration**

## Project Overview
IoT_RideSync is an IoT-based bike ride logging system that collects, stores, and visualizes key ride data. Designed with low-power consumption in mind, the system monitors parameters such as GPS location, acceleration, and suspension motion, and transmits this data to a smartphone app for real-time display and cloud-based analysis.

## Key Features
- **Energy Harvesting (Future Work):** The project explores thermoelectric generator (TEG) energy harvesting as a future development for powering sensors sustainably.
  
- **Comprehensive Data Collection:**
  - **GPS Coordinates**
  - **Acceleration**
  - **Suspension Motion**
  - **Environmental Parameters** (using the Arduino Nano 33 BLE Sense Rev2)

- **Low-Power Communication:** Uses Bluetooth Low Energy (BLE) to send data to a paired smartphone.

- **Real-Time Data Visualization:**
  - The RideSyncIoT app (built using MIT App Inventor) displays live sensor data and logs ride information.
  - Data is uploaded to a cloud server via MQTT for long-term storage and visualization.

## Getting Started

### Hardware Requirements
- **Arduino Nano 33 BLE Sense Rev2**
- **Raspberry Pi 4** (for data handling and analytics)
- **Android Smartphone** (Android 11 or lower recommended)
  - Tested devices: OnePlus 3T (Android 9) and OnePlus 7 Pro (Android 11)

### Installation

1. **Arduino Setup**
   - Download and install the [Arduino IDE](https://www.arduino.cc/en/software).
   - Clone the [Arduino code from this repository](https://github.com/TheBepps/IoT_RideSync/tree/main/Arduino%20Code).
   - Open the sketch in Arduino IDE, install any required libraries if prompted, and upload the code to the Arduino Nano 33 BLE Sense Rev2.

2. **Smartphone Setup**
   - Download the RideSyncIoT APK from the [MIT App Inventor directory](https://github.com/TheBepps/IoT_RideSync/tree/main/MIT%20App%20Inventor) and install it on your smartphone.
   - Grant all necessary permissions (e.g., location, Bluetooth, storage).

3. **Raspberry Pi Setup**
   - Follow the [Raspberry Pi setup instructions](https://github.com/TheBepps/IoT_RideSync/blob/main/Raspberry_installation.md) to install Grafana and InfluxDB.
   - Verify connectivity to ensure the Raspberry Pi can receive data from the RideSyncIoT app for visualization.

## System Architecture
The RideSyncIoT system operates as follows:

1. **Data Collection:** Sensors on the Arduino gather data on GPS location, acceleration, environmental conditions, and suspension movement.
2. **Data Transmission:** Collected data is sent via BLE to the RideSyncIoT smartphone app.
3. **Data Storage and Visualization:** The app displays real-time data and uploads it to a Raspberry Pi server via MQTT. Data is stored in InfluxDB and visualized with Grafana for in-depth analysis.

## Usage

1. **Launch the RideSyncIoT app** on your smartphone and ensure Bluetooth is enabled.
2. **Pair the Arduino device** with the app to start receiving sensor data.
3. **Monitor real-time data** on the app, including:
   - GPS coordinates
   - Speed and distance
   - Environmental data and acceleration
4. **View ride history and analytics** through Grafana on the Raspberry Pi.

## Future Improvements
- **Enhanced Energy Harvesting:** Implement TEG-based energy harvesting to power the system sustainably.
- **Expanded Connectivity:** Extend system capabilities to include IoT-based bike-to-bike communication.
- **User Interface Enhancements:** Further development of the RideSyncIoT app for more intuitive data display and interaction.
- **Additional Sensor Support:** Integrate more environmental sensors for enriched data collection.

## Acknowledgments
This project was developed as part of the "Laboratory of Internet of Things" course. Special thanks to the development community for providing essential libraries and tools that enable IoT innovation.

---

**Get Started Now:** Clone the repository, follow the setup steps, and experience next-level bike tracking and data analytics with IoT_RideSync!
