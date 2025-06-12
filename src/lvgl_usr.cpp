#include "lvgl_usr.h"

void lv_popup_warning(const char * warning, bool clickable);
void lv_popup_remove(lv_event_t * e) ;
uint32_t lastLvglUpdate;
lv_color_t activeColor = lv_color_hex(0xab34eb);
lv_color_t loadedColor = lv_color_hex(0x3ca811);
lv_color_t unloadedColor = lv_color_hex(0xf29727);
lv_color_t buttonColor = lv_color_hex(0x2095F6);

int activeLane = -1;

const char* booleanHelper(bool value){
    const char* temp;
    switch(value){
        case true:
                temp = "True";
            break;
        case false:
                temp = "False";
            break;
    }
    return temp;
}

uint32_t lv_color_to_hex(lv_color_t color) {
    uint8_t red = color.ch.red;
    uint8_t green = (color.ch.green_h << 3) | color.ch.green_l;
    uint8_t blue = color.ch.blue;

    uint32_t hexValue = (red << 16) | (green << 8) | blue;

    return hexValue;
}

void lvgl_set_current_leg() {
   if (currentLoadBuffer != nullptr)activeLane = atoi(&currentLoadBuffer[3]);
}

void lvgl_set_tool_status(){
    const char *toolStatus = lv_label_get_text(ui_ToolStatus);
    if (strcmp(toolStatus, currentLoadBuffer) != 0){
        lv_label_set_text(ui_ToolStatus, booleanHelper(toolLoaded));
    }
}

void lvgl_set_hub_status(){
    const char *hubStatus = lv_label_get_text(ui_HubStatus);
    if (strcmp(hubStatus, booleanHelper(loadedToHub)) != 0){
        lv_label_set_text(ui_HubStatus, booleanHelper(loadedToHub));
    }
}

void lvgl_set_active_lane_color(){
    switch (activeLane)
                {
                    case 1:
                    lv_obj_set_style_bg_color(ui_Tool0Button, activeColor, LV_PART_MAIN);
                        break;
                    case 2:
                    lv_obj_set_style_bg_color(ui_Tool1Button, activeColor, LV_PART_MAIN);
                        break;
                    case 3:
                    lv_obj_set_style_bg_color(ui_Tool2Button, activeColor, LV_PART_MAIN);
                        break;
                    case 4:
                    lv_obj_set_style_bg_color(ui_Tool3Button, activeColor, LV_PART_MAIN);
                        break;
                    
                }
}

void lvgl_set_leg_status(){
    if(leg1Load){lv_obj_set_style_bg_color(ui_Tool0Button, loadedColor, LV_PART_MAIN);}
    else{lv_obj_set_style_bg_color(ui_Tool0Button, unloadedColor, LV_PART_MAIN);}
    if(leg2Load){lv_obj_set_style_bg_color(ui_Tool1Button, loadedColor, LV_PART_MAIN);}
    else{lv_obj_set_style_bg_color(ui_Tool1Button, unloadedColor, LV_PART_MAIN);}
    if(leg3Load){lv_obj_set_style_bg_color(ui_Tool2Button, loadedColor, LV_PART_MAIN);}
    else{lv_obj_set_style_bg_color(ui_Tool2Button, unloadedColor, LV_PART_MAIN);}
    if(leg4Load){lv_obj_set_style_bg_color(ui_Tool3Button, loadedColor, LV_PART_MAIN);}
    else{lv_obj_set_style_bg_color(ui_Tool3Button, unloadedColor, LV_PART_MAIN);}
    lvgl_set_active_lane_color();
}

void lvgl_set_nozzle_temp(){
    if(moonraker.data.nozzle_actual != atoi(lv_label_get_text(ui_CurrentNozzleTemp))){
        char buffer[12];
        sprintf(buffer, "%d", moonraker.data.nozzle_actual);
        lv_label_set_text(ui_CurrentNozzleTemp, buffer);
    }
}

void lvgl_set_bed_temp(){
    if(moonraker.data.nozzle_actual != atoi(lv_label_get_text(ui_CurrentBedTemp))){
        char buffer[12];
        sprintf(buffer, "%d", moonraker.data.bed_actual);
        lv_label_set_text(ui_CurrentBedTemp, buffer);
    }
}

void lvgl_toggle_current_print_panel() {
    if (ui_SwitchStatusPanel == nullptr || ui_Printstatus == nullptr) {
        Serial.println("Error: UI elements are not initialized properly.");
        return;
    }
    bool switchStatusHidden = lv_obj_has_flag(ui_SwitchStatusPanel, LV_OBJ_FLAG_HIDDEN);
    bool printStatusHidden = lv_obj_has_flag(ui_Printstatus, LV_OBJ_FLAG_HIDDEN);

    if (moonraker.data.printing) {
        if (printStatusHidden) {
            if (!switchStatusHidden) {
                lv_obj_add_flag(ui_SwitchStatusPanel, LV_OBJ_FLAG_HIDDEN);
            }
            lv_obj_clear_flag(ui_Printstatus, LV_OBJ_FLAG_HIDDEN);
        }
    } else {
        if (!printStatusHidden) {
            lv_obj_add_flag(ui_Printstatus, LV_OBJ_FLAG_HIDDEN);
        }
        lv_obj_clear_flag(ui_SwitchStatusPanel, LV_OBJ_FLAG_HIDDEN);
    }
}

void lvgl_update_print_progress() {
    if (moonraker.data.printing) {
        uint8_t progress = moonraker.data.progress > 100 ? 100 : moonraker.data.progress;

        char progressBuffer[5]; // Small stack buffer to hold progress value.
        snprintf(progressBuffer, sizeof(progressBuffer), "%u", progress);

        const char *currentLabel = lv_label_get_text(ui_PercentComplete);
        if (strcmp(currentLabel, progressBuffer) != 0) {
            lv_label_set_text(ui_PercentComplete, progressBuffer);
            lv_bar_set_value(ui_PrintProgressBar, progress, LV_ANIM_ON);
        }
    }
}


void lvgl_set_print_label(){
    if(moonraker.data.printing){
        lv_label_set_text(ui_Label3, moonraker.data.file_path);
    }
}

void lvgl_more_lanes_toggle_show(){
    if(numUnits > 1){

    }
}

void lvgl_set_params(){
        lvgl_set_current_leg();
        lvgl_set_tool_status();
        lvgl_set_hub_status();
        lvgl_set_leg_status();
        lvgl_set_nozzle_temp();
        lvgl_set_bed_temp();
        lvgl_toggle_current_print_panel();
        lvgl_update_print_progress();
        lvgl_set_print_label();
        lvgl_more_lanes_toggle_show();
}

void lvgl_ui_task(void * parameter) {
    lvgl_hal_init();
    ui_init();
       for(;;) {
        lvgl_set_params();
        lv_timer_handler();
        lastLvglUpdate = xTaskGetTickCount();
        lvglTaskHeartbeat = true;
        vTaskDelay(pdMS_TO_TICKS(10));
       }
}

