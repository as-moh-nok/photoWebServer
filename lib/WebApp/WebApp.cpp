#include "WebApp.h"
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <SPIFFS.h>
#include <SD_MMC.h>
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

void Web_begin(AsyncWebServer &server)
{
  server.begin();
}