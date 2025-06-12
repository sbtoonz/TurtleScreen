#include "ui_functions.h"
#include "moonraker.h"

lv_obj_t *gif_img;
lv_color_t color;

int colorChangeState = -1;
int selectedTool = -1;

void toolChangeCall(lv_event_t * e, int toolNo){
    char command[10];
    snprintf(command, sizeof(command), "T%d", toolNo);
    moonraker.post_gcode_to_queue(command);
}

void ejectLaneCall(lv_event_t * e, int toolNo){
    char command[10];
    snprintf(command, sizeof(command), "t%d", toolNo);
    moonraker.post_gcode_to_queue(command);
}

void afcBrushCall(lv_event_t * e){
    moonraker.post_gcode_to_queue("AFC_BRUSH");
}

void afcCutCall(lv_event_t * e){
    moonraker.post_gcode_to_queue("AFC_CUT");
}

void afcKickCall(lv_event_t * e){
    moonraker.post_gcode_to_queue("AFC_KICK");
}

void afcParkCall(lv_event_t * e){
    moonraker.post_gcode_to_queue("AFC_PARK");
}

void afcPoopCall(lv_event_t * e){
    moonraker.post_gcode_to_queue("AFC_POOP");
}

void btPrepCall(lv_event_t * e){
    moonraker.post_gcode_to_queue("BT_PREP");
}

void toolUnloadCall(lv_event_t * e){
    moonraker.post_gcode_to_queue("BT_TOOL_UNLOAD");
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
    char command[10];
    snprintf(command, sizeof(command), "t%d", laneActive);
    moonraker.post_gcode_to_queue(command);
}

void EjectLane(lv_event_t * e, int laneEject){
    char command[20];
    snprintf(command, sizeof(command), "BT_LANE_EJECT LANE=%d", laneEject);
    moonraker.post_gcode_to_queue(command);
    
}
