#pragma once

#include "config.h"

void printDigits(int digits);

void digitalClockDisplay();

void sendNTPpacket(IPAddress &address);

time_t getNtpTime();

void setupTimeNTP();