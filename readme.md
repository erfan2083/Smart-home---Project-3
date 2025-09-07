# 🏠 EFN Smart Home System (ESP32 + RTOS + Web UI + OLED)

## 📖 Overview
This project is a **Smart Home Automation System** built on the **ESP32** microcontroller.  
It uses **FreeRTOS** tasks for concurrency, allowing real-time control and monitoring of multiple devices such as lights, an air conditioner, and sensors.  

The project integrates:
- **Web-based UI** served by the ESP32.
- **API endpoints** for controlling and monitoring devices.
- **OLED display (SSD1306)** for real-time visualization.
- **DHT22 sensor** for temperature and humidity monitoring.
- **PIR sensors** for motion detection in different rooms.
- **Day/Night detection** using an LDR sensor.

---

## ⚙️ Hardware Components
| Component             | Purpose                                   | Pin |
|-----------------------|-------------------------------------------|-----|
| **ESP32**            | Main MCU with WiFi support                 | —   |
| **DHT22**            | Temperature & humidity sensor              | 21  |
| **PIR – Living Room**| Motion detection (living room)             | 12  |
| **PIR – Bedroom**    | Motion detection (bedroom)                 | 13  |
| **PIR – Bathroom**   | Motion detection (bathroom)                | 0   |
| **LDR Sensor**       | Detects day/night                          | 36  |
| **LEDs**             | Simulate room lights & AC                  | 2, 4, 8, 7, 32, 33, 27 |
| **SSD1306 OLED**     | Displays system info (I2C interface)       | SDA/SCL |

---

## 🖥️ Software Architecture
This system is divided into **three main layers**:

### 🔹 1. RTOS Tasks
- `TaskDisplay`: Updates OLED display with current status (temperature, humidity, lights, AC).
- `TaskDHT`: Continuously reads temperature & humidity from DHT22.
- `TaskPIRLiving`: Handles motion detection and lights in the living room.
- `TaskPIRBedroom`: Handles motion detection and lights in the bedroom.
- `TaskPIRBathroom`: Handles motion detection and light in the bathroom.
- `TaskACAuto`: Manages air conditioner state based on temperature.

### 🔹 2. Web Server (ESPAsyncWebServer)
The ESP32 hosts an HTTP server that:
- Serves the **HTML UI page**.
- Provides **API endpoints** to toggle lights, AC, and monitor sensors.

### 🔹 3. OLED Display (SSD1306)
- Displays **temperature & humidity**.
- Shows **lights, AC, and room status** in real-time.
- Displays **sun/moon icons** based on day/night (from LDR sensor).
- Intro screen with project logo at startup.

---

## 🌐 API Endpoints

| Endpoint | Method | Description | Example Response |
|----------|--------|-------------|------------------|
| `/` | GET | Returns Smart Home HTML UI | HTML Page |
| `/api/light?id=...` | GET | Toggle light by ID (`bedroom-light-1-btn`, etc.) | `"light done"` |
| `/api/ac/light` | GET | Toggle AC manually | `"ac done"` |
| `/api/status/temperature` | GET | Get current temperature | `{"temperature": 24.5}` |
| `/api/status/humidity` | GET | Get current humidity | `{"humidity": 40.2}` |
| `/api/auto/bathroom` | GET | Get bathroom PIR status | `{"state": 1}` |
| `/api/auto/other?bedRoomMode=1&livingRoomLightMode=0&livingRoomAcMode=1` | GET | Set auto/manual modes for bedroom, living room lights, and AC | `{"livingRoom":1,"bedRoom":0,"ac":1}` |

---

## 🖼️ OLED UI
- Displays **temperature & humidity** on the top.
- Shows **day/night icon** (sun or moon).
- Visual room layout:
  - Bedroom lights
  - Bathroom light
  - Living room lights
  - AC status

---

## 🚀 Getting Started

### 🔧 Prerequisites
- Install **PlatformIO** (recommended) or Arduino IDE.
- Install required libraries:
  - `ESPAsyncWebServer`
  - `AsyncTCP`
  - `DHT sensor library`
  - `Adafruit GFX`
  - `Adafruit SSD1306`

### 📥 Uploading
1. Clone this repository:
   ```bash
   git clone https://github.com/your-username/efn-smart-home.git
   cd efn-smart-home

2. Open project in PlatformIO or Arduino IDE.

3. Flash to ESP32.

4. Open Serial Monitor at 115200 baud.

5. Copy the IP address displayed and open it in your browser.

### 👨‍💻 Author
**Erfan Manouchehri**