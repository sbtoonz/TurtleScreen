#include <lvgl.h>
#include "ui/ui.h"

#ifndef _UI_FUNCTIONS_H
#define _UI_FUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

extern lv_color_t color;
extern int colorChangeState;
extern int selectedTool;

void toolChangeCall(lv_event_t * e, int toolNo);
void ejectLaneCall(lv_event_t * e, int toolNo);
void afcBrushCall(lv_event_t * e);
void afcCutCall(lv_event_t * e);
void afcKickCall(lv_event_t * e);
void afcParkCall(lv_event_t * e);
void afcPoopCall(lv_event_t * e);
void btPrepCall(lv_event_t * e);
void toolUnloadCall(lv_event_t * e);
void setActiveColor(lv_event_t * e);
void setLoadedColor(lv_event_t * e);
void setUnloadedColor(lv_event_t * e);
void setButtonColor(lv_event_t * e);
void saveColorWheel(lv_event_t * e);

void SetLaneActive(lv_event_t * e, int activeLane);
void EjectLane(lv_event_t * e, int ejectLane);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif