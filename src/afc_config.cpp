#include "afc_config.h"
#include <SPIFFS.h>
#include <HTTPClient.h>

AFCConfig afcConfig;

void afc_config_set_defaults() {
    strncpy(afcConfig.endpoint, "/printer/afc/status", sizeof(afcConfig.endpoint));
    afcConfig.config_url[0] = '\0';

    // Commands
    strncpy(afcConfig.commands.tool_change, "CHANGE_TOOL", MAX_CMD_LEN);
    strncpy(afcConfig.commands.tool_unload, "TOOL_UNLOAD", MAX_CMD_LEN);
    strncpy(afcConfig.commands.lane_unload, "LANE_UNLOAD", MAX_CMD_LEN);
    strncpy(afcConfig.commands.prep, "PREP", MAX_CMD_LEN);
    strncpy(afcConfig.commands.cut, "AFC_CUT", MAX_CMD_LEN);
    strncpy(afcConfig.commands.brush, "AFC_BRUSH", MAX_CMD_LEN);
    strncpy(afcConfig.commands.kick, "AFC_KICK", MAX_CMD_LEN);
    strncpy(afcConfig.commands.park, "AFC_PARK", MAX_CMD_LEN);
    strncpy(afcConfig.commands.poop, "AFC_POOP", MAX_CMD_LEN);

    // Field mappings
    strncpy(afcConfig.fields.status_key, "status:", sizeof(afcConfig.fields.status_key));
    strncpy(afcConfig.fields.lane_load, "load", sizeof(afcConfig.fields.lane_load));
    strncpy(afcConfig.fields.lane_prep, "prep", sizeof(afcConfig.fields.lane_prep));
    strncpy(afcConfig.fields.lane_tool_loaded, "tool_loaded", sizeof(afcConfig.fields.lane_tool_loaded));
    strncpy(afcConfig.fields.lane_hub_loaded, "loaded_to_hub", sizeof(afcConfig.fields.lane_hub_loaded));
    strncpy(afcConfig.fields.lane_material, "material", sizeof(afcConfig.fields.lane_material));
    strncpy(afcConfig.fields.lane_color, "color", sizeof(afcConfig.fields.lane_color));
    strncpy(afcConfig.fields.lane_weight, "weight", sizeof(afcConfig.fields.lane_weight));
    strncpy(afcConfig.fields.lane_spool_id, "spool_id", sizeof(afcConfig.fields.lane_spool_id));
    strncpy(afcConfig.fields.lane_index, "lane", sizeof(afcConfig.fields.lane_index));
    strncpy(afcConfig.fields.lane_map, "map", sizeof(afcConfig.fields.lane_map));
    strncpy(afcConfig.fields.lane_name, "name", sizeof(afcConfig.fields.lane_name));
    strncpy(afcConfig.fields.sys_current_load, "current_load", sizeof(afcConfig.fields.sys_current_load));
    strncpy(afcConfig.fields.sys_num_units, "num_units", sizeof(afcConfig.fields.sys_num_units));
    strncpy(afcConfig.fields.sys_num_lanes, "num_lanes", sizeof(afcConfig.fields.sys_num_lanes));
    strncpy(afcConfig.fields.ext_tool_status, "tool_start_status", sizeof(afcConfig.fields.ext_tool_status));
    strncpy(afcConfig.fields.unit_hub_loaded, "hub_loaded", sizeof(afcConfig.fields.unit_hub_loaded));
    strncpy(afcConfig.fields.unit_type, "type", sizeof(afcConfig.fields.unit_type));
}

void afc_config_init() {
    if (!SPIFFS.begin(true)) {
        DEBUG_PRINTLN("SPIFFS mount failed, using defaults");
        afc_config_set_defaults();
        return;
    }
    if (!afc_config_load()) {
        DEBUG_PRINTLN("No config found, writing defaults");
        afc_config_set_defaults();
        afc_config_save();
    }
}

bool afc_config_load() {
    if (!SPIFFS.exists(CONFIG_PATH)) return false;

    File file = SPIFFS.open(CONFIG_PATH, "r");
    if (!file) return false;

    String json = file.readString();
    file.close();

    return afc_config_from_json(json);
}

bool afc_config_save() {
    String json = afc_config_to_json();

    File file = SPIFFS.open(CONFIG_PATH, "w");
    if (!file) {
        DEBUG_PRINTLN("Failed to open config for writing");
        return false;
    }
    file.print(json);
    file.close();
    DEBUG_PRINTLN("Config saved to SPIFFS");
    return true;
}

String afc_config_to_json() {
    DynamicJsonDocument doc(2048);

    doc["endpoint"] = afcConfig.endpoint;
    doc["config_url"] = afcConfig.config_url;

    JsonObject cmds = doc.createNestedObject("commands");
    cmds["tool_change"] = afcConfig.commands.tool_change;
    cmds["tool_unload"] = afcConfig.commands.tool_unload;
    cmds["lane_unload"] = afcConfig.commands.lane_unload;
    cmds["prep"] = afcConfig.commands.prep;
    cmds["cut"] = afcConfig.commands.cut;
    cmds["brush"] = afcConfig.commands.brush;
    cmds["kick"] = afcConfig.commands.kick;
    cmds["park"] = afcConfig.commands.park;
    cmds["poop"] = afcConfig.commands.poop;

    JsonObject flds = doc.createNestedObject("fields");
    flds["status_key"] = afcConfig.fields.status_key;
    flds["lane_load"] = afcConfig.fields.lane_load;
    flds["lane_prep"] = afcConfig.fields.lane_prep;
    flds["lane_tool_loaded"] = afcConfig.fields.lane_tool_loaded;
    flds["lane_hub_loaded"] = afcConfig.fields.lane_hub_loaded;
    flds["lane_material"] = afcConfig.fields.lane_material;
    flds["lane_color"] = afcConfig.fields.lane_color;
    flds["lane_weight"] = afcConfig.fields.lane_weight;
    flds["lane_spool_id"] = afcConfig.fields.lane_spool_id;
    flds["lane_index"] = afcConfig.fields.lane_index;
    flds["lane_map"] = afcConfig.fields.lane_map;
    flds["lane_name"] = afcConfig.fields.lane_name;
    flds["sys_current_load"] = afcConfig.fields.sys_current_load;
    flds["sys_num_units"] = afcConfig.fields.sys_num_units;
    flds["sys_num_lanes"] = afcConfig.fields.sys_num_lanes;
    flds["ext_tool_status"] = afcConfig.fields.ext_tool_status;
    flds["unit_hub_loaded"] = afcConfig.fields.unit_hub_loaded;
    flds["unit_type"] = afcConfig.fields.unit_type;

    String output;
    serializeJsonPretty(doc, output);
    return output;
}

bool afc_config_from_json(const String &json) {
    DynamicJsonDocument doc(2048);
    DeserializationError err = deserializeJson(doc, json);
    if (err) {
        DEBUG_PRINT("Config parse error: ");
        DEBUG_PRINTLN(err.f_str());
        return false;
    }

    if (doc.containsKey("endpoint"))
        strncpy(afcConfig.endpoint, doc["endpoint"] | "/printer/afc/status", sizeof(afcConfig.endpoint));
    if (doc.containsKey("config_url"))
        strncpy(afcConfig.config_url, doc["config_url"] | "", sizeof(afcConfig.config_url));

    JsonObject cmds = doc["commands"];
    if (!cmds.isNull()) {
        if (cmds.containsKey("tool_change"))  strncpy(afcConfig.commands.tool_change, cmds["tool_change"], MAX_CMD_LEN);
        if (cmds.containsKey("tool_unload")) strncpy(afcConfig.commands.tool_unload, cmds["tool_unload"], MAX_CMD_LEN);
        if (cmds.containsKey("lane_unload")) strncpy(afcConfig.commands.lane_unload, cmds["lane_unload"], MAX_CMD_LEN);
        if (cmds.containsKey("prep"))        strncpy(afcConfig.commands.prep, cmds["prep"], MAX_CMD_LEN);
        if (cmds.containsKey("cut"))         strncpy(afcConfig.commands.cut, cmds["cut"], MAX_CMD_LEN);
        if (cmds.containsKey("brush"))       strncpy(afcConfig.commands.brush, cmds["brush"], MAX_CMD_LEN);
        if (cmds.containsKey("kick"))        strncpy(afcConfig.commands.kick, cmds["kick"], MAX_CMD_LEN);
        if (cmds.containsKey("park"))        strncpy(afcConfig.commands.park, cmds["park"], MAX_CMD_LEN);
        if (cmds.containsKey("poop"))        strncpy(afcConfig.commands.poop, cmds["poop"], MAX_CMD_LEN);
    }

    JsonObject flds = doc["fields"];
    if (!flds.isNull()) {
        if (flds.containsKey("status_key"))      strncpy(afcConfig.fields.status_key, flds["status_key"], sizeof(afcConfig.fields.status_key));
        if (flds.containsKey("lane_load"))       strncpy(afcConfig.fields.lane_load, flds["lane_load"], sizeof(afcConfig.fields.lane_load));
        if (flds.containsKey("lane_prep"))       strncpy(afcConfig.fields.lane_prep, flds["lane_prep"], sizeof(afcConfig.fields.lane_prep));
        if (flds.containsKey("lane_tool_loaded")) strncpy(afcConfig.fields.lane_tool_loaded, flds["lane_tool_loaded"], sizeof(afcConfig.fields.lane_tool_loaded));
        if (flds.containsKey("lane_hub_loaded")) strncpy(afcConfig.fields.lane_hub_loaded, flds["lane_hub_loaded"], sizeof(afcConfig.fields.lane_hub_loaded));
        if (flds.containsKey("lane_material"))   strncpy(afcConfig.fields.lane_material, flds["lane_material"], sizeof(afcConfig.fields.lane_material));
        if (flds.containsKey("lane_color"))      strncpy(afcConfig.fields.lane_color, flds["lane_color"], sizeof(afcConfig.fields.lane_color));
        if (flds.containsKey("lane_weight"))     strncpy(afcConfig.fields.lane_weight, flds["lane_weight"], sizeof(afcConfig.fields.lane_weight));
        if (flds.containsKey("lane_spool_id"))   strncpy(afcConfig.fields.lane_spool_id, flds["lane_spool_id"], sizeof(afcConfig.fields.lane_spool_id));
        if (flds.containsKey("lane_index"))      strncpy(afcConfig.fields.lane_index, flds["lane_index"], sizeof(afcConfig.fields.lane_index));
        if (flds.containsKey("lane_map"))        strncpy(afcConfig.fields.lane_map, flds["lane_map"], sizeof(afcConfig.fields.lane_map));
        if (flds.containsKey("lane_name"))       strncpy(afcConfig.fields.lane_name, flds["lane_name"], sizeof(afcConfig.fields.lane_name));
        if (flds.containsKey("sys_current_load")) strncpy(afcConfig.fields.sys_current_load, flds["sys_current_load"], sizeof(afcConfig.fields.sys_current_load));
        if (flds.containsKey("sys_num_units"))   strncpy(afcConfig.fields.sys_num_units, flds["sys_num_units"], sizeof(afcConfig.fields.sys_num_units));
        if (flds.containsKey("sys_num_lanes"))   strncpy(afcConfig.fields.sys_num_lanes, flds["sys_num_lanes"], sizeof(afcConfig.fields.sys_num_lanes));
        if (flds.containsKey("ext_tool_status")) strncpy(afcConfig.fields.ext_tool_status, flds["ext_tool_status"], sizeof(afcConfig.fields.ext_tool_status));
        if (flds.containsKey("unit_hub_loaded")) strncpy(afcConfig.fields.unit_hub_loaded, flds["unit_hub_loaded"], sizeof(afcConfig.fields.unit_hub_loaded));
        if (flds.containsKey("unit_type"))       strncpy(afcConfig.fields.unit_type, flds["unit_type"], sizeof(afcConfig.fields.unit_type));
    }

    return true;
}

bool afc_config_fetch_remote(const String &url) {
    if (url.isEmpty()) return false;

    HTTPClient http;
    http.begin(url);
    http.setTimeout(10000);
    int code = http.GET();

    if (code == 200) {
        String payload = http.getString();
        http.end();
        if (afc_config_from_json(payload)) {
            strncpy(afcConfig.config_url, url.c_str(), sizeof(afcConfig.config_url) - 1);
            afc_config_save();
            DEBUG_PRINTLN("Remote config loaded and saved");
            return true;
        }
        DEBUG_PRINTLN("Remote config parse failed");
        return false;
    }

    DEBUG_PRINT("Remote config fetch failed: HTTP ");
    DEBUG_PRINTLN(code);
    http.end();
    return false;
}
