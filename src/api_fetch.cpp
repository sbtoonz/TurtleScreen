#include "api_fetch.h"
#include "afc_config.h"

String apiURL;
float eventTime;
LaneStatus lanes[MAX_LANES];
int numLanesFound;
const char *currentLoad;
bool toolLoaded;
bool hubLoaded;
bool currentLoadChanged;
char currentLoadBuffer[32] = "";
int numUnits;
int numLanes;
char unitType[24] = "";
uint32_t lastApiUpdate;
HTTPClient http;

void fetchDataTask(void *pvParameters)
{
    while (true)
    {
        if (WiFi.status() == WL_CONNECTED && !apiURL.isEmpty())
        {
#ifdef DEBUG_OUTPUT
            DEBUG_PRINT("Trying to grab API data \n");
#endif
            http.begin(apiURL);
            int httpResponseCode = http.GET();

            if (httpResponseCode == 200)
            {
                String payload = http.getString();
#ifdef DEBUG_OUTPUT
                DEBUG_PRINTLN("Received data:");
                DEBUG_PRINTLN(payload);
#endif
                ParseAPIResponse(payload);
            }
            else
            {
                DEBUG_PRINT("HTTP error: ");
                DEBUG_PRINTLN(httpResponseCode);
            }
            http.end();
            moonraker.get_printer_ready();
            if(!moonraker.unready){
                moonraker.get_AFC_status();
                moonraker.get_printer_info();
                if(moonraker.data.printing)
                {
                    moonraker.get_progress();
                }
            }
        }
        else
        {
            DEBUG_PRINTLN("Wi-Fi not connected or API URL not set");
        }
        lastApiUpdate = xTaskGetTickCount();
        vTaskDelay(pdMS_TO_TICKS(900));
    }
}

void ParseAPIResponse(const String &jsonResponse) {
    DEBUG_PRINTLN("Running API Parse");
    DynamicJsonDocument doc(8192);
    DeserializationError error = deserializeJson(doc, jsonResponse);

    if (error) {
        DEBUG_PRINT("Failed to parse JSON: ");
        DEBUG_PRINTLN(error.f_str());
        return;
    }

    // Use configurable status key (defaults to "status:")
    JsonObject afc = doc[afcConfig.fields.status_key]["AFC"];
    if (afc.isNull()) {
        // Fallback: try alternate key
        afc = doc["status"]["AFC"];
    }
    if (afc.isNull()) {
        afc = doc["status:"]["AFC"];
    }
    if (afc.isNull()) {
        DEBUG_PRINTLN("AFC key not found in response.");
        return;
    }

    // Find the first unit (e.g. "Turtle_1")
    JsonObject unitObj;
    String unitName;
    for (JsonPair kv : afc) {
        String key = kv.key().c_str();
        if (key == "system") continue;
        unitObj = kv.value().as<JsonObject>();
        unitName = key;
        break;
    }

    if (!unitObj.isNull()) {
        // Get unit system info
        JsonObject unitSystem = unitObj["system"];
        if (!unitSystem.isNull()) {
            hubLoaded = unitSystem[afcConfig.fields.unit_hub_loaded].as<bool>();
            const char *type = unitSystem[afcConfig.fields.unit_type];
            if (type) {
                strncpy(unitType, type, sizeof(unitType) - 1);
                unitType[sizeof(unitType) - 1] = '\0';
            }
            DEBUG_PRINT("Unit type: ");
            DEBUG_PRINTLN(unitType);
            DEBUG_PRINT("Hub loaded: ");
            DEBUG_PRINTLN(hubLoaded ? "true" : "false");
        }

        // Iterate lanes within the unit (skip "system" key)
        numLanesFound = 0;
        for (JsonPair kv : unitObj) {
            String key = kv.key().c_str();
            if (key == "system") continue;
            if (numLanesFound >= MAX_LANES) break;

            JsonObject laneData = kv.value().as<JsonObject>();
            LaneStatus &lane = lanes[numLanesFound];

            strncpy(lane.name, key.c_str(), LANE_NAME_LEN - 1);
            lane.name[LANE_NAME_LEN - 1] = '\0';

            const char *mapVal = laneData[afcConfig.fields.lane_map];
            if (mapVal) {
                strncpy(lane.map, mapVal, sizeof(lane.map) - 1);
                lane.map[sizeof(lane.map) - 1] = '\0';
            } else {
                lane.map[0] = '\0';
            }

            lane.load = laneData[afcConfig.fields.lane_load].as<bool>();
            lane.prep = laneData[afcConfig.fields.lane_prep].as<bool>();
            lane.tool_loaded = laneData[afcConfig.fields.lane_tool_loaded].as<bool>();
            lane.loaded_to_hub = laneData[afcConfig.fields.lane_hub_loaded].as<bool>();
            lane.lane_index = laneData[afcConfig.fields.lane_index].as<int>();

            const char *mat = laneData[afcConfig.fields.lane_material];
            if (mat) {
                strncpy(lane.material, mat, sizeof(lane.material) - 1);
                lane.material[sizeof(lane.material) - 1] = '\0';
            } else {
                lane.material[0] = '\0';
            }

            const char *col = laneData[afcConfig.fields.lane_color];
            if (col) {
                strncpy(lane.color, col, sizeof(lane.color) - 1);
                lane.color[sizeof(lane.color) - 1] = '\0';
            } else {
                lane.color[0] = '\0';
            }

            lane.weight = laneData[afcConfig.fields.lane_weight].as<float>();
            lane.spool_id = laneData[afcConfig.fields.lane_spool_id] | -1;

            DEBUG_PRINT("Lane: ");
            DEBUG_PRINT(lane.name);
            DEBUG_PRINT(" map=");
            DEBUG_PRINT(lane.map);
            DEBUG_PRINT(" load=");
            DEBUG_PRINT(lane.load ? "true" : "false");
            DEBUG_PRINT(" prep=");
            DEBUG_PRINT(lane.prep ? "true" : "false");
            DEBUG_PRINT(" tool_loaded=");
            DEBUG_PRINT(lane.tool_loaded ? "true" : "false");
            DEBUG_PRINT(" loaded_to_hub=");
            DEBUG_PRINTLN(lane.loaded_to_hub ? "true" : "false");

            numLanesFound++;
        }
    } else {
        DEBUG_PRINTLN("No unit found in AFC response.");
    }

    // Parse AFC system information
    JsonObject system = afc["system"];
    if (!system.isNull()) {
        currentLoadChanged = false;
        currentLoad = system[afcConfig.fields.sys_current_load].as<const char *>();

        if (currentLoad == nullptr) {
            if (currentLoadBuffer[0] != '\0') {
                currentLoadBuffer[0] = '\0';
                currentLoadChanged = true;
            }
        } else {
            if (strcmp(currentLoadBuffer, currentLoad) != 0) {
                strncpy(currentLoadBuffer, currentLoad, sizeof(currentLoadBuffer) - 1);
                currentLoadBuffer[sizeof(currentLoadBuffer) - 1] = '\0';
                currentLoadChanged = true;
            }
        }

        numUnits = system[afcConfig.fields.sys_num_units] | 0;
        numLanes = system[afcConfig.fields.sys_num_lanes] | 0;

        // Parse extruder tool status
        JsonObject extruders = system["extruders"];
        if (!extruders.isNull()) {
            for (JsonPair kv : extruders) {
                JsonObject ext = kv.value().as<JsonObject>();
                toolLoaded = ext[afcConfig.fields.ext_tool_status].as<bool>();
                DEBUG_PRINT("Tool loaded (");
                DEBUG_PRINT(kv.key().c_str());
                DEBUG_PRINT("): ");
                DEBUG_PRINTLN(toolLoaded ? "true" : "false");
                break;
            }
        }
    } else {
        DEBUG_PRINTLN("System key not found in AFC.");
    }

    DEBUG_PRINT("Lanes found: ");
    DEBUG_PRINTLN(numLanesFound);
    for (int i = 0; i < numLanesFound; i++) {
        DEBUG_PRINT(lanes[i].name);
        DEBUG_PRINT(" load=");
        DEBUG_PRINTLN(lanes[i].load ? "true" : "false");
    }
    DEBUG_PRINT("Tool Status: ");
    DEBUG_PRINTLN(toolLoaded ? "true" : "false");
    DEBUG_PRINT("Current Load: ");
    DEBUG_PRINTLN(currentLoadBuffer);
}
