/*
 * 
 */
 
#include <TimeLib.h> 
#include <ESP8266WiFi.h>

#include "TimeNTP_ESP8266WiFi.h"

void setup() 
{
  //init TimeNTP_ESP8266WiFi Library. Will also init Wifi (settings can be found in the lib)
  setupTimeNTP();
}

time_t prevDisplay = 0; // when the digital clock was displayed

void loop()
{  
  if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) { //update the display only if time has changed
      prevDisplay = now();
      digitalClockDisplay();  
    }
  }
}
