//add spiff lib and capture image option and button on page

#include "WiFiManager.h"
#include "Arduino.h"
#include <ESPAsyncWebServer.h>
#include <StringArray.h>
#include <SPIFFS.h>
#include <FS.h>
#include "WebApp.h"

const char* ssid = "hwa";
const char* password = "wifi1373";
#define PHOTO_PATH "/image.jpg"

AsyncWebServer server(80);//global asyncWebServer

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
  // put your setup code here, to run once:
  Serial.begin(9600);

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

  if (SPIFFS.exists(PHOTO_PATH)) {
    Serial.println("File exists: " + String(PHOTO_PATH));
  } else {
      Serial.println("File not found: " + String(PHOTO_PATH));
  }


  Serial.print("IP Address: http://");
  Serial.println(WiFi.localIP());

  //route for root
  SetPageRoute(server, "/", index_html);
  SetImageRoute(server, "/image", PHOTO_PATH);



  server.begin();
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
