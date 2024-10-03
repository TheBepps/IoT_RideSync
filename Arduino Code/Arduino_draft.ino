#include <ArduinoBLE.h>
#include "Arduino_BMI270_BMM150.h"
#include "Arduino_HS300x.h"
#include <ReefwingLPS22HB.h>
#include "INA226.h"


// ------------- GLOBAL VARIABLES -------------

// Define services
BLEService EnvService("181A"); // Environmental data service UUID
BLEService AccService("12345678-1234-1234-1234-1234567890AB"); // IMU data service UUID (custom)

BLEService INAService("12345678-1234-1234-1234-1234512345AB"); // INA data service UUID (custom)

// Define characteristics
BLEStringCharacteristic ble_press("2A6D", BLERead | BLENotify,10);
BLEStringCharacteristic ble_temp("2A6E", BLERead | BLENotify, 10);  // Set the max length of the string
BLEStringCharacteristic ble_hum("2A6F", BLERead | BLENotify,10);
BLECharacteristic ble_acc("12345678-1234-1234-1234-1234567890CD", BLERead | BLENotify, 20);
BLECharacteristic ble_gyro("12345678-1234-1234-1234-1234567890EF", BLERead | BLENotify, 20);

BLEStringCharacteristic ble_INA("12345678-1234-1234-1234-1234512345AC", BLERead | BLENotify, 20);
BLEStringCharacteristic ble_INA2("12345678-1234-1234-1234-1234512345AD", BLERead | BLENotify, 20);


// GPS data


// Environment data
float temp;
float hum;
float press;
char tempStr[10];

// IMU data
int16_t x_int, y_int, z_int;
int16_t uu_int, v_int, w_int;
float x, y, z;
float u, v, w;
char acceleration[20];
char gyroscope_data[20];

// Mean accelerometer data
float sum_x = 0;
float sum_y = 0;
float sum_z = 0;
float count_acc = 0;
float mean_x, mean_y, mean_z;

// Mean gyroscope data
float sum_u = 0;
float sum_v = 0;
float sum_w = 0;
float count_gyro = 0;
float mean_u, mean_v, mean_w;

// INA data
float volt_TEG, curr_TEG, volt_batt_mV, curr_consumption, curr_panel, volt_prod_mV;
char INA_meas[20];
char INA2_meas[20];

// Timing 
int current_time;
int last_env_send_time = 0;
int last_acc_send_time = 0;
int last_gyro_send_time = 0;

int last_INA_send_time = 0;
int env_send_period = 2000;  // 20 seconds
int acc_send_period = 2000;   // 2 seconds
int gyro_send_period = 2000;  // 2 seconds

int INA_send_period = 1000;   // 1 second

 
// INA226
INA226 INA_1(0x44);
INA226 INA_2(0x45);
INA226 INA_3(0x46); 

// Pressure sensor
ReefwingLPS22HB barom;


// --------------- SETUP AND LOOP -------------------

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600); // Serial for debug
  Serial1.begin(9600); // GPS UART communication

  Wire.begin(); // I2C communication for INA226 sensors

  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
  }

  // Sensors initialization
  if (!HS300x.begin()) {
  Serial.println("Failed to initialize humidity temperature sensor!");
  }
  barom.begin();
  if (!barom.connected()) {
  Serial.println("Failed to initialize pressure sensor!");
  }
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
  }
  if (!INA_1.begin() )
  {
    Serial.println("Could not connect to INA226_1 (0x44).");
  }
  if (!INA_2.begin() )
  {
    Serial.println("Could not connect to INA226_2 (0x45).");
  }
  if (!INA_3.begin() )
  {
    Serial.println("Could not connect to INA226_3 (0x46)."); 
  }

  // INA226 parameters
  INA_1.setMaxCurrentShunt(1, 0.083);
  INA_1.setAverage(2);
  INA_1.setBusVoltageConversionTime(7);
  INA_2.setMaxCurrentShunt(1, 0.398);
  INA_2.setAverage(2);
  INA_2.setBusVoltageConversionTime(7);
  INA_3.setMaxCurrentShunt(1, 0.083); 
  INA_3.setAverage(2);
  INA_3.setBusVoltageConversionTime(7);
 
  // BLE services and characteristics initialization 
  BLE.setLocalName("Nano33BLErev2");
  BLE.setAdvertisedService(EnvService);
  BLE.setAdvertisedService(AccService);

  BLE.setAdvertisedService(INAService);

  EnvService.addCharacteristic(ble_temp);
  EnvService.addCharacteristic(ble_hum);
  EnvService.addCharacteristic(ble_press);

  AccService.addCharacteristic(ble_acc);
  AccService.addCharacteristic(ble_gyro);



  INAService.addCharacteristic(ble_INA);
  INAService.addCharacteristic(ble_INA2);

  BLE.addService(EnvService);
  BLE.addService(AccService);

  BLE.addService(INAService);
  
  ble_temp.writeValue("17");
  ble_hum.writeValue("17");
  ble_press.writeValue("17");

  ble_acc.writeValue("x0 y0 z0");
  ble_gyro.writeValue("u0 v0 w0");



  ble_INA.writeValue("c0000 v0000 c0000");
  ble_INA2.writeValue("c0000 v0000 c0000");

  BLE.advertise();
  Serial.println("BLE device is now advertising");
}

void loop() {

  int count = 0;

  BLEDevice central = BLE.central();

  if (central) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    while (central.connected()) {

      current_time = millis();

      // Sum contributions of accelerometer data to compute the mean value before sending BLE data
      accumulate_acc();
      
      // Sum contributions of gyroscope data to compute the mean value before sending BLE data
      accumulate_gyro();



      // Send environment data (every 20 seconds)
      send_env_data();

      // Send acc data (every 2 seconds)
      send_acc_data();

      // Send gyro data (every 2 seconds)
      send_gyro_data();



      // Send current measurement data (every 1 second)
      send_INA_data();

    }

    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }

  else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}



// --------- Functions to send BLE data ----------------

void send_env_data(){

  if ((current_time - last_env_send_time) > env_send_period) {

    // Temperature
    temp = HS300x.readTemperature();

    // Humidity
    hum = HS300x.readHumidity();

    // Pressure
    press = barom.readPressure();

    // Write values to BLE characteristics
    snprintf(tempStr, sizeof(tempStr), "%.2f", temp);  // Convert float to string with 2 decimal places
    ble_temp.writeValue(tempStr);  // Send the temperature as a string
    snprintf(tempStr, sizeof(tempStr), "%.2f", press);
    ble_press.writeValue(tempStr);
    snprintf(tempStr, sizeof(tempStr), "%.2f", hum);
    ble_hum.writeValue(tempStr);

    // Update last_env_send_time
    last_env_send_time = current_time;

    // Serial print
    Serial.println("Sending environment data");
    Serial.print("Temperature: "); Serial.print(temp); Serial.println(" °C");
    Serial.print("Humidity: "); Serial.print(hum); Serial.println(" %");
    Serial.print("Pressure: "); Serial.print(press); Serial.println(" hPa");
  } 
  
}

void send_acc_data() {

  if ((current_time - last_acc_send_time) > acc_send_period) {
  
    // Mean acceleration
    mean_x = sum_x/count_acc;
    mean_y = sum_y/count_acc;
    mean_z = sum_z/count_acc;
    x_int = int(round(mean_x*1000));
    y_int = int(round(mean_y*1000));
    z_int = int(round(mean_z*1000));
    snprintf(acceleration, sizeof(acceleration), "x%d y%d z%d", x_int, y_int, z_int);

    // Write values to BLE characteristics
    ble_acc.writeValue(acceleration);

    // Update last_acc_send_time
    last_acc_send_time = current_time;

    // Reset values
    sum_x = 0;
    sum_y = 0;
    sum_z = 0;
    count_acc = 0;

    // Serial print
    Serial.println("Sending accelerometer data "); 
    Serial.println(acceleration);
  
  }

}


void send_gyro_data() {

  if ((current_time - last_gyro_send_time) > gyro_send_period) {
  
    // Mean gyroscope data
    mean_u = sum_u/count_gyro;
    mean_v = sum_v/count_gyro;
    mean_w = sum_w/count_gyro;
    uu_int = int(round(mean_u*1000));
    v_int = int(round(mean_v*1000));
    w_int = int(round(mean_w*1000));
    snprintf(gyroscope_data, sizeof(gyroscope_data), "u%d v%d w%d", uu_int, v_int, w_int);
    
    // Write values to BLE characteristics
    ble_gyro.writeValue(gyroscope_data);

    // Update last_gyro_send_time
    last_gyro_send_time = current_time;

    // Reset values
    sum_u = 0;
    sum_v = 0;
    sum_w = 0;
    count_gyro = 0;

    // Serial print
    Serial.println("Sending gyro data "); 
    Serial.println(gyroscope_data);
  
  }

}



void send_INA_data() {

  if ((current_time - last_INA_send_time) > INA_send_period) {
  
    // INA_1 measurements
    float shunt_1 = INA_1.getShuntVoltage_mV();
    //curr_consumption = shunt_1/0.083;           // current consumption [mA]
    curr_consumption = random(0, 100);
    //volt_batt_mV = INA_1.getBusVoltage_mV();    // battery voltage level [mV]
    volt_batt_mV = random(0, 100);
    //INA_2 measurements
    float shunt_2 = INA_2.getShuntVoltage_mV(); // TEG current production [mA]
    //curr_TEG = shunt_2/0.398;
    curr_TEG = random(0, 100);
    //volt_TEG = INA_2.getBusVoltage_mV();        // TEG voltage [mV]
    volt_TEG = random(0, 100);
    //INA_3 measurements
    float shunt_3 = INA_3.getShuntVoltage_mV(); // Panel current production [mA]
    //curr_panel = shunt_3/0.083; 
    curr_panel = random(0, 100);
    //volt_prod_mV = INA_3.getBusVoltage_mV();    // Production voltage level [mV]
    volt_prod_mV = random(0, 100);

    snprintf(INA_meas, sizeof(INA_meas), "v%d v%d c%d", int(volt_TEG), int(volt_prod_mV), int(curr_TEG));
    snprintf(INA2_meas, sizeof(INA2_meas), "v%d c%d c%d", int(volt_batt_mV), int(curr_consumption), int(curr_panel));

    // Write values to BLE characteristics
    ble_INA.writeValue(INA_meas);
    ble_INA2.writeValue(INA2_meas);

    // Update last_gyro_send_time
    last_INA_send_time = current_time;

    // Serial print
    Serial.println("Sending INA data "); 
    Serial.print("Battery voltage: "); Serial.println(volt_batt_mV);
    Serial.print("Current consumption: "); Serial.println(curr_consumption);
    Serial.print("TEG voltage: "); Serial.println(volt_TEG);
    Serial.print("TEG current: "); Serial.println(curr_TEG);
    Serial.print("Panel current: "); Serial.println(curr_panel);
    Serial.print("Production voltage: "); Serial.println(volt_prod_mV);
  }

}

// GPS FUNCTIONS DEFINITION

// Function to accumulate accelerometer data
void accumulate_acc() {
  // new values
  IMU.readAcceleration(x, y, z); 
  // sum contributions
  sum_x = sum_x + x;
  sum_y = sum_y + y;
  sum_z = sum_z + z;
  count_acc = count_acc + 1;
}

// Function to accumulate gyroscope data
void accumulate_gyro() {
  // new values
  IMU.readGyroscope(u, v, w);
  // sum contributions
  sum_u = sum_u + u;
  sum_v = sum_v + v;
  sum_w = sum_w + w;
  count_gyro = count_gyro + 1;
}



