// 
// 
// 

#include "TimeNTP.h"

void setupTimeNTP()
{
	Serial.begin(115200);
	while (!Serial); // Needed for Leonardo only
	delay(250);
	Serial.println("TimeNTP Example");
	Serial.print("Connecting to ");
	Serial.println(ssid);
	WiFi.begin(ssid, pass);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	//get a random server from the pool
	WiFi.hostByName(ntpServerName, timeServer);

	Serial.print("IP number assigned by DHCP is ");
	Serial.println(WiFi.localIP());
	Serial.println("Starting UDP");
	Udp.begin(localPort);
	Serial.print("Local port: ");
	Serial.println(Udp.localPort());
	Serial.println("waiting for sync");
	setSyncProvider(getNtpTime);
}

void printDigits(int digits) {
	// utility for digital clock display: prints preceding colon and leading 0
	Serial.print(":");
	if (digits < 10)
		Serial.print('0');
	Serial.print(digits);
}

void digitalClockDisplay() {
	// digital clock display of the time
	Serial.print(hour());
	printDigits(minute());
	printDigits(second());
	Serial.print(" ");
	Serial.print(day());
	Serial.print(".");
	Serial.print(month());
	Serial.print(".");
	Serial.print(year());
	Serial.println();
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
	// set all bytes in the buffer to 0
	memset(packetBuffer, 0, NTP_PACKET_SIZE);
	// Initialize values needed to form NTP request
	// (see URL above for details on the packets)
	packetBuffer[0] = 0b11100011;   // LI, Version, Mode
	packetBuffer[1] = 0;     // Stratum, or type of clock
	packetBuffer[2] = 6;     // Polling Interval
	packetBuffer[3] = 0xEC;  // Peer Clock Precision
							 // 8 bytes of zero for Root Delay & Root Dispersion
	packetBuffer[12] = 49;
	packetBuffer[13] = 0x4E;
	packetBuffer[14] = 49;
	packetBuffer[15] = 52;
	// all NTP fields have been given values, now
	// you can send a packet requesting a timestamp:                 
	Udp.beginPacket(address, 123); //NTP requests are to port 123
	Udp.write(packetBuffer, NTP_PACKET_SIZE);
	Udp.endPacket();
}

time_t getNtpTime()
{
	while (Udp.parsePacket() > 0); // discard any previously received packets
	Serial.println("Transmit NTP Request");
	sendNTPpacket(timeServer);
	uint32_t beginWait = millis();
	while (millis() - beginWait < 1500) {
		int size = Udp.parsePacket();
		if (size >= NTP_PACKET_SIZE) {
			Serial.println("Receive NTP Response");
			Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
			unsigned long secsSince1900;
			// convert four bytes starting at location 40 to a long integer
			secsSince1900 = (unsigned long)packetBuffer[40] << 24;
			secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
			secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
			secsSince1900 |= (unsigned long)packetBuffer[43];
			return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
		}
	}
	Serial.println("No NTP Response :-(");
	return 0; // return 0 if unable to get the time
}