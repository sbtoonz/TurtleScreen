#ifndef API_FETCH_H
#define API_FETCH_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "moonraker.h"
#include "debug.hpp"

extern float eventTime;
extern bool leg1Load;
extern bool leg2Load;
extern bool leg3Load;
extern bool leg4Load;
extern const char* currentLoad;
extern char currentLoadBuffer[32];
extern bool toolLoaded;
extern bool loadedToHub;
extern bool currentLoadChanged;
extern int numUnits;
extern int numLanes;

void fetchDataTask(void *pvParameters);
void ParseAPIResponse(const String& jsonResponse);
extern  HTTPClient http;

extern String apiURL;
extern TaskHandle_t apiFetchTaskHandle;
extern uint32_t lastApiUpdate;

#endif // API_FETCH_H
