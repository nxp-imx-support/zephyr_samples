/*
* Copyright 2023 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct
{
  
	lv_obj_t *screen;
	bool screen_del;
	lv_obj_t *screen_background;
	lv_obj_t *screen_meter;
	lv_meter_indicator_t *screen_meter_scale_1_ndimg_0;
	lv_obj_t *screen_speed;
	lv_obj_t *screen_gauge_label_2;
	lv_obj_t *screen_hdr_img;
	lv_obj_t *screen_date_label;
	lv_obj_t *screen_time_label;
	lv_obj_t *screen_battery_icon;
	lv_obj_t *screen_battery_label;
	lv_obj_t *screen_sub_label;
	lv_obj_t *screen_drive_mode;
	lv_obj_t *screen_info_img_h;
	lv_obj_t *screen_speed_icon;
	lv_obj_t *screen_travelled_icon;
	lv_obj_t *screen_info2_label_1;
	lv_obj_t *screen_distance_value;
	lv_obj_t *screen_avg_speed;
	lv_obj_t *screen_label_14;
	lv_obj_t *screen_label_18;
	lv_obj_t *screen_label_19;
	lv_obj_t *screen_img_1;
	lv_obj_t *screen_label_0;
	lv_obj_t *screen_label_10;
	lv_obj_t *screen_label_20;
	lv_obj_t *screen_label_30;
	lv_obj_t *screen_label_40;
	lv_obj_t *screen_label_50;
}lv_ui;

void ui_init_style(lv_style_t * style);
void init_scr_del_flag(lv_ui *ui);
void setup_ui(lv_ui *ui);
extern lv_ui guider_ui;

void setup_scr_screen(lv_ui *ui);
LV_IMG_DECLARE(_bg1_alpha_1280x800);

LV_IMG_DECLARE(_dashboardnew_626x626);
LV_IMG_DECLARE(_tick_alpha_286x16);
LV_IMG_DECLARE(_ebike_header_bg_alpha_1280x125);
LV_IMG_DECLARE(_ebike_gauge_battery_alpha_40x35);
LV_IMG_DECLARE(_info_h_alpha_8x352);
LV_IMG_DECLARE(_ebike_icn_average_speed_big_alpha_61x50);
LV_IMG_DECLARE(_ebike_icn_distance_travelled_big_alpha_56x41);
LV_IMG_DECLARE(_nxp_alpha_183x66);

LV_FONT_DECLARE(lv_font_Amiko_Regular_1)
LV_FONT_DECLARE(lv_font_Amiko_Regular_12)
LV_FONT_DECLARE(lv_font_Amiko_Regular_112)
LV_FONT_DECLARE(lv_font_Amiko_Regular_36)
LV_FONT_DECLARE(lv_font_Amiko_Regular_32)
LV_FONT_DECLARE(lv_font_Amiko_Regular_48)
LV_FONT_DECLARE(lv_font_Amiko_Regular_64)


#ifdef __cplusplus
}
#endif
#endif
