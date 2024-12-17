#ifndef _SPIFFS_H
#define _SPIFFS_H

#include <StringArray.h>
#include <SPIFFS.h>
#include <FS.h>

void Spiff_begin()
{
    if(!SPIFFS.begin(true)){
    Serial.println("An Error has occured while mounting spiffs");
    ESP.restart(); //reset esp32
  }
  else{
    delay(500);
    Serial.println("SPIFFA mounted successfully");
  }
}
#endif