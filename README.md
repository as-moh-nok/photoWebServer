# ESP32-CAM Web Server with Photo Capture

## Overview
This project implements a **web server** using an **ESP32-CAM** module that captures images and displays them via a web interface. The captured image is stored on an **SD card**.

## Features
- **WiFi-enabled Web Server** for viewing and capturing images
- **Image Storage**: Saves captured images to an **SD card**
- **ESPAsyncWebServer** for handling HTTP requests
- **Web-based Image Capture Button**
- **Supports SPIFFS & SD_MMC for file storage**

## Hardware Requirements
- **ESP32-CAM** (AI-THINKER module recommended)
- **MicroSD card** (formatted as FAT32)
- **FTDI Programmer** (for uploading code)
- **External 5V Power Source** (recommended for stable operation)

## Installation & Setup
### 1. **Flash the Code to ESP32-CAM**
Ensure you have **Arduino IDE** installed with the following libraries:
- **ESPAsyncWebServer**
- **ESPAsyncTCP**
- **FS** (File System support)
- **SPIFFS**
- **SD_MMC**

**Steps:**
1. Clone this repository:
   ```sh
   git clone https://github.com/yourusername/esp32-cam-webserver.git
   cd esp32-cam-webserver
   ```
2. Open the project in **Arduino IDE**
3. Modify `ssid` and `password` in `main.cpp` to match your WiFi network.
4. Connect **FTDI programmer** to ESP32-CAM and upload the sketch.

### 2. **Access the Web Interface**
Once the ESP32-CAM boots up, check the **Serial Monitor** for the **IP Address**.
Access the web interface using:
   ```sh
   http://<ESP32-IP-Address>/
   ```

## How It Works
- The **ESP32-CAM** initializes WiFi and mounts **SPIFFS & SD_MMC**.
- A **web server** is started on port **80**.
- When the user visits the webpage, the last captured image is displayed.
- Clicking the **"Capture Photo"** button triggers the camera to take a picture and store it on the **SD card**.
- The latest image is served dynamically when requested.


## License
This project is **open-source** and licensed under the **MIT License**.

## Author
Developed by **Asma Mohammadi** (a.mohamma.nok@gmail.com)

