#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Preferences.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>
#include "api_fetch.h"
#include <Update.h>
#include "lvgl_usr.h"
#include "web_page.h"
#include "watchdog.h"

extern const char* apSSID;
extern const char* apPassword;
extern Preferences preferences;
extern AsyncWebServer server;
extern TaskHandle_t WifITaskHandle;

void setupWiFiAP();
void setupWebSite();
void connectToWiFiTask(void *pvParameters);
void loadCredentials();
void saveCredentials(const String& ssid, const String& password, const String& host);

extern String targetSSID;
extern String targetPassword;
extern String targetHost;
extern String apiURL;

#endif // WIFI_MANAGER_H