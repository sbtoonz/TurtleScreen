#include "wifi_manager.h"

const char *apSSID = "DeviceSetupAP";
const char *apPassword = "setup1234";
Preferences preferences;
AsyncWebServer server(80);

String targetSSID;
String targetPassword;
String targetHost;
TaskHandle_t WifITaskHandle = NULL;


void suspendTasks() {
    if (apiFetchTaskHandle != NULL) vTaskSuspend(apiFetchTaskHandle);
    if (lvglUiTaskHandle != NULL) vTaskSuspend(lvglUiTaskHandle);
    if (postTaskHandle != NULL) vTaskSuspend(postTaskHandle);
    if (apiFetchTaskHandle != NULL) vTaskSuspend(apiFetchTaskHandle);
}

void setupWebSite(){
    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/html", webpage_html); });

    server.on("/setConfig", HTTP_POST, [](AsyncWebServerRequest *request)
              {
        if (request->hasParam("ssid", true) && request->hasParam("password", true) && request->hasParam("host", true)) {
            targetSSID = request->getParam("ssid", true)->value();
            targetPassword = request->getParam("password", true)->value();
            targetHost = request->getParam("host", true)->value();

            saveCredentials(targetSSID, targetPassword, targetHost);
            DEBUG_PRINT("Host set as:");

            apiURL = "http://" + targetHost + "/printer/objects/query?AFC";
            DEBUG_PRINTLN(apiURL);

            if (xTaskCreate(connectToWiFiTask, "WiFi Connect Task", 4096, NULL, 8, &WifITaskHandle) != pdPASS) {
                DEBUG_PRINTLN("Failed to create WiFi Connect Task");
            }
            xTaskCreate(fetchDataTask, "Data Fetch Task", 4096, NULL, 8, &apiFetchTaskHandle);
            xTaskCreate(watchdog_task, "WatchdogTask", 2048, NULL, 9, NULL);
            xTaskCreate(moonraker_post_task, "moonraker post", 4096, NULL, 7, &postTaskHandle);

            request->send(200, "text/plain", "Configuration saved. Connecting to Wi-Fi...");
        } else {
            request->send(400, "text/plain", "Missing parameters.");
        } });

    server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", upload_html);
    });

    server.on(
    "/update", HTTP_POST, [](AsyncWebServerRequest *request) {
        bool updateSuccess = !Update.hasError();
        AsyncWebServerResponse *response = request->beginResponse(
            updateSuccess ? 200 : 500, "text/plain",
            updateSuccess ? "Update complete. Rebooting..." : "Update failed. Please try again."
        );
        response->addHeader("Connection", "close");
        request->send(response);
        if (updateSuccess) {
            // Give the response time to flush before restarting
            delay(1000);
            ESP.restart();
        }
    },
    [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
        if (index == 0) {
            Serial.printf("OTA update starting: %s\n", filename.c_str());
            suspendTasks();
            if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
                Update.printError(Serial);
                request->send(500, "text/plain", "Not enough space for update");
                return;
            }
        }

        if (Update.hasError()) return;

        if (Update.write(data, len) != len) {
            Update.printError(Serial);
            Update.abort();
            return;
        }

        if (final) {
            if (Update.end(true)) {
                Serial.printf("OTA update success: %u bytes\n", index + len);
            } else {
                Update.printError(Serial);
            }
        }
    });

    server.begin();
}
void setupWiFiAP()
{
    WiFi.softAP(apSSID, apPassword);
    DEBUG_PRINTLN("Access Point started. Connect to 'DeviceSetupAP'.");
    DEBUG_PRINT("IP address: ");
    DEBUG_PRINTLN(WiFi.softAPIP());
}

void connectToWiFiTask(void *pvParameters)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(targetSSID.c_str(), targetPassword.c_str());

    int retryCount = 0;
    while (WiFi.status() != WL_CONNECTED && retryCount < 10)
    {
        DEBUG_PRINTLN(".");
        retryCount++;
        delay(1000);
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        DEBUG_PRINTLN("\nConnected to Wi-Fi!");
        DEBUG_PRINT("IP address: ");
        DEBUG_PRINTLN(WiFi.localIP());

        apiURL = "http://" + targetHost + "/printer/afc/status";

        if (xTaskCreate(fetchDataTask, "Data Fetch Task", 4096, NULL, 8, &apiFetchTaskHandle) != pdPASS)
        {
            DEBUG_PRINTLN("Failed to create Data Fetch Task");
        }
    }
    else
    {
        DEBUG_PRINTLN("\nFailed to connect to Wi-Fi. Restarting AP mode.");
        setupWiFiAP();
    }

    vTaskDelete(NULL);
}

void loadCredentials()
{
    targetSSID = preferences.getString("ssid", "");
    targetPassword = preferences.getString("password", "");
    targetHost = preferences.getString("host", "");

    if (!targetSSID.isEmpty() && !targetPassword.isEmpty() && !targetHost.isEmpty())
    {
        DEBUG_PRINTLN("Loaded Wi-Fi credentials and target host from memory.");
        DEBUG_PRINT("SSID: ");
        DEBUG_PRINTLN(targetSSID); // Print SSID
        DEBUG_PRINT("Password: ");
        DEBUG_PRINTLN(targetPassword); // Print Password
        DEBUG_PRINT("Host: ");
        DEBUG_PRINTLN(targetHost);

        // Begin Wi-Fi connection
        WiFi.begin(targetSSID.c_str(), targetPassword.c_str());

        // Construct the API URL from the saved host
        apiURL = "http://" + targetHost + "/printer/afc/status";
        
        DEBUG_PRINTLN("API URL: " + apiURL); // Print API URL for debugging
    }
    else
    {
        DEBUG_PRINTLN("No saved Wi-Fi credentials or host found.");
    }
}

void saveCredentials(const String &ssid, const String &password, const String &host)
{
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    preferences.putString("host", host);
    DEBUG_PRINTLN("Wi-Fi credentials and host saved.");
}
