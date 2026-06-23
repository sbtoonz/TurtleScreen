#include "ui_functions.h"
#include "moonraker.h"
#include "api_fetch.h"
#include "afc_config.h"

lv_obj_t *gif_img;
lv_color_t color;

int colorChangeState = -1;
int selectedTool = -1;

void toolChangeCall(lv_event_t * e, int toolNo){
    char command[48];
    if (toolNo >= 0 && toolNo < numLanesFound && lanes[toolNo].map[0] != '\0') {
        snprintf(command, sizeof(command), "%s LANE=%s", afcConfig.commands.tool_change, lanes[toolNo].name);
    } else {
        snprintf(command, sizeof(command), "T%d", toolNo);
    }
    moonraker.post_gcode_to_queue(command);
}

void ejectLaneCall(lv_event_t * e, int toolNo){
    char command[48];
    if (toolNo >= 0 && toolNo < numLanesFound) {
        snprintf(command, sizeof(command), "%s LANE=%s", afcConfig.commands.lane_unload, lanes[toolNo].name);
    } else {
        snprintf(command, sizeof(command), "%s LANE=lane%d", afcConfig.commands.lane_unload, toolNo + 1);
    }
    moonraker.post_gcode_to_queue(command);
}

void afcBrushCall(lv_event_t * e){
    moonraker.post_gcode_to_queue(afcConfig.commands.brush);
}

void afcCutCall(lv_event_t * e){
    moonraker.post_gcode_to_queue(afcConfig.commands.cut);
}

void afcKickCall(lv_event_t * e){
    moonraker.post_gcode_to_queue(afcConfig.commands.kick);
}

void afcParkCall(lv_event_t * e){
    moonraker.post_gcode_to_queue(afcConfig.commands.park);
}

void afcPoopCall(lv_event_t * e){
    moonraker.post_gcode_to_queue(afcConfig.commands.poop);
}

void btPrepCall(lv_event_t * e){
    moonraker.post_gcode_to_queue(afcConfig.commands.prep);
}

void toolUnloadCall(lv_event_t * e){
    moonraker.post_gcode_to_queue(afcConfig.commands.tool_unload);
}

void setActiveColor(lv_event_t * e){
    color = lv_colorwheel_get_rgb(ui_Colorwheel1);
    if(activeColor.full != color.full) activeColor = color;
}

void setLoadedColor(lv_event_t * e){
    color = lv_colorwheel_get_rgb(ui_Colorwheel1);
    if(loadedColor.full != color.full) loadedColor = color;

}

void setUnloadedColor(lv_event_t * e){
    color = lv_colorwheel_get_rgb(ui_Colorwheel1);
    if(unloadedColor.full != color.full) unloadedColor = color;
}

void setButtonColor(lv_event_t * e) {

    color = lv_colorwheel_get_rgb(ui_Colorwheel1);
    if(buttonColor.full != color.full) buttonColor = color;
    if(_ui_theme_color_Blue[1] != lv_color_to_hex(color)) _ui_theme_color_Blue[1] =lv_color_to_hex(color);
    ui_theme_set(1);
}

void saveColorWheel(lv_event_t * e){
    color = lv_colorwheel_get_rgb(ui_Colorwheel1);
}

void SetLaneActive(lv_event_t * e, int laneActive){
    char command[48];
    if (laneActive >= 0 && laneActive < numLanesFound) {
        snprintf(command, sizeof(command), "%s LANE=%s", afcConfig.commands.tool_change, lanes[laneActive].name);
    } else {
        snprintf(command, sizeof(command), "T%d", laneActive);
    }
    moonraker.post_gcode_to_queue(command);
}

void EjectLane(lv_event_t * e, int laneEject){
    char command[48];
    if (laneEject >= 0 && laneEject < numLanesFound) {
        snprintf(command, sizeof(command), "%s LANE=%s", afcConfig.commands.lane_unload, lanes[laneEject].name);
    } else {
        snprintf(command, sizeof(command), "%s LANE=lane%d", afcConfig.commands.lane_unload, laneEject + 1);
    }
    moonraker.post_gcode_to_queue(command);
}
