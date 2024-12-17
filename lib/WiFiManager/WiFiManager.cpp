#include "WiFiManager.h"
//#include "Database.h"
#include <WiFi.h>

void WiFiManager_begin(const char* ssid,const char* password)
{
    //set up WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
}