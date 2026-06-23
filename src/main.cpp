#include <Preferences.h>
#include "wifi_manager.h"
#include "api_fetch.h"
#include "web_page.h"
#include "lvgl_usr.h"
#include "moonraker.h"
#include "watchdog.h"
#include "afc_config.h"
#include <Update.h>
#include <esp_task_wdt.h>

TaskHandle_t lvglUiTaskHandle = NULL;
TaskHandle_t apiFetchTaskHandle = NULL;

void setup()
{
    Serial.begin(115200);
    esp_task_wdt_init(30, true);
    delay(1000);
    Serial.println("Starting up... \n");

    afc_config_init();

    preferences.begin("wifi-config", false);
    loadCredentials();

    if (!targetSSID.isEmpty() && !targetPassword.isEmpty())
    {
        Serial.println("Connecting to saved Wi-Fi network");
        if (xTaskCreate(connectToWiFiTask, "WiFi Connect Task", 4096, NULL, 8, NULL) != pdPASS)
        {
            Serial.println("Failed to create WiFi Connect Task");
        }
        xTaskCreate(lvgl_ui_task, "LVGL Init Task", 8192, NULL, 10, &lvglUiTaskHandle);
        xTaskCreate(moonraker_post_task, "moonraker post", 4096, NULL, 7, &postTaskHandle);
    }
    else
    {
        setupWiFiAP();
        xTaskCreate(lvgl_ui_task, "LVGL Init Task", 4096, NULL, 10, &lvglUiTaskHandle);
    }
    setupWebSite();
}

void loop()
{
}