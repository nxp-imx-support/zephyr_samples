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


void setup_scr_scanner(lv_ui *ui)
{
	//Write codes scanner
	ui->scanner = lv_obj_create(NULL);
	lv_obj_set_size(ui->scanner, 1280, 800);

	//Write style for scanner, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->scanner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes scanner_text_results
	ui->scanner_text_results = lv_textarea_create(ui->scanner);
	lv_textarea_set_text(ui->scanner_text_results, "Put barcode scan result here");
	lv_textarea_set_password_bullet(ui->scanner_text_results, "*");
	lv_textarea_set_password_mode(ui->scanner_text_results, false);
	lv_textarea_set_one_line(ui->scanner_text_results, false);
	lv_obj_set_pos(ui->scanner_text_results, 890, 40);
	lv_obj_set_size(ui->scanner_text_results, 350, 720);

	//Write style for scanner_text_results, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->scanner_text_results, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->scanner_text_results, &lv_font_simsun_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->scanner_text_results, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->scanner_text_results, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->scanner_text_results, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->scanner_text_results, lv_color_hex(0xb0b0b0), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->scanner_text_results, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->scanner_text_results, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->scanner_text_results, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->scanner_text_results, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->scanner_text_results, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->scanner_text_results, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->scanner_text_results, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->scanner_text_results, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->scanner_text_results, 4, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes scanner_image_camera_preview
	ui->scanner_image_camera_preview = lv_img_create(ui->scanner);
	lv_obj_add_flag(ui->scanner_image_camera_preview, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->scanner_image_camera_preview, &_qr_code_image_alpha_810x720);
	lv_img_set_pivot(ui->scanner_image_camera_preview, 50,50);
	lv_img_set_angle(ui->scanner_image_camera_preview, 0);
	lv_obj_set_pos(ui->scanner_image_camera_preview, 40, 40);
	lv_obj_set_size(ui->scanner_image_camera_preview, 810, 720);

	//Write style for scanner_image_camera_preview, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->scanner_image_camera_preview, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Update current screen layout.
	lv_obj_update_layout(ui->scanner);

	
}
