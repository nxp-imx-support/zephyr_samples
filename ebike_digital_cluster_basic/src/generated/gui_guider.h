/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
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
  
	lv_obj_t *Overview;
	bool Overview_del;
	lv_obj_t *Overview_cover_imgbtn;
	lv_obj_t *Overview_cover_imgbtn_label;
	lv_obj_t *Overview_bg_img;
	lv_obj_t *Overview_hdr_img;
	lv_obj_t *Overview_date_label;
	lv_obj_t *Overview_time_label;
	lv_obj_t *Overview_sub_label;
	lv_obj_t *Overview_gps_icon;
	lv_obj_t *Overview_travelled_icon;
	lv_obj_t *Overview_speed_icon;
	lv_obj_t *Overview_remaining_icon;
	lv_obj_t *Overview_elapsed_icon;
	lv_obj_t *Overview_dist_label;
	lv_obj_t *Overview_dir_label;
	lv_obj_t *Overview_str_label;
	lv_obj_t *Overview_info1_label_1;
	lv_obj_t *Overview_info2_label_1;
	lv_obj_t *Overview_info3_label_1;
	lv_obj_t *Overview_linfo4_label_1;
	lv_obj_t *Overview_info1_label_3;
	lv_obj_t *Overview_info2_label_3;
	lv_obj_t *Overview_info3_label_3;
	lv_obj_t *Overview_info4_label_3;
	lv_obj_t *Overview_info1_label_4;
	lv_obj_t *Overview_info2_label_4;
	lv_obj_t *Overview_info3_label_4;
	lv_obj_t *Overview_info_img_v;
	lv_obj_t *Overview_info_img_h;
	lv_obj_t *Overview_gauge;
	lv_obj_t *Overview_needle_img;
	lv_obj_t *Overview_battery_line_img;
	lv_obj_t *Overview_battery_icon;
	lv_obj_t *Overview_battery_label;
	lv_obj_t *Overview_battery_arc;
	lv_obj_t *Overview_gauge_label_1;
	lv_obj_t *Overview_gauge_label_2;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_screen_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, uint32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                  uint32_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                  lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_completed_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_bottom_layer(void);

void setup_ui(lv_ui *ui);

void video_play(lv_ui *ui);

void init_keyboard(lv_ui *ui);

extern lv_ui guider_ui;


void setup_scr_Overview(lv_ui *ui);
LV_IMAGE_DECLARE(_bg1_ARGB8888_1024x600);
LV_IMAGE_DECLARE(_ebike_header_bg_ARGB8888_1024x135);
LV_IMAGE_DECLARE(_ebike_gps_arrow_1_ARGB8888_55x106);
LV_IMAGE_DECLARE(_ebike_icn_distance_travelled_ARGB8888_43x37);
LV_IMAGE_DECLARE(_ebike_icn_average_speed_ARGB8888_47x45);
LV_IMAGE_DECLARE(_ebike_icn_remaining_distance_ARGB8888_41x42);
LV_IMAGE_DECLARE(_ebike_icn_elapsed_time_2_ARGB8888_51x58);
LV_IMAGE_DECLARE(_info_h_ARGB8888_400x4);
LV_IMAGE_DECLARE(_info_v_ARGB8888_4x302);
LV_IMAGE_DECLARE(_ebike_gauge_ARGB8888_400x400);
LV_IMAGE_DECLARE(_ebike_gauge_needle_ARGB8888_320x320);
LV_IMAGE_DECLARE(_ebike_gauge_battery_line_ARGB8888_400x200);
LV_IMAGE_DECLARE(_ebike_gauge_battery_ARGB8888_38x33);

LV_FONT_DECLARE(lv_font_simsun_32)
LV_FONT_DECLARE(lv_font_simsun_42)
LV_FONT_DECLARE(lv_font_montserratMedium_30)
LV_FONT_DECLARE(lv_font_montserratMedium_49)
LV_FONT_DECLARE(lv_font_simsun_53)
LV_FONT_DECLARE(lv_font_montserratMedium_55)
LV_FONT_DECLARE(lv_font_simsun_74)
LV_FONT_DECLARE(lv_font_montserratMedium_26)
LV_FONT_DECLARE(lv_font_montserratMedium_45)
LV_FONT_DECLARE(lv_font_montserratMedium_28)
LV_FONT_DECLARE(lv_font_montserratMedium_75)
LV_FONT_DECLARE(lv_font_simsun_112)


#ifdef __cplusplus
}
#endif
#endif
