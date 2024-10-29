
# Installation Guide for InfluxDB, Telegraf, and Grafana

This guide provides steps to install InfluxDB, Telegraf, and Grafana on a Raspberry Pi and configure them to run at startup.

## Prerequisites
- Raspberry Pi running Raspberry Pi OS (preferably a recent version).
- Internet connection.

## Step 1: Update Your System
```bash
sudo apt update && sudo apt upgrade -y
```

## Step 2: Install InfluxDB
```bash
wget -qO - https://repos.influxdata.com/influxdb.key | sudo apt-key add -
echo "deb https://repos.influxdata.com/debian bullseye stable" | sudo tee /etc/apt/sources.list.d/influxdb.list
udo apt updatesudo apt install influxdb -ysudo systemctl start influxdb
sudo systemctl enable influxdb
```

## Step 3: Install Telegraf
```bash
wget -qO - https://repos.influxdata.com/influxdb.key | sudo apt-key add -
echo "deb https://repos.influxdata.com/debian bullseye stable" | sudo tee /etc/apt/sources.list.d/influxdb.list
sudo apt update
sudo apt install telegraf -y
sudo systemctl start telegraf
sudo systemctl enable telegraf
```

## Step 4: Install Grafana
```bash
wget -q -O - https://packages.grafana.com/gpg.key | sudo apt-key add -
echo "deb https://packages.grafana.com/oss/deb stable main" | sudo tee /etc/apt/sources.list.d/grafana.list
sudo apt update
sudo apt install grafana -y
sudo systemctl start grafana-server
sudo systemctl enable grafana-server
```

## Step 5: Configure InfluxDB and Telegraf
1. Access InfluxDB: Open your web browser and go to `http://<YOUR_RASPBERRY_PI_IP>:8086`. 
   - To find your Raspberry Pi's IP, you can run `hostname -I` in the terminal.
2. Set your Raspberry Pi's IP to be static (in my case 192.168.1.96) using your router's DHCP settings. (Refer to your router's documentation for specific steps.)
3. Complete the **Initial user setup** in InfluxDB and save your **<YOUR_INFLUX_TOKEN>**.
4. Create a Telegraf configuration:
   - Navigate to **Load data** > **Telegraf** > **Create Configuration**.
   - Choose a bucket and select **MQTT** as the data source.
   - Replace the default configuration with the project's configuration file located at `IoT_RideSync\InfluxDB\GPS_MQTT_tag\gps_mqtt_tag.conf`.
   - Save the resulting **<SETUP_INSTRUCTION>** configuration.
5. Save the Telegraf configuration; on Rpi terminal:
   ```bash
   sudo nano /etc/systemd/system/telegraf.service
   ```
   Replace the content with:
   ```ini
   [Unit]
   Description=Telegraf
   After=network.target

   [Service]
   Environment="INFLUX_TOKEN=<YOUR_INFLUX_TOKEN>"
   ExecStart=/usr/bin/telegraf --config http://||||<SETUP_INSTRUCTION>(command to start the Telegraf agent running on your machine)||||
   Restart=on-failure

   [Install]
   WantedBy=multi-user.target
   ```
6. Restart Telegraf:
   ```bash
   sudo systemctl daemon-reload
   sudo systemctl restart telegraf
   ```

## Step 6: Configure Grafana
1. Access Grafana: Open your web browser and go to `http://<YOUR_RASPBERRY_PI_IP>:3000`.
2. Default login: Username: `admin`, Password: `admin` (change the password upon first login).
3. Add InfluxDB as a Data Source:
   - Go to **Connections** (menu) > **Data Sources** > **Add data source**.
   - Select **InfluxDB** and configure it according to your setup.
4. Import RideSync dashboard:
   - Go to **Dashboard** > **New** > **Import** > upload the project dashboard (`IoT_RideSync\Grafana\GPS_MQTT_magnetometer.json`) > Select InfluxDB as datasource.

## Step 7: Validate Installation
```bash
sudo systemctl status influxdb
sudo systemctl status telegraf
sudo systemctl status grafana-server
```
Ensure all services are running without errors.

## Conclusion
You have successfully installed and configured InfluxDB, Telegraf, and Grafana on your Raspberry Pi. All three services are set to run at startup. You can now start collecting and visualizing your data!
