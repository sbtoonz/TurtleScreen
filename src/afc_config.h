#ifndef AFC_CONFIG_H
#define AFC_CONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "debug.hpp"

#define CONFIG_PATH "/afc_config.json"
#define MAX_CMD_LEN 32

struct AFCCommands {
    char tool_change[MAX_CMD_LEN];
    char tool_unload[MAX_CMD_LEN];
    char lane_unload[MAX_CMD_LEN];
    char prep[MAX_CMD_LEN];
    char cut[MAX_CMD_LEN];
    char brush[MAX_CMD_LEN];
    char kick[MAX_CMD_LEN];
    char park[MAX_CMD_LEN];
    char poop[MAX_CMD_LEN];
};

struct AFCFieldMap {
    char status_key[16];       // "status:" or "status"
    char lane_load[16];        // "load"
    char lane_prep[16];        // "prep"
    char lane_tool_loaded[16]; // "tool_loaded"
    char lane_hub_loaded[16];  // "loaded_to_hub"
    char lane_material[16];    // "material"
    char lane_color[16];       // "color"
    char lane_weight[16];      // "weight"
    char lane_spool_id[16];    // "spool_id"
    char lane_index[16];       // "lane"
    char lane_map[16];         // "map"
    char lane_name[16];        // "name"
    char sys_current_load[24]; // "current_load"
    char sys_num_units[16];    // "num_units"
    char sys_num_lanes[16];    // "num_lanes"
    char ext_tool_status[24];  // "tool_start_status"
    char unit_hub_loaded[16];  // "hub_loaded"
    char unit_type[16];        // "type"
};

struct AFCConfig {
    char endpoint[64];
    char config_url[128];
    AFCCommands commands;
    AFCFieldMap fields;
};

extern AFCConfig afcConfig;

void afc_config_init();
bool afc_config_load();
bool afc_config_save();
void afc_config_set_defaults();
String afc_config_to_json();
bool afc_config_from_json(const String &json);
bool afc_config_fetch_remote(const String &url);

#endif // AFC_CONFIG_H
