// TimeNTP.h

#include <TimeLib.h> 
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char ssid[] = "LA134-2016";  //  your network SSID (name)
const char pass[] = "MAD2016TI";       // your network password

// NTP Server
const char* ntpServerName = "time.nist.gov";
IPAddress timeServer;

const int timeZone = 1;     // Central European Time

WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

void setupTimeNTP(void);

void printDigits(int digits);

void digitalClockDisplay(void);

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address);

time_t getNtpTime(void);