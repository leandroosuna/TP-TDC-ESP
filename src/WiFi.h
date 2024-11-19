#pragma once

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>
#include "common.h"
#include "FS.h"


void setupWiFi();

bool anyChange();
int getK();
int getObj();
void handleNotFound(AsyncWebServerRequest *request);
void newPoints(int, int);