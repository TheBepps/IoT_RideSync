# RideSyncIoT Project Guide

This guide walks through the hardware setup and software installation steps to get the RideSyncIoT system up and running.

## Required Hardware

1. **Arduino Nano 33 BLE Sense Rev2**
2. **Raspberry Pi 4** (for data handling and analytics)
3. **Android Smartphone** (Preferably running Android 11 or lower for compatibility)
   - The RideSyncIoT app has been tested on:
      - OnePlus 3T (Android 9)
      - OnePlus 7 Pro (Android 11)

## Step-by-Step Installation

### 1. Arduino Setup

1. Download and install the [Arduino IDE](https://www.arduino.cc/en/software) if you haven’t already.
2. Upload the latest version of the Arduino sketch:
   - Go to the [RideSync Arduino Code repository](https://github.com/TheBepps/IoT_RideSync/tree/main/Arduino%20Code).
   - Open the Arduino sketch file in the Arduino IDE.
3. Install any required libraries if prompted in the IDE. This may include libraries for BLE, sensors, or MQTT communication.

### 2. Smartphone Setup

1. Install the **RideSyncIoT app** on your Android smartphone:
   - Download the APK from the [RideSyncIoT app repository](https://github.com/TheBepps/IoT_RideSync/tree/main/MIT%20App%20Inventor).
   - Install the APK and allow all necessary permissions for the app (e.g., location, Bluetooth, storage).
2. Launch the RideSyncIoT app with GPS and bluetooth enabled, and ensure that it can connect to the Arduino and send data.

### 3. Raspberry Pi Setup

1. Follow the [Raspberry Pi setup instructions](https://github.com/TheBepps/IoT_RideSync/blob/main/Raspberry_installation.md) to configure:
   - **Grafana** (for data visualization)
   - **InfluxDB** (for data storage and time-series analytics)
2. Once setup is complete, confirm that the Raspberry Pi can communicate with the RideSyncIoT app and receive data for visualization in Grafana.

---

This setup guide provides the essentials for getting RideSyncIoT running smoothly. Once configured, you’ll be ready to begin tracking and analyzing bike ride data in real-time!
