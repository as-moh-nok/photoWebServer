#ifndef _WEB_APP_H
#define _WEB_APP_H
#include <ESPAsyncWebServer.h>  // Include the required library


void SetPageRoute(AsyncWebServer &server, const char* route, const char* source);
void SetImageRoute(AsyncWebServer &server, const char* route, const char* source);
void SetImageRouteSD(AsyncWebServer &server, const char* route, const char* source);
void SetButtonRoute(AsyncWebServer &server, const char* route, const char* source);

void Web_begin(AsyncWebServer &server);


#endif //_WEB_APP_H