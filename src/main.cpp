/*
 * ESP32-CAM Web Server with Photo Capture and Image Viewing
 * 
 * This project captures an image using ESP32-CAM and serves it on a web interface.
 * Features:
 * - Captures photos and saves them to an SD card.
 * - Displays images on a web page via SPIFFS or SD.
 * - Includes a web server with dynamic image updates.
 */

#include "WiFiManager.h"
#include "Arduino.h"
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <FS.h>
#include <SD_MMC.h>   // SD card
#include "WebApp.h"
#include "CapturePhoto.h"
#include <soc/soc.h>
#include <soc/rtc_cntl_reg.h>
#include <driver/rtc_io.h>

// Camera PIN Configuration (for AI-THINKER module)
#define PWDN_GPIO_NUM   32
#define RESET_GPIO_NUM  -1
#define XCLK_GPIO_NUM   0
#define SIOD_GPIO_NUM   26
#define SIOC_GPIO_NUM   27
#define Y9_GPIO_NUM     35
#define Y8_GPIO_NUM     34
#define Y7_GPIO_NUM     39
#define Y6_GPIO_NUM     36
#define Y5_GPIO_NUM     21
#define Y4_GPIO_NUM     19
#define Y3_GPIO_NUM     18
#define Y2_GPIO_NUM     5
#define VSYNC_GPIO_NUM  25
#define HREF_GPIO_NUM   23
#define PCLK_GPIO_NUM   22

// WiFi credentials (better to use a configuration file instead of hardcoding)
const char* ssid = "your_ssid";
const char* password = "your_password";

#define PHOTO_PATH "/image.jpg"  // File path for the captured image
AsyncWebServer server(80);  // Global web server instance
camera_config_t config;  // Camera configuration struct

// HTML page for displaying the image
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32-CAM Image Viewer</title>
</head>
<body>
    <h1>ESP32 SPIFFS Image Viewer</h1>
    <img src="image" alt="Captured Image" style="max-width:100%; height:auto;">
    <button onclick="capturePhoto()">Capture Photo</button>
    <script>
        function capturePhoto() {
            fetch('/capture').then(response => location.reload());
        }
    </script>
</body>
</html>
)rawliteral";

void setup() {
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Disable brownout detection
    Serial.begin(9600);

    // Camera setup
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    
    // Configure frame size based on available PSRAM
    if (psramFound()) {
        config.frame_size = FRAMESIZE_VGA;
        config.jpeg_quality = 10;
        config.fb_count = 2;
    } else {
        config.frame_size = FRAMESIZE_SVGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }

    // Initialize camera
    if (!camera_init(config)) {
        Serial.println("Camera initialization failed!");
        ESP.restart();
    }

    warmup_sensor(30);
    WiFiManager_begin(ssid, password);

    // Mount SPIFFS and SD card
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed!");
        ESP.restart();
    }
    if (!SD_MMC.begin()) {
        Serial.println("SD Card Mount Failed!");
    }

    Serial.print("IP Address: http://");
    Serial.println(WiFi.localIP());

    // Capture initial photo
    camera_fb_t* fb = capture_photo();
    if (fb) {
        File file = SD_MMC.open(PHOTO_PATH, FILE_WRITE);
        if (file) {
            file.write(fb->buf, fb->len);
            file.close();
            Serial.println("Photo Captured and Saved.");
        }
        esp_camera_fb_return(fb);
    }

    // Configure web server
    SetPageRoute(server, "/", index_html);
    SetImageRouteSD(server, "/image", PHOTO_PATH);
    SetButtonRoute(server, "/capture", PHOTO_PATH);

    /*server.on("/capture", HTTP_GET, [](AsyncWebServerRequest *request) {
        camera_fb_t* fb = capture_photo();
        if (!fb) {
            request->send(500, "text/plain", "Camera capture failed");
            return;
        }
        File file = SD_MMC.open(PHOTO_PATH, FILE_WRITE);
        if (file) {
            file.write(fb->buf, fb->len);
            file.close();
        }
        esp_camera_fb_return(fb);
        request->send(200, "text/plain", "Photo Captured");
    });*/
    
    server.begin();
}

void loop() {
    // Keep server running
}
