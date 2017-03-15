/*
 *
 */

#include "config.h"

void setup()
{
	Serial.begin(115200);
	Serial.println("Init started.");
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
