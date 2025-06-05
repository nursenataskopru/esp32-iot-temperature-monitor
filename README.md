# ESP32 IoT Temperature & Humidity Monitor

This repository contains a complete ESP32-based IoT project that reads temperature and humidity from a DHT22 sensor, displays the values on an LCD1602 (via I²C), publishes the data to the Arduino IoT Cloud, and includes a red LED indicator that can be controlled remotely. The code was developed and tested in the Wokwi simulator, but can be deployed on any ESP32 DevKit-C board.

---

## Project Overview

This project demonstrates how to:

- Read temperature and humidity data from a **DHT22** sensor using an ESP32.  
- Display real-time readings on a **16×2 LCD** (I²C interface).  
- Publish sensor values and a control variable (LED on/off) to the **Arduino IoT Cloud**.  
- Control a **red LED** on the ESP32 remotely via the cloud dashboard.  
- Simulate the entire setup in the **Wokwi** online simulator (diagram.json provided).

You can adapt this code to any ESP32 DevKit-C (or compatible) board. The code is modular: the main sketch is in `sketch.ino`, and IoT-specific settings and property callbacks reside in `thingsProperties.h`.

---

## Features

- **DHT22 Sensor**: Measures temperature (°C) and relative humidity (%).  
- **LCD1602 Display (I²C)**: Shows “Temperature: XX.X °C” on the first row and “Humidity: XX.X %” on the second row.  
- **Arduino IoT Cloud Integration**:  
  - Publishes `sicaklik` (temperature) and `nem` (humidity) as **read-only** properties.  
  - Publishes and subscribes to `led` (boolean) as a **read/write** property.  
- **Red LED Indicator**:  
  - Turns **ON** if `led` property is set to `true` from the cloud dashboard.  
  - Turns **OFF** if `led` property is set to `false`.  
- **Wokwi Simulation**: `diagram.json` can be imported into [wokwi.com](https://wokwi.com/) to see the wiring and logic before deploying on real hardware.

---

## Hardware Components

1. **ESP32 DevKit-C** (any ESP32 board with I²C support)  
2. **DHT22 Temperature & Humidity Sensor**  
3. **LCD1602 Display with I²C Backpack**  
4. **Red LED** (with 1 kΩ current-limiting resistor)  
5. **Jumper Wires**  
6. **Breadboard** (optional, for real-world wiring)

---

## Wiring Diagram

Below is the wiring setup as used in the Wokwi simulation (refer to `diagram.json` for exact coordinates):

- **DHT22 Sensor**  
  - VCC → ESP32 3.3 V  
  - GND → ESP32 GND  
  - DATA → ESP32 GPIO 0  

- **LCD1602 (I²C Backpack)**  
  - VCC → ESP32 3.3 V  
  - GND → ESP32 GND  
  - SDA → ESP32 GPIO 21  
  - SCL → ESP32 GPIO 22  

- **Red LED**  
  - LED Anode (+) → 1 kΩ Resistor → ESP32 GPIO 12  
  - LED Cathode (–) → ESP32 GND  

> **Note:** If you use a different GPIO for the LED or DHT22 data pin, update `LED_PIN` or `DHTPIN` in `sketch.ino` accordingly.

---

## Software Dependencies

The required Arduino libraries are listed in `libraries.txt` (see below). Install them via the Arduino IDE’s Library Manager or manually:

```
LiquidCrystal I2C
DHT sensor library
Arduino_ConnectionHandler
AwsIotWiFiClient
SimpleWiFiClient
Arduino_JSON
```

1. **LiquidCrystal I2C** – for driving the LCD1602 over I²C.  
2. **DHT sensor library** – for reading the DHT22.  
3. **ArduinoIoTCloud**, **Arduino_ConnectionHandler** – for Arduino IoT Cloud integration.  
4. **AwsIotWiFiClient** and **SimpleWiFiClient** – cloud-related connection handlers (automatically installed with Arduino IoT Cloud).  
5. **Arduino_JSON** – for encoding/decoding JSON payloads when needed.

---

## File Structure

```
├── diagram.json
├── libraries.txt
├── thingsProperties.h
├── sketch.ino
├── wokwi-project.txt
└── README.md
```

- **diagram.json**  
  Wokwi simulation file. Import at https://wokwi.com/ to view the circuit.

- **libraries.txt**  
  Plain-text list of all required Arduino libraries.

- **thingsProperties.h**  
  - Defines `DEVICE_LOGIN_NAME` and `DEVICE_KEY` for Arduino IoT Cloud.  
  - Declares cloud-bound properties: `mesaj` (unused placeholder), `sicaklik`, `nem`, and `led`.  
  - Contains callback functions: `onMesajChange()` and `onLedChange()`.  
  - `initProperties()` sets up IoT Cloud properties and Wi-Fi credentials.

- **sketch.ino**  
  - `setup()` initializes serial, Wi-Fi, IoT Cloud properties, DHT sensor, LCD, and configures the LED pin.  
  - `loop()` reads DHT22 every 2 seconds, updates the LCD, syncs with Arduino IoT Cloud (sending `sicaklik` and `nem`), and toggles the LED based on the cloud property.

- **wokwi-project.txt**  
  Short instructions on how to simulate the project on Wokwi.

---

## Setup & Usage

Follow these steps to run the project on an actual ESP32 DevKit-C:

### 1. Clone the Repository

```bash
git clone https://github.com/<your-username>/esp32-iot-temperature-monitor.git
cd esp32-iot-temperature-monitor
```

### 2. Install Libraries

1. Open the Arduino IDE.  
2. Go to **Sketch → Include Library → Manage Libraries…**  
3. Search for and install each of the following:
   - **LiquidCrystal I2C**  
   - **DHT sensor library**  
   - **ArduinoIoTCloud** (this will pull in Arduino_ConnectionHandler, AwsIotWiFiClient, SimpleWiFiClient, Arduino_JSON, etc.)

> **Tip:** You can also use **libraries.txt** as a reference. If you prefer manual installation, download and unzip each library into your Arduino `libraries/` folder.

### 3. Configure Arduino IoT Cloud

1. Log in to [Arduino IoT Cloud](https://create.arduino.cc/iot).  
2. Create a new **Thing**.  
3. Copy the **Device ID** and **Device Key** from the cloud dashboard, then paste them into `thingsProperties.h`:

   ```cpp
   const char DEVICE_LOGIN_NAME[] = "YOUR_DEVICE_ID_HERE";
   const char DEVICE_KEY[]       = "YOUR_DEVICE_KEY_HERE";
   ```

4. Under **Thing Properties**, add:
   - `sicaklik` (Type: Number, Unit: °C, Access: Read)  
   - `nem` (Type: Number, Unit: %, Access: Read)  
   - `led` (Type: Boolean, Access: Read/Write)  

5. Link the Thing to your physical ESP32 board (follow the cloud’s onboarding wizard).  
6. Select your Wi-Fi SSID and password in the IoT Cloud wizard. Make sure they match `SSID` and `PASS` in `thingsProperties.h`.

### 4. Upload to ESP32

1. Open `esp32-iot-temperature-monitor.ino` (i.e., `sketch.ino`) in the Arduino IDE.  
2. Confirm that your board settings match your actual ESP32 (e.g., **Board: ESP32 Dev Module**, **Port: COMx**).  
3. Click **Upload**.  
4. Open the **Serial Monitor** at **115200 baud** to see debug messages.

You should see messages like:

```
WiFi connecting...
WiFi connected. IP address: 192.168.xxx.xxx
Initializing IoT Cloud…
DHT22 sensor ready.
LCD initialized.
```

### 5. View on Arduino IoT Cloud Dashboard

1. Go to your Thing’s **Dashboard** on the Arduino IoT Cloud.  
2. Add widgets for:
   - Temperature (`sicaklik`)  
   - Humidity (`nem`)  
   - Toggle switch for `led`  

Now you can view live sensor data and toggle the red LED from anywhere with Internet access.

---

## Simulation (Wokwi)

To simulate the entire circuit without hardware:

1. Go to [Wokwi](https://wokwi.com/).  
2. Click **Projects → Import → From JSON**.  
3. Paste the contents of `diagram.json`.  
4. Press **Start Simulation**.  
5. Open the **Serial Monitor** tab in Wokwi to view debug output.  
6. On the simulated IoT Cloud (if configured), you can also toggle the LED property.

The `wokwi-project.txt` file contains a direct link and additional hints:
```
Downloaded from https://wokwi.com/projects/425066481839851521
Simulate this project on https://wokwi.com/
```

---

## License

This project is released under the [MIT License](LICENSE). Feel free to reuse, modify, and distribute as needed.

---

**Enjoy building your ESP32 IoT Temperature & Humidity Monitor!**
