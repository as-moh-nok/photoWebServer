//add spiff lib and capture image option and button on page

#include "WiFiManager.h"
#include "Arduino.h"
#include <ESPAsyncWebServer.h>
#include <StringArray.h>
#include <SPIFFS.h>
#include <FS.h>
#include <SD_MMC.h>   //SD card
#include "WebApp.h"
//#define CAMERA_MODEL_AI_THINKER
//#include "camera_pins.h"
#include "CapturePhoto.h"
//Disable brownour problems:
#include <soc/soc.h>
#include <soc/rtc_cntl_reg.h>
#include <driver/rtc_io.h>


//Pin definition for CAMERA_MODEL_AI_THINKER
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
#define PCLK_GPIO_NUM     22

const char* ssid = "hwa";
const char* password = "wifi1373";
#define PHOTO_PATH "/image.jpg"

AsyncWebServer server(80);//global asyncWebServer
camera_config_t config;


const char index_html[] PROGMEM = R"rawlitral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Image Viewer</title>
</head>
<body>
    <h1>ESP32 SPIFFS Image Viewer</h1>
    <img src="image" alt="Image from SPIFFS" style="max-width:100%; height:auto;">
</body>
</html>

)rawlitral";

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout

  Serial.begin(9600);

  //set up camera
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
  //config.pin_sscb_sda = SIOD_GPIO_NUM; //depricated
  //config.pin_sscb_scl = SIOC_GPIO_NUM; //depricated
  config.pin_sccb_sda = SIOD_GPIO_NUM;  // Updated field name
  config.pin_sccb_scl = SIOC_GPIO_NUM;  // Updated field nam
  
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if(psramFound()){
    config.frame_size = FRAMESIZE_VGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    config.jpeg_quality = 10;
    config.fb_count = 30; //buffer size Queue
  }else{
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12; //higher number = lower quality
    config.fb_count = 1;
  }
    
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x\n", err);
    return;
    //return false;  // Indicate failure
  }
  //return true;  // Success

  warmup_sensor(30);

  //set up WiFi
  WiFiManager_begin(ssid, password);

  //SPIFF begin and Check
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occured while mounting spiffs");
    ESP.restart(); //reset esp32
  }
  else{
    delay(500);
    Serial.println("SPIFFA mounted successfully");
  }

  if(!SD_MMC.begin()){
    Serial.println("SD Card Mount Failed");
  }


  Serial.print("IP Address: http://");
  Serial.println(WiFi.localIP());

  camera_fb_t* fb= capture_photo();
  //Path where new picture with be saved in SD Card
  //sprintf(str, "/image.jpg"); //format string 
    char str[64] = "/image.jpg";

  Serial.printf("Picture file name: %s\n", str );
  
  File file = SD_MMC.open(str, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file in writing mode");
  }
  else {
    file.write(fb->buf, fb->len); //fb is struc with buf and len property
    Serial.printf("Saved file to path: %s\n", str);
    //EEPROM.write(0, imgNum);//update imgNum back to eeprom at address 0
   // EEPROM.commit();  //save any changes
  }
  file.close();
  esp_camera_fb_return(fb);//This line releases the frame buffer (fb) back to the camera driver,//
  // freeing up memory after the image is saved
  
  Serial.println("Photo Captured, and free space");

  //route for root
  SetPageRoute(server, "/", index_html);
  SetImageRouteSD(server, "/image", PHOTO_PATH);

  server.begin();
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
