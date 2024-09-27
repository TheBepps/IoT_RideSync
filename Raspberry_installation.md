
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
sudo apt update
sudo apt install influxdb -y
sudo systemctl start influxdb
sudo systemctl enable influxdb
```

## Step 3: Install Telegraf
```bash
wget -qO - https://repos.influxdata.com/influxdb.key | sudo apt-key add -
echo "deb https://repos.influxdata.com/debian bullseye stable" | sudo tee /etc/apt/sources.list.d/influxdb.list
sudo apt update
sudo apt install telegraf -y
sudo nano /etc/systemd/system/telegraf.service
```
Replace the content with:
```ini
[Unit]
Description=Telegraf
After=network.target

[Service]
Environment="INFLUX_TOKEN=YOUR_INFLUX_TOKEN"
ExecStart=/usr/bin/telegraf --config /etc/telegraf/telegraf.conf
Restart=on-failure

[Install]
WantedBy=multi-user.target
```
```bash
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

## Step 5: Configure Grafana
1. Access Grafana: Open your web browser and go to `http://<YOUR_RASPBERRY_PI_IP>:3000`.
2. Default login: Username: `admin`, Password: `admin` (change the password upon first login).
3. Add InfluxDB as a Data Source:
   - Go to **Configuration** (gear icon) > **Data Sources** > **Add data source**.
   - Select **InfluxDB** and configure it according to your setup.

## Step 6: Validate Installation
```bash
sudo systemctl status influxdb
sudo systemctl status telegraf
sudo systemctl status grafana-server
```
Ensure all services are running without errors.

## Conclusion
You have successfully installed and configured InfluxDB, Telegraf, and Grafana on your Raspberry Pi. All three services are set to run at startup. You can now start collecting and visualizing your data!
