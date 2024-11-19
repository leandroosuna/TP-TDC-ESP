#pragma once
#include <Arduino.h>
#include "common.h"
#include "WiFi.h"
#include "SoftwareSerial.h"

enum STATE {hud, test};

#define HC05_TX D5
#define HC05_RX D6

void ledBlink();
