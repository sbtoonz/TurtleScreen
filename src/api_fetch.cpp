#include "api_fetch.h"

String apiURL;
float eventTime;
bool leg1Load;
bool leg2Load;
bool leg3Load;
bool leg4Load;
const char *currentLoad;
bool toolLoaded;
bool loadedToHub;
bool currentLoadChanged;
char currentLoadBuffer[32] = "";
int numUnits;
int numLanes;
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
                // Parse JSON data
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
    DynamicJsonDocument doc(4096); // Adjusted size as necessary
    DeserializationError error = deserializeJson(doc, jsonResponse);

    if (error) {
        DEBUG_PRINT("Failed to parse JSON: ");
        DEBUG_PRINTLN(error.f_str());
        return;
    }

    JsonObject result = doc["result"];
    if (result.isNull()) {
        DEBUG_PRINTLN("Result key not found in JSON.");
        return;
    }

    eventTime = result["eventtime"].as<float>();

    JsonObject status = result["status"];
    JsonObject afc = status["AFC"];
    JsonObject turtleUnit;
    String turtleUnitName;
    for (JsonPair kv : afc) {
        turtleUnitName = kv.key().c_str();  
        turtleUnit = kv.value().as<JsonObject>();
        break; 
    }

    if (!turtleUnit.isNull()) {
        DEBUG_PRINT(turtleUnitName + " Object: ");
        // Iterate through the legs (leg1, leg2, leg3, leg4)
        for (int leg = 1; leg <= 4; ++leg) {
            String legKey = "leg" + String(leg);
            if (turtleUnit.containsKey(legKey)) {
                JsonObject legData = turtleUnit[legKey];
                bool load = legData["load"];
                bool prep = legData["prep"];
                bool loadedToHub = legData["loaded_to_hub"];
                String material = legData["material"].as<String>();
                String spool_id = legData["spool_id"].as<String>();
                String color = legData["color"].as<String>();
                float weight = legData["weight"].as<float>();

                int lane = legData["LANE"];
                DEBUG_PRINT("Lane ");
                DEBUG_PRINT(lane);
                DEBUG_PRINT(" - Load: ");
                DEBUG_PRINT(load ? "true" : "false");
                DEBUG_PRINT(", Prep: ");
                DEBUG_PRINT(prep ? "true" : "false");
                DEBUG_PRINT(", Loaded to Hub: ");
                DEBUG_PRINT(loadedToHub ? "true" : "false");
                DEBUG_PRINT(", Material: ");
                DEBUG_PRINT(material);
                DEBUG_PRINT(", Spool ID: ");
                DEBUG_PRINT(spool_id);
                DEBUG_PRINT(", Color: ");
                DEBUG_PRINT(color);
                DEBUG_PRINT(", Weight: ");
                DEBUG_PRINTLN(weight);
                // Update leg load statuses
                switch (lane) {
                    case 1:
                        leg1Load = load;
                        break;
                    case 2:
                        leg2Load = load;
                        break;
                    case 3:
                        leg3Load = load;
                        break;
                    case 4:
                        leg4Load = load;
                        break;
                    default:
                        break;
                }
            } else {
                DEBUG_PRINT("Checking key: ");
                DEBUG_PRINTLN(legKey);
                DEBUG_PRINTLN("Key not found.");
            }
        }

        // Parsing Turtle Unit's system information (hub_loaded, etc.)
        JsonObject turtleSystem = turtleUnit["system"];
        if (!turtleSystem.isNull()) {
            bool hubLoaded = turtleSystem["hub_loaded"].as<bool>();
            bool canCut = turtleSystem["can_cut"].as<bool>();
            String screen = turtleSystem["screen"].as<String>();
            DEBUG_PRINT("Hub Loaded: ");
            DEBUG_PRINTLN(hubLoaded ? "true" : "false");
            DEBUG_PRINT("Can Cut: ");
            DEBUG_PRINTLN(canCut ? "true" : "false");
            DEBUG_PRINT("Screen: ");
            DEBUG_PRINTLN(screen);
            loadedToHub = hubLoaded; // Assigning hubLoaded status to toolLoaded if relevant
        }
    } else {
        DEBUG_PRINTLN("Unit key not found in AFC.");
    }

    // Parsing AFC system information
    JsonObject system = afc["system"];
    if (!system.isNull()) {
        currentLoadChanged = false;
        currentLoad = system["current_load"].as<const char *>();

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

        JsonObject extruder = system["extruders"]["extruder"];
        if (!extruder.isNull()) {
            toolLoaded = extruder["tool_start_sensor"].as<bool>();

            String buffer = extruder["buffer"].as<String>();
            String bufferStatus = extruder["buffer_status"].as<String>();
            DEBUG_PRINT("Tool Loaded: ");
            DEBUG_PRINTLN(toolLoaded ? "true" : "false");
            DEBUG_PRINT("Buffer: ");
            DEBUG_PRINTLN(buffer);
            DEBUG_PRINT("Buffer Status: ");
            DEBUG_PRINTLN(bufferStatus);
        }
    } else {
        DEBUG_PRINTLN("System key not found in AFC.");
    }
    DEBUG_PRINT("Lane 1 Status: ");
    DEBUG_PRINTLN(leg1Load);
    DEBUG_PRINT("Lane 2 Status: ");
    DEBUG_PRINTLN(leg2Load);
    DEBUG_PRINT("Lane 3 Status: ");
    DEBUG_PRINTLN(leg3Load);
    DEBUG_PRINT("Lane 4 Status: ");
    DEBUG_PRINTLN(leg4Load);
    DEBUG_PRINT("Tool Status: ");
    DEBUG_PRINTLN(toolLoaded ? "true" : "false");
    DEBUG_PRINT("Current Load: ");
    DEBUG_PRINTLN(currentLoad);
}