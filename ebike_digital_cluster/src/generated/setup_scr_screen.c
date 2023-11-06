/*
* Copyright 2023 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"


void setup_scr_screen(lv_ui *ui)
{
	//Write codes screen
	ui->screen = lv_obj_create(NULL);
	lv_obj_set_size(ui->screen, 1280, 800);
	lv_obj_set_scrollbar_mode(ui->screen, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cover_imgbtn
	ui->screen_cover_imgbtn = lv_imgbtn_create(ui->screen);
	lv_obj_add_flag(ui->screen_cover_imgbtn, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->screen_cover_imgbtn, LV_IMGBTN_STATE_RELEASED, NULL, &_bg1_alpha_1280x800, NULL);
	lv_imgbtn_set_src(ui->screen_cover_imgbtn, LV_IMGBTN_STATE_PRESSED, NULL, &_bg1_alpha_1280x800, NULL);
	lv_obj_add_flag(ui->screen_cover_imgbtn, LV_OBJ_FLAG_CHECKABLE);
	ui->screen_cover_imgbtn_label = lv_label_create(ui->screen_cover_imgbtn);
	lv_label_set_text(ui->screen_cover_imgbtn_label, "");
	lv_label_set_long_mode(ui->screen_cover_imgbtn_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_cover_imgbtn_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_cover_imgbtn, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_cover_imgbtn, 0, 0);
	lv_obj_set_size(ui->screen_cover_imgbtn, 1280, 800);
	lv_obj_set_scrollbar_mode(ui->screen_cover_imgbtn, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cover_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_cover_imgbtn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_cover_imgbtn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_cover_imgbtn, &lv_font_simsun_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_cover_imgbtn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cover_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_cover_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->screen_cover_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_cover_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->screen_cover_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->screen_cover_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for screen_cover_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->screen_cover_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_cover_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->screen_cover_imgbtn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->screen_cover_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write codes screen_meter
	ui->screen_meter = lv_meter_create(ui->screen);
	// add scale screen_meter_scale_1
	lv_meter_scale_t *screen_meter_scale_1 = lv_meter_add_scale(ui->screen_meter);
	lv_meter_set_scale_ticks(ui->screen_meter, screen_meter_scale_1, 57, 2, 10, lv_color_hex(0xa9a9a9));
	lv_meter_set_scale_major_ticks(ui->screen_meter, screen_meter_scale_1, 10, 5, 15, lv_color_hex(0xe1f3a2), 45);
	lv_meter_set_scale_range(ui->screen_meter, screen_meter_scale_1, 0, 56, 270, 135);

	// add arc for screen_meter_scale_1
	lv_meter_indicator_t *screen_meter_scale_1_arc_0;
	screen_meter_scale_1_arc_0 = lv_meter_add_arc(ui->screen_meter, screen_meter_scale_1, 4, lv_color_hex(0xffffff), 0);
	lv_meter_set_indicator_start_value(ui->screen_meter, screen_meter_scale_1_arc_0, 0);
	lv_meter_set_indicator_end_value(ui->screen_meter, screen_meter_scale_1_arc_0, 5);

	// add arc for screen_meter_scale_1
	lv_meter_indicator_t *screen_meter_scale_1_arc_1;
	screen_meter_scale_1_arc_1 = lv_meter_add_arc(ui->screen_meter, screen_meter_scale_1, 4, lv_color_hex(0x098D6B), 0);
	lv_meter_set_indicator_start_value(ui->screen_meter, screen_meter_scale_1_arc_1, 5);
	lv_meter_set_indicator_end_value(ui->screen_meter, screen_meter_scale_1_arc_1, 25);

	// add arc for screen_meter_scale_1
	lv_meter_indicator_t *screen_meter_scale_1_arc_2;
	screen_meter_scale_1_arc_2 = lv_meter_add_arc(ui->screen_meter, screen_meter_scale_1, 4, lv_color_hex(0xffff00), 0);
	lv_meter_set_indicator_start_value(ui->screen_meter, screen_meter_scale_1_arc_2, 25);
	lv_meter_set_indicator_end_value(ui->screen_meter, screen_meter_scale_1_arc_2, 40);

	// add arc for screen_meter_scale_1
	lv_meter_indicator_t *screen_meter_scale_1_arc_3;
	screen_meter_scale_1_arc_3 = lv_meter_add_arc(ui->screen_meter, screen_meter_scale_1, 4, lv_color_hex(0xff0000), 0);
	lv_meter_set_indicator_start_value(ui->screen_meter, screen_meter_scale_1_arc_3, 40);
	lv_meter_set_indicator_end_value(ui->screen_meter, screen_meter_scale_1_arc_3, 56);

	// add needle images for screen_meter_scale_1.
	ui->screen_meter_scale_1_ndimg_0 = lv_meter_add_needle_img(ui->screen_meter, screen_meter_scale_1, &_tick_alpha_286x5, 6, 6);
	lv_meter_set_indicator_value(ui->screen_meter, ui->screen_meter_scale_1_ndimg_0, 20);
	lv_obj_set_pos(ui->screen_meter, 8, 118);
	lv_obj_set_size(ui->screen_meter, 626, 626);
	lv_obj_set_scrollbar_mode(ui->screen_meter, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_meter, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_meter, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_meter, 200, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_meter, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_meter, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_img_src(ui->screen_meter, &_dashboardnew_626x626, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_img_opa(ui->screen_meter, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_meter, Part: LV_PART_TICKS, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_meter, lv_color_hex(0xffffff), LV_PART_TICKS|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_meter, &lv_font_montserratMedium_26, LV_PART_TICKS|LV_STATE_DEFAULT);

	//Write codes screen_speed
	ui->screen_speed = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_speed, "0");
	lv_label_set_long_mode(ui->screen_speed, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_speed, 193, 371);
	lv_obj_set_size(ui->screen_speed, 266, 117);
	lv_obj_set_scrollbar_mode(ui->screen_speed, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_speed, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_speed, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_speed, &lv_font_simsun_112, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_speed, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_speed, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_gauge_label_2
	ui->screen_gauge_label_2 = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_gauge_label_2, "km/h");
	lv_label_set_long_mode(ui->screen_gauge_label_2, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_gauge_label_2, 193, 484);
	lv_obj_set_size(ui->screen_gauge_label_2, 266, 49);
	lv_obj_set_scrollbar_mode(ui->screen_gauge_label_2, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_gauge_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_gauge_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_gauge_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_gauge_label_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_gauge_label_2, &lv_font_simsun_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_gauge_label_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_gauge_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_gauge_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_gauge_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_gauge_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_gauge_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_gauge_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_gauge_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_gauge_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_hdr_img
	ui->screen_hdr_img = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_hdr_img, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_hdr_img, &_ebike_header_bg_alpha_1280x125);
	lv_img_set_pivot(ui->screen_hdr_img, 0,0);
	lv_img_set_angle(ui->screen_hdr_img, 0);
	lv_obj_set_pos(ui->screen_hdr_img, 0, 0);
	lv_obj_set_size(ui->screen_hdr_img, 1280, 125);
	lv_obj_set_scrollbar_mode(ui->screen_hdr_img, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_hdr_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_hdr_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_date_label
	ui->screen_date_label = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_date_label, "May6,2023");
	lv_label_set_long_mode(ui->screen_date_label, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_date_label, 53, 14);
	lv_obj_set_size(ui->screen_date_label, 266, 94);
	lv_obj_set_scrollbar_mode(ui->screen_date_label, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_date_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_date_label, lv_color_hex(0xefefef), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_date_label, &lv_font_simsun_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_date_label, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_date_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_time_label
	ui->screen_time_label = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_time_label, "10:26 AM");
	lv_label_set_long_mode(ui->screen_time_label, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_time_label, 960, 14);
	lv_obj_set_size(ui->screen_time_label, 266, 94);
	lv_obj_set_scrollbar_mode(ui->screen_time_label, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_time_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_time_label, lv_color_hex(0xfffefe), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_time_label, &lv_font_simsun_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_time_label, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_time_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_battery_icon
	ui->screen_battery_icon = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_battery_icon, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_battery_icon, &_ebike_gauge_battery_alpha_40x35);
	lv_img_set_pivot(ui->screen_battery_icon, 0,0);
	lv_img_set_angle(ui->screen_battery_icon, 0);
	lv_obj_set_pos(ui->screen_battery_icon, 279, 554);
	lv_obj_set_size(ui->screen_battery_icon, 40, 35);
	lv_obj_set_scrollbar_mode(ui->screen_battery_icon, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_battery_icon, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_battery_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_battery_label
	ui->screen_battery_label = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_battery_label, "56%");
	lv_label_set_long_mode(ui->screen_battery_label, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_battery_label, 339, 554);
	lv_obj_set_size(ui->screen_battery_label, 133, 46);
	lv_obj_set_scrollbar_mode(ui->screen_battery_label, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_battery_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_battery_label, lv_color_hex(0x0080ff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_battery_label, &lv_font_simsun_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_battery_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_sub_label
	ui->screen_sub_label = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_sub_label, "OVERVIEW");
	lv_label_set_long_mode(ui->screen_sub_label, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_sub_label, 466, 14);
	lv_obj_set_size(ui->screen_sub_label, 346, 94);
	lv_obj_set_scrollbar_mode(ui->screen_sub_label, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_sub_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_sub_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_sub_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_sub_label, lv_color_hex(0xfdfdfd), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_sub_label, &lv_font_simsun_53, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_sub_label, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_sub_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_sub_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_sub_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_sub_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_sub_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_sub_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_sub_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_sub_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_drive_mode
	ui->screen_drive_mode = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_drive_mode, "Drive Mode");
	lv_label_set_long_mode(ui->screen_drive_mode, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_drive_mode, 732, 228);
	lv_obj_set_size(ui->screen_drive_mode, 386, 93);
	lv_obj_set_scrollbar_mode(ui->screen_drive_mode, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_drive_mode, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_drive_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_drive_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_drive_mode, lv_color_hex(0x00fff5), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_drive_mode, &lv_font_simsun_66, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_drive_mode, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_drive_mode, 26, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_drive_mode, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_drive_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_drive_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_drive_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_drive_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_drive_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_drive_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_info_img_h
	ui->screen_info_img_h = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_info_img_h, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_info_img_h, &_info_h_alpha_8x352);
	lv_img_set_pivot(ui->screen_info_img_h, 0,0);
	lv_img_set_angle(ui->screen_info_img_h, 0);
	lv_obj_set_pos(ui->screen_info_img_h, 945, 341);
	lv_obj_set_size(ui->screen_info_img_h, 8, 352);
	lv_obj_set_scrollbar_mode(ui->screen_info_img_h, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_info_img_h, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_info_img_h, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_speed_icon
	ui->screen_speed_icon = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_speed_icon, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_speed_icon, &_ebike_icn_average_speed_big_alpha_61x50);
	lv_img_set_pivot(ui->screen_speed_icon, 0,0);
	lv_img_set_angle(ui->screen_speed_icon, 0);
	lv_obj_set_pos(ui->screen_speed_icon, 988, 416);
	lv_obj_set_size(ui->screen_speed_icon, 61, 50);
	lv_obj_set_scrollbar_mode(ui->screen_speed_icon, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_speed_icon, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_speed_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_travelled_icon
	ui->screen_travelled_icon = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_travelled_icon, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_travelled_icon, &_ebike_icn_distance_travelled_big_alpha_56x41);
	lv_img_set_pivot(ui->screen_travelled_icon, 0,0);
	lv_img_set_angle(ui->screen_travelled_icon, 0);
	lv_obj_set_pos(ui->screen_travelled_icon, 682, 419);
	lv_obj_set_size(ui->screen_travelled_icon, 56, 41);
	lv_obj_set_scrollbar_mode(ui->screen_travelled_icon, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_travelled_icon, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_travelled_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_info2_label_1
	ui->screen_info2_label_1 = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_info2_label_1, "Average\nSpeed");
	lv_label_set_long_mode(ui->screen_info2_label_1, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_info2_label_1, 1082, 405);
	lv_obj_set_size(ui->screen_info2_label_1, 177, 72);
	lv_obj_set_scrollbar_mode(ui->screen_info2_label_1, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_info2_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_info2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_info2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_info2_label_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_info2_label_1, &lv_font_simsun_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_info2_label_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_info2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_info2_label_1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_info2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_info2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_info2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_info2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_info2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_info2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_distance_value
	ui->screen_distance_value = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_distance_value, "12.7");
	lv_label_set_long_mode(ui->screen_distance_value, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_distance_value, 706, 557);
	lv_obj_set_size(ui->screen_distance_value, 134, 78);
	lv_obj_set_scrollbar_mode(ui->screen_distance_value, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_distance_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_distance_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_distance_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_distance_value, lv_color_hex(0x0080ff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_distance_value, &lv_font_simsun_66, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_distance_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_distance_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_distance_value, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_distance_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_distance_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_distance_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_distance_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_distance_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_distance_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_avg_speed
	ui->screen_avg_speed = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_avg_speed, "16.1\n");
	lv_label_set_long_mode(ui->screen_avg_speed, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_avg_speed, 980, 555);
	lv_obj_set_size(ui->screen_avg_speed, 139, 71);
	lv_obj_set_scrollbar_mode(ui->screen_avg_speed, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_avg_speed, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_avg_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_avg_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_avg_speed, lv_color_hex(0x0080ff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_avg_speed, &lv_font_simsun_66, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_avg_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_avg_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_avg_speed, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_avg_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_avg_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_avg_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_avg_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_avg_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_avg_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_14
	ui->screen_label_14 = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_label_14, "km");
	lv_label_set_long_mode(ui->screen_label_14, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_14, 820, 580);
	lv_obj_set_size(ui->screen_label_14, 79, 63);
	lv_obj_set_scrollbar_mode(ui->screen_label_14, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_label_14, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_14, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_14, &lv_font_simsun_36, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_14, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_14, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_15
	ui->screen_label_15 = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_label_15, "km/h\n");
	lv_label_set_long_mode(ui->screen_label_15, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_15, 1129, 573);
	lv_obj_set_size(ui->screen_label_15, 102, 40);
	lv_obj_set_scrollbar_mode(ui->screen_label_15, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_label_15, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_label_15, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_15, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_15, &lv_font_simsun_36, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_15, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_15, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_16
	ui->screen_label_16 = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_label_16, "Travelled");
	lv_label_set_long_mode(ui->screen_label_16, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_16, 739, 439);
	lv_obj_set_size(ui->screen_label_16, 168, 45);
	lv_obj_set_scrollbar_mode(ui->screen_label_16, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_label_16, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_label_16, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_16, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_16, &lv_font_simsun_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_16, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_16, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_17
	ui->screen_label_17 = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_label_17, "Speed");
	lv_label_set_long_mode(ui->screen_label_17, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_17, 1060, 439);
	lv_obj_set_size(ui->screen_label_17, 177, 53);
	lv_obj_set_scrollbar_mode(ui->screen_label_17, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_label_17, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_label_17, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_17, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_17, &lv_font_simsun_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_17, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_17, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_18
	ui->screen_label_18 = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_label_18, "km/h");
	lv_label_set_long_mode(ui->screen_label_18, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_18, 1118, 580);
	lv_obj_set_size(ui->screen_label_18, 91, 55);
	lv_obj_set_scrollbar_mode(ui->screen_label_18, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_label_18, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_18, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_18, &lv_font_simsun_36, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_18, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_18, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_19
	ui->screen_label_19 = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_label_19, "Distance\nTravelled");
	lv_label_set_long_mode(ui->screen_label_19, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_19, 763, 405);
	lv_obj_set_size(ui->screen_label_19, 177, 72);
	lv_obj_set_scrollbar_mode(ui->screen_label_19, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_label_19, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_19, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_19, &lv_font_simsun_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_19, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_19, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_11
	ui->screen_label_11 = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_label_11, "Distance");
	lv_label_set_long_mode(ui->screen_label_11, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_11, 732, 394);
	lv_obj_set_size(ui->screen_label_11, 168, 45);
	lv_obj_set_scrollbar_mode(ui->screen_label_11, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_label_11, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_label_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_11, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_11, &lv_font_simsun_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_11, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_1
	ui->screen_img_1 = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_img_1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_1, &_nxp_alpha_183x66);
	lv_img_set_pivot(ui->screen_img_1, 50,50);
	lv_img_set_angle(ui->screen_img_1, 0);
	lv_obj_set_pos(ui->screen_img_1, 1026, 133);
	lv_obj_set_size(ui->screen_img_1, 183, 66);
	lv_obj_set_scrollbar_mode(ui->screen_img_1, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Update current screen layout.
	lv_obj_update_layout(ui->screen);

	
}
