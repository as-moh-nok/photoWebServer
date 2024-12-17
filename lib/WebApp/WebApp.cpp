#include "WebApp.h"
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <SPIFFS.h>


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

void Web_begin(AsyncWebServer &server)
{
  server.begin();
}