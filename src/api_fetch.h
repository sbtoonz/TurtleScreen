#ifndef API_FETCH_H
#define API_FETCH_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "moonraker.h"
#include "debug.hpp"

#define MAX_LANES 4
#define LANE_NAME_LEN 16

struct LaneStatus {
    char name[LANE_NAME_LEN];
    char map[8];
    bool load;
    bool prep;
    bool tool_loaded;
    bool loaded_to_hub;
    char material[16];
    char color[12];
    float weight;
    int spool_id;
    int lane_index;
};

extern float eventTime;
extern LaneStatus lanes[MAX_LANES];
extern int numLanesFound;
extern const char* currentLoad;
extern char currentLoadBuffer[32];
extern bool toolLoaded;
extern bool hubLoaded;
extern bool currentLoadChanged;
extern int numUnits;
extern int numLanes;
extern char unitType[24];

void fetchDataTask(void *pvParameters);
void ParseAPIResponse(const String& jsonResponse);
extern  HTTPClient http;

extern String apiURL;
extern TaskHandle_t apiFetchTaskHandle;
extern uint32_t lastApiUpdate;

#endif // API_FETCH_H
