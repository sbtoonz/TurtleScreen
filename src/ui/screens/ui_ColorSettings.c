// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.11
// Project name: BoxTurtle

#include "../ui.h"

void ui_ColorSettings_screen_init(void)
{
    ui_ColorSettings = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ColorSettings, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Panel6 = lv_obj_create(ui_ColorSettings);
    lv_obj_set_width(ui_Panel6, 550);
    lv_obj_set_height(ui_Panel6, 150);
    lv_obj_set_align(ui_Panel6, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel6, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Label1 = lv_label_create(ui_Panel6);
    lv_obj_set_width(ui_Label1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label1, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_Label1, "Set Color:");

    ui_SetActiveColor = lv_btn_create(ui_Panel6);
    lv_obj_set_width(ui_SetActiveColor, 100);
    lv_obj_set_height(ui_SetActiveColor, 50);
    lv_obj_set_align(ui_SetActiveColor, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_SetActiveColor, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_SetActiveColor, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    ui_object_set_themeable_style_property(ui_SetActiveColor, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR,
                                           _ui_theme_color_Blue);
    ui_object_set_themeable_style_property(ui_SetActiveColor, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA,
                                           _ui_theme_alpha_Blue);

    ui_Label5 = lv_label_create(ui_SetActiveColor);
    lv_obj_set_width(ui_Label5, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label5, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label5, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label5, "Active Color");

    ui_SetLoadedColor = lv_btn_create(ui_Panel6);
    lv_obj_set_width(ui_SetLoadedColor, 100);
    lv_obj_set_height(ui_SetLoadedColor, 50);
    lv_obj_set_x(ui_SetLoadedColor, -70);
    lv_obj_set_y(ui_SetLoadedColor, 0);
    lv_obj_set_align(ui_SetLoadedColor, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_SetLoadedColor, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_SetLoadedColor, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    ui_object_set_themeable_style_property(ui_SetLoadedColor, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR,
                                           _ui_theme_color_Blue);
    ui_object_set_themeable_style_property(ui_SetLoadedColor, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA,
                                           _ui_theme_alpha_Blue);

    ui_Label6 = lv_label_create(ui_SetLoadedColor);
    lv_obj_set_width(ui_Label6, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label6, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label6, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label6, "Loaded\nColor");
    lv_obj_set_style_text_align(ui_Label6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SetButtonColor = lv_btn_create(ui_Panel6);
    lv_obj_set_width(ui_SetButtonColor, 100);
    lv_obj_set_height(ui_SetButtonColor, 50);
    lv_obj_set_align(ui_SetButtonColor, LV_ALIGN_RIGHT_MID);
    lv_obj_add_flag(ui_SetButtonColor, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_SetButtonColor, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    ui_object_set_themeable_style_property(ui_SetButtonColor, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR,
                                           _ui_theme_color_Blue);
    ui_object_set_themeable_style_property(ui_SetButtonColor, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA,
                                           _ui_theme_alpha_Blue);

    ui_Label18 = lv_label_create(ui_SetButtonColor);
    lv_obj_set_width(ui_Label18, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label18, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label18, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label18, "Button\nColor");
    lv_obj_set_style_text_align(ui_Label18, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SetUnloadedColor = lv_btn_create(ui_Panel6);
    lv_obj_set_width(ui_SetUnloadedColor, 100);
    lv_obj_set_height(ui_SetUnloadedColor, 50);
    lv_obj_set_x(ui_SetUnloadedColor, 70);
    lv_obj_set_y(ui_SetUnloadedColor, 0);
    lv_obj_set_align(ui_SetUnloadedColor, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_SetUnloadedColor, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_SetUnloadedColor, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    ui_object_set_themeable_style_property(ui_SetUnloadedColor, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR,
                                           _ui_theme_color_Blue);
    ui_object_set_themeable_style_property(ui_SetUnloadedColor, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA,
                                           _ui_theme_alpha_Blue);

    ui_Label19 = lv_label_create(ui_SetUnloadedColor);
    lv_obj_set_width(ui_Label19, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label19, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label19, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label19, "Unloaded\nColor");
    lv_obj_set_style_text_align(ui_Label19, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_BackColorSettings = lv_btn_create(ui_Panel6);
    lv_obj_set_width(ui_BackColorSettings, 75);
    lv_obj_set_height(ui_BackColorSettings, 25);
    lv_obj_set_x(ui_BackColorSettings, 0);
    lv_obj_set_y(ui_BackColorSettings, 10);
    lv_obj_set_align(ui_BackColorSettings, LV_ALIGN_BOTTOM_MID);
    lv_obj_add_flag(ui_BackColorSettings, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_BackColorSettings, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    ui_object_set_themeable_style_property(ui_BackColorSettings, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR,
                                           _ui_theme_color_Blue);
    ui_object_set_themeable_style_property(ui_BackColorSettings, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA,
                                           _ui_theme_alpha_Blue);

    ui_Image8 = lv_img_create(ui_BackColorSettings);
    lv_img_set_src(ui_Image8, &ui_img_1470730272);
    lv_obj_set_width(ui_Image8, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Image8, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Image8, -28);
    lv_obj_set_y(ui_Image8, 1);
    lv_obj_set_align(ui_Image8, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image8, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image8, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Label25 = lv_label_create(ui_BackColorSettings);
    lv_obj_set_width(ui_Label25, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label25, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label25, 2);
    lv_obj_set_y(ui_Label25, 1);
    lv_obj_set_align(ui_Label25, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label25, "Back");

    lv_obj_add_event_cb(ui_SetActiveColor, ui_event_SetActiveColor, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_SetLoadedColor, ui_event_SetLoadedColor, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_SetButtonColor, ui_event_SetButtonColor, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_SetUnloadedColor, ui_event_SetUnloadedColor, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BackColorSettings, ui_event_BackColorSettings, LV_EVENT_ALL, NULL);

}