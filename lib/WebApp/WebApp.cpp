#include "WebApp.h"
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <SPIFFS.h>
#include <SD_MMC.h>
#include "CapturePhoto.h"
#include "esp_camera.h"
//#include "camera_pins.h"


/*void createServer(void) //remove and use it in main.cpp to ensure use one asyncWebServer object
{
  AsyncWebServer server(80);
}*/

void SetPageRoute(AsyncWebServer &server, const char* route, const char* source)
{
    server.on(route , HTTP_GET, [source](AsyncWebServerRequest * request){
    request->send_P(200, "text/html",source);
  });
}

void SetImageRoute(AsyncWebServer &server, const char* route, const char* source)
{
    server.on(route,  HTTP_GET, [source](AsyncWebServerRequest * request){
    request->send(SPIFFS, source, "image/jpg");
  });
}

void SetImageRouteSD(AsyncWebServer &server, const char* route, const char* source)
{
    server.on(route,  HTTP_GET, [source](AsyncWebServerRequest * request){
      //check file exist
      if(SD_MMC.exists(source)){
    request->send(SD_MMC, source, "image/jpg");
      } else {
        request-> send(404, "text/plain", "File Not Found");
      }
  });
}

void SetButtonRoute(AsyncWebServer &server, const char* route, const char* source) {
    server.on(route, HTTP_GET, [source](AsyncWebServerRequest *request) {
        camera_fb_t* fb = capture_photo();
        if (!fb) {
            request->send(500, "text/plain", "Camera capture failed");
            return;
        }
        File file = SD_MMC.open(source, FILE_WRITE);
        if (file) {
            file.write(fb->buf, fb->len);
            file.close();
        }
        esp_camera_fb_return(fb);
        request->send(200, "text/plain", "Photo Captured");
    });
}

void Web_begin(AsyncWebServer &server)
{
  server.begin();
}