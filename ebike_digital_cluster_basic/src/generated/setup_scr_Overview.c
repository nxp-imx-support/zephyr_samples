/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
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



void setup_scr_Overview(lv_ui *ui)
{
    //Write codes Overview
    ui->Overview = lv_obj_create(NULL);
    lv_obj_set_size(ui->Overview, 1024, 600);
    lv_obj_set_scrollbar_mode(ui->Overview, LV_SCROLLBAR_MODE_OFF);

    //Write style for Overview, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Overview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_cover_imgbtn
    ui->Overview_cover_imgbtn = lv_imagebutton_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_cover_imgbtn, 0, 0);
    lv_obj_set_size(ui->Overview_cover_imgbtn, 1280, 800);
    lv_obj_add_flag(ui->Overview_cover_imgbtn, LV_OBJ_FLAG_HIDDEN);
    ui->Overview_cover_imgbtn_label = lv_label_create(ui->Overview_cover_imgbtn);
    lv_label_set_text(ui->Overview_cover_imgbtn_label, "");
    lv_label_set_long_mode(ui->Overview_cover_imgbtn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->Overview_cover_imgbtn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->Overview_cover_imgbtn, 0, LV_STATE_DEFAULT);

    //Write style for Overview_cover_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->Overview_cover_imgbtn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Overview_cover_imgbtn, &lv_font_simsun_32, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Overview_cover_imgbtn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Overview_cover_imgbtn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Overview_cover_imgbtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for Overview_cover_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_image_recolor_opa(ui->Overview_cover_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_image_opa(ui->Overview_cover_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->Overview_cover_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->Overview_cover_imgbtn, &lv_font_simsun_42, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->Overview_cover_imgbtn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->Overview_cover_imgbtn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for Overview_cover_imgbtn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_image_recolor_opa(ui->Overview_cover_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_image_opa(ui->Overview_cover_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->Overview_cover_imgbtn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->Overview_cover_imgbtn, &lv_font_simsun_42, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->Overview_cover_imgbtn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->Overview_cover_imgbtn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for Overview_cover_imgbtn, Part: LV_PART_MAIN, State: LV_IMAGEBUTTON_STATE_RELEASED.
    lv_obj_set_style_image_recolor_opa(ui->Overview_cover_imgbtn, 0, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);
    lv_obj_set_style_image_opa(ui->Overview_cover_imgbtn, 255, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);

    //Write codes Overview_bg_img
    ui->Overview_bg_img = lv_image_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_bg_img, 0, 0);
    lv_obj_set_size(ui->Overview_bg_img, 1024, 600);
    lv_obj_add_flag(ui->Overview_bg_img, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->Overview_bg_img, &_bg1_ARGB8888_1024x600);
    lv_image_set_pivot(ui->Overview_bg_img, 0,0);
    lv_image_set_rotation(ui->Overview_bg_img, 0);

    //Write style for Overview_bg_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->Overview_bg_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->Overview_bg_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_hdr_img
    ui->Overview_hdr_img = lv_image_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_hdr_img, 0, 0);
    lv_obj_set_size(ui->Overview_hdr_img, 1024, 135);
    lv_obj_add_flag(ui->Overview_hdr_img, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->Overview_hdr_img, &_ebike_header_bg_ARGB8888_1024x135);
    lv_image_set_pivot(ui->Overview_hdr_img, 0,0);
    lv_image_set_rotation(ui->Overview_hdr_img, 0);

    //Write style for Overview_hdr_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->Overview_hdr_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->Overview_hdr_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_date_label
    ui->Overview_date_label = lv_label_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_date_label, 30, 10);
    lv_obj_set_size(ui->Overview_date_label, 209, 34);
    lv_label_set_text(ui->Overview_date_label, "May 5, 2025");
    lv_label_set_long_mode(ui->Overview_date_label, LV_LABEL_LONG_WRAP);

    //Write style for Overview_date_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Overview_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Overview_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Overview_date_label, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Overview_date_label, &lv_font_montserratMedium_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Overview_date_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Overview_date_label, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Overview_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Overview_date_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Overview_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Overview_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Overview_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Overview_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Overview_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Overview_date_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_time_label
    ui->Overview_time_label = lv_label_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_time_label, 850, 10);
    lv_obj_set_size(ui->Overview_time_label, 156, 34);
    lv_label_set_text(ui->Overview_time_label, "10:28 AM");
    lv_label_set_long_mode(ui->Overview_time_label, LV_LABEL_LONG_WRAP);

    //Write style for Overview_time_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Overview_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Overview_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Overview_time_label, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Overview_time_label, &lv_font_montserratMedium_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Overview_time_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Overview_time_label, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Overview_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Overview_time_label, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Overview_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Overview_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Overview_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Overview_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Overview_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Overview_time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_sub_label
    ui->Overview_sub_label = lv_label_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_sub_label, 360, 14);
    lv_obj_set_size(ui->Overview_sub_label, 293, 59);
    lv_label_set_text(ui->Overview_sub_label, "OVERVIEW");
    lv_label_set_long_mode(ui->Overview_sub_label, LV_LABEL_LONG_WRAP);

    //Write style for Overview_sub_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Overview_sub_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Overview_sub_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Overview_sub_label, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Overview_sub_label, &lv_font_montserratMedium_49, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Overview_sub_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Overview_sub_label, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Overview_sub_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Overview_sub_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Overview_sub_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Overview_sub_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Overview_sub_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Overview_sub_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Overview_sub_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Overview_sub_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_gps_icon
    ui->Overview_gps_icon = lv_image_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_gps_icon, 687, 133);
    lv_obj_set_size(ui->Overview_gps_icon, 55, 106);
    lv_obj_add_flag(ui->Overview_gps_icon, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->Overview_gps_icon, &_ebike_gps_arrow_1_ARGB8888_55x106);
    lv_image_set_pivot(ui->Overview_gps_icon, 0,0);
    lv_image_set_rotation(ui->Overview_gps_icon, 0);

    //Write style for Overview_gps_icon, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->Overview_gps_icon, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->Overview_gps_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_travelled_icon
    ui->Overview_travelled_icon = lv_image_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_travelled_icon, 588, 289);
    lv_obj_set_size(ui->Overview_travelled_icon, 43, 37);
    lv_obj_add_flag(ui->Overview_travelled_icon, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->Overview_travelled_icon, &_ebike_icn_distance_travelled_ARGB8888_43x37);
    lv_image_set_pivot(ui->Overview_travelled_icon, 0,0);
    lv_image_set_rotation(ui->Overview_travelled_icon, 0);

    //Write style for Overview_travelled_icon, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->Overview_travelled_icon, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->Overview_travelled_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_speed_icon
    ui->Overview_speed_icon = lv_image_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_speed_icon, 813, 288);
    lv_obj_set_size(ui->Overview_speed_icon, 47, 45);
    lv_obj_add_flag(ui->Overview_speed_icon, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->Overview_speed_icon, &_ebike_icn_average_speed_ARGB8888_47x45);
    lv_image_set_pivot(ui->Overview_speed_icon, 0,0);
    lv_image_set_rotation(ui->Overview_speed_icon, 0);

    //Write style for Overview_speed_icon, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->Overview_speed_icon, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->Overview_speed_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_remaining_icon
    ui->Overview_remaining_icon = lv_image_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_remaining_icon, 588, 446);
    lv_obj_set_size(ui->Overview_remaining_icon, 41, 42);
    lv_obj_add_flag(ui->Overview_remaining_icon, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->Overview_remaining_icon, &_ebike_icn_remaining_distance_ARGB8888_41x42);
    lv_image_set_pivot(ui->Overview_remaining_icon, 0,0);
    lv_image_set_rotation(ui->Overview_remaining_icon, 0);

    //Write style for Overview_remaining_icon, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->Overview_remaining_icon, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->Overview_remaining_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_elapsed_icon
    ui->Overview_elapsed_icon = lv_image_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_elapsed_icon, 810, 446);
    lv_obj_set_size(ui->Overview_elapsed_icon, 51, 58);
    lv_obj_add_flag(ui->Overview_elapsed_icon, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->Overview_elapsed_icon, &_ebike_icn_elapsed_time_2_ARGB8888_51x58);
    lv_image_set_pivot(ui->Overview_elapsed_icon, 0,0);
    lv_image_set_rotation(ui->Overview_elapsed_icon, 0);

    //Write style for Overview_elapsed_icon, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->Overview_elapsed_icon, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->Overview_elapsed_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_dist_label
    ui->Overview_dist_label = lv_label_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_dist_label, 764, 108);
    lv_obj_set_size(ui->Overview_dist_label, 219, 49);
    lv_label_set_text(ui->Overview_dist_label, "3.1 KM");
    lv_label_set_long_mode(ui->Overview_dist_label, LV_LABEL_LONG_WRAP);

    //Write style for Overview_dist_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Overview_dist_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Overview_dist_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Overview_dist_label, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Overview_dist_label, &lv_font_montserratMedium_55, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Overview_dist_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Overview_dist_label, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Overview_dist_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Overview_dist_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Overview_dist_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Overview_dist_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Overview_dist_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Overview_dist_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Overview_dist_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Overview_dist_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_dir_label
    ui->Overview_dir_label = lv_label_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_dir_label, 764, 181);
    lv_obj_set_size(ui->Overview_dir_label, 167, 26);
    lv_label_set_text(ui->Overview_dir_label, "Turn right to");
    lv_label_set_long_mode(ui->Overview_dir_label, LV_LABEL_LONG_WRAP);

    //Write style for Overview_dir_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Overview_dir_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Overview_dir_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Overview_dir_label, lv_color_hex(0x9e9e9e), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Overview_dir_label, &lv_font_montserratMedium_26, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Overview_dir_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Overview_dir_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Overview_dir_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Overview_dir_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Overview_dir_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Overview_dir_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Overview_dir_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Overview_dir_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Overview_dir_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Overview_dir_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_str_label
    ui->Overview_str_label = lv_label_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_str_label, 764, 220);
    lv_obj_set_size(ui->Overview_str_label, 236, 33);
    lv_label_set_text(ui->Overview_str_label, "Ed Bluestein Blvd");
    lv_label_set_long_mode(ui->Overview_str_label, LV_LABEL_LONG_WRAP);

    //Write style for Overview_str_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Overview_str_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Overview_str_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Overview_str_label, lv_color_hex(0xcccccc), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Overview_str_label, &lv_font_montserratMedium_26, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Overview_str_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Overview_str_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Overview_str_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Overview_str_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Overview_str_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Overview_str_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Overview_str_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Overview_str_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Overview_str_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Overview_str_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_info1_label_1
    ui->Overview_info1_label_1 = lv_label_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_info1_label_1, 653, 286);
    lv_obj_set_size(ui->Overview_info1_label_1, 122, 60);
    lv_label_set_text(ui->Overview_info1_label_1, "Distance\nTravelled");
    lv_label_set_long_mode(ui->Overview_info1_label_1, LV_LABEL_LONG_WRAP);

    //Write style for Overview_info1_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Overview_info1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Overview_info1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Overview_info1_label_1, lv_color_hex(0x9e9e9e), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Overview_info1_label_1, &lv_font_montserratMedium_26, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Overview_info1_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Overview_info1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Overview_info1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Overview_info1_label_1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Overview_info1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Overview_info1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Overview_info1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Overview_info1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Overview_info1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Overview_info1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_info2_label_1
    ui->Overview_info2_label_1 = lv_label_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_info2_label_1, 874, 285);
    lv_obj_set_size(ui->Overview_info2_label_1, 113, 60);
    lv_label_set_text(ui->Overview_info2_label_1, "Average\nSpeed");
    lv_label_set_long_mode(ui->Overview_info2_label_1, LV_LABEL_LONG_WRAP);

    //Write style for Overview_info2_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Overview_info2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Overview_info2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Overview_info2_label_1, lv_color_hex(0x9e9e9e), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Overview_info2_label_1, &lv_font_montserratMedium_26, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Overview_info2_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Overview_info2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Overview_info2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Overview_info2_label_1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Overview_info2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Overview_info2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Overview_info2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Overview_info2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Overview_info2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Overview_info2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_info3_label_1
    ui->Overview_info3_label_1 = lv_label_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_info3_label_1, 643, 442);
    lv_obj_set_size(ui->Overview_info3_label_1, 150, 57);
    lv_label_set_text(ui->Overview_info3_label_1, "Remaining\nDistance");
    lv_label_set_long_mode(ui->Overview_info3_label_1, LV_LABEL_LONG_WRAP);

    //Write style for Overview_info3_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Overview_info3_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Overview_info3_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Overview_info3_label_1, lv_color_hex(0x9e9e9e), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Overview_info3_label_1, &lv_font_montserratMedium_26, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Overview_info3_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Overview_info3_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Overview_info3_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Overview_info3_label_1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Overview_info3_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Overview_info3_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Overview_info3_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Overview_info3_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Overview_info3_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Overview_info3_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_linfo4_label_1
    ui->Overview_linfo4_label_1 = lv_label_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_linfo4_label_1, 875, 446);
    lv_obj_set_size(ui->Overview_linfo4_label_1, 114, 60);
    lv_label_set_text(ui->Overview_linfo4_label_1, "Elapsed\nTime");
    lv_label_set_long_mode(ui->Overview_linfo4_label_1, LV_LABEL_LONG_WRAP);

    //Write style for Overview_linfo4_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Overview_linfo4_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Overview_linfo4_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Overview_linfo4_label_1, lv_color_hex(0x9e9e9e), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Overview_linfo4_label_1, &lv_font_montserratMedium_26, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Overview_linfo4_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Overview_linfo4_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Overview_linfo4_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Overview_linfo4_label_1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Overview_linfo4_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Overview_linfo4_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Overview_linfo4_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Overview_linfo4_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Overview_linfo4_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Overview_linfo4_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_info1_label_3
    ui->Overview_info1_label_3 = lv_label_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_info1_label_3, 589, 364);
    lv_obj_set_size(ui->Overview_info1_label_3, 89, 47);
    lv_label_set_text(ui->Overview_info1_label_3, "12.7");
    lv_label_set_long_mode(ui->Overview_info1_label_3, LV_LABEL_LONG_WRAP);

    //Write style for Overview_info1_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Overview_info1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Overview_info1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Overview_info1_label_3, lv_color_hex(0x0080ff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Overview_info1_label_3, &lv_font_montserratMedium_45, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Overview_info1_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Overview_info1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Overview_info1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Overview_info1_label_3, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Overview_info1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Overview_info1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Overview_info1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Overview_info1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Overview_info1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Overview_info1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_info2_label_3
    ui->Overview_info2_label_3 = lv_label_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_info2_label_3, 813, 364);
    lv_obj_set_size(ui->Overview_info2_label_3, 89, 53);
    lv_label_set_text(ui->Overview_info2_label_3, "16.1");
    lv_label_set_long_mode(ui->Overview_info2_label_3, LV_LABEL_LONG_WRAP);

    //Write style for Overview_info2_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Overview_info2_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Overview_info2_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Overview_info2_label_3, lv_color_hex(0x0080ff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Overview_info2_label_3, &lv_font_montserratMedium_45, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Overview_info2_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Overview_info2_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Overview_info2_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Overview_info2_label_3, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Overview_info2_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Overview_info2_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Overview_info2_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Overview_info2_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Overview_info2_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Overview_info2_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_info3_label_3
    ui->Overview_info3_label_3 = lv_label_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_info3_label_3, 595, 517);
    lv_obj_set_size(ui->Overview_info3_label_3, 96, 50);
    lv_label_set_text(ui->Overview_info3_label_3, "18.3");
    lv_label_set_long_mode(ui->Overview_info3_label_3, LV_LABEL_LONG_WRAP);

    //Write style for Overview_info3_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Overview_info3_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Overview_info3_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Overview_info3_label_3, lv_color_hex(0x0080ff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Overview_info3_label_3, &lv_font_montserratMedium_45, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Overview_info3_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Overview_info3_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Overview_info3_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Overview_info3_label_3, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Overview_info3_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Overview_info3_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Overview_info3_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Overview_info3_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Overview_info3_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Overview_info3_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_info4_label_3
    ui->Overview_info4_label_3 = lv_label_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_info4_label_3, 821, 517);
    lv_obj_set_size(ui->Overview_info4_label_3, 139, 45);
    lv_label_set_text(ui->Overview_info4_label_3, "46:28");
    lv_label_set_long_mode(ui->Overview_info4_label_3, LV_LABEL_LONG_WRAP);

    //Write style for Overview_info4_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Overview_info4_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Overview_info4_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Overview_info4_label_3, lv_color_hex(0x0080ff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Overview_info4_label_3, &lv_font_montserratMedium_45, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Overview_info4_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Overview_info4_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Overview_info4_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Overview_info4_label_3, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Overview_info4_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Overview_info4_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Overview_info4_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Overview_info4_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Overview_info4_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Overview_info4_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_info1_label_4
    ui->Overview_info1_label_4 = lv_label_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_info1_label_4, 687, 375);
    lv_obj_set_size(ui->Overview_info1_label_4, 55, 33);
    lv_label_set_text(ui->Overview_info1_label_4, "km");
    lv_label_set_long_mode(ui->Overview_info1_label_4, LV_LABEL_LONG_WRAP);

    //Write style for Overview_info1_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Overview_info1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Overview_info1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Overview_info1_label_4, lv_color_hex(0x9e9e9e), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Overview_info1_label_4, &lv_font_montserratMedium_26, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Overview_info1_label_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Overview_info1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Overview_info1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Overview_info1_label_4, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Overview_info1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Overview_info1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Overview_info1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Overview_info1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Overview_info1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Overview_info1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_info2_label_4
    ui->Overview_info2_label_4 = lv_label_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_info2_label_4, 910, 375);
    lv_obj_set_size(ui->Overview_info2_label_4, 71, 33);
    lv_label_set_text(ui->Overview_info2_label_4, "km/h");
    lv_label_set_long_mode(ui->Overview_info2_label_4, LV_LABEL_LONG_WRAP);

    //Write style for Overview_info2_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Overview_info2_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Overview_info2_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Overview_info2_label_4, lv_color_hex(0x9e9e9e), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Overview_info2_label_4, &lv_font_montserratMedium_26, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Overview_info2_label_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Overview_info2_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Overview_info2_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Overview_info2_label_4, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Overview_info2_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Overview_info2_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Overview_info2_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Overview_info2_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Overview_info2_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Overview_info2_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_info3_label_4
    ui->Overview_info3_label_4 = lv_label_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_info3_label_4, 704, 528);
    lv_obj_set_size(ui->Overview_info3_label_4, 46, 27);
    lv_label_set_text(ui->Overview_info3_label_4, "km");
    lv_label_set_long_mode(ui->Overview_info3_label_4, LV_LABEL_LONG_WRAP);

    //Write style for Overview_info3_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Overview_info3_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Overview_info3_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Overview_info3_label_4, lv_color_hex(0x9e9e9e), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Overview_info3_label_4, &lv_font_montserratMedium_26, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Overview_info3_label_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Overview_info3_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Overview_info3_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Overview_info3_label_4, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Overview_info3_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Overview_info3_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Overview_info3_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Overview_info3_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Overview_info3_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Overview_info3_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_info_img_v
    ui->Overview_info_img_v = lv_image_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_info_img_v, 595, 426);
    lv_obj_set_size(ui->Overview_info_img_v, 400, 4);
    lv_obj_add_flag(ui->Overview_info_img_v, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->Overview_info_img_v, &_info_h_ARGB8888_400x4);
    lv_image_set_pivot(ui->Overview_info_img_v, 0,0);
    lv_image_set_rotation(ui->Overview_info_img_v, 0);

    //Write style for Overview_info_img_v, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->Overview_info_img_v, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->Overview_info_img_v, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_info_img_h
    ui->Overview_info_img_h = lv_image_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_info_img_h, 794, 275);
    lv_obj_set_size(ui->Overview_info_img_h, 4, 302);
    lv_obj_add_flag(ui->Overview_info_img_h, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->Overview_info_img_h, &_info_v_ARGB8888_4x302);
    lv_image_set_pivot(ui->Overview_info_img_h, 0,0);
    lv_image_set_rotation(ui->Overview_info_img_h, 0);

    //Write style for Overview_info_img_h, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->Overview_info_img_h, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->Overview_info_img_h, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_gauge
    ui->Overview_gauge = lv_image_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_gauge, 71, 147);
    lv_obj_set_size(ui->Overview_gauge, 400, 400);
    lv_obj_add_flag(ui->Overview_gauge, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->Overview_gauge, &_ebike_gauge_ARGB8888_400x400);
    lv_image_set_pivot(ui->Overview_gauge, 0,0);
    lv_image_set_rotation(ui->Overview_gauge, 0);

    //Write style for Overview_gauge, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->Overview_gauge, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->Overview_gauge, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_needle_img
    ui->Overview_needle_img = lv_image_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_needle_img, 110, 189);
    lv_obj_set_size(ui->Overview_needle_img, 320, 320);
    lv_obj_add_flag(ui->Overview_needle_img, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->Overview_needle_img, &_ebike_gauge_needle_ARGB8888_320x320);
    lv_image_set_pivot(ui->Overview_needle_img, 160,160);
    lv_image_set_rotation(ui->Overview_needle_img, 1500);

    //Write style for Overview_needle_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->Overview_needle_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->Overview_needle_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_battery_line_img
    ui->Overview_battery_line_img = lv_image_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_battery_line_img, 71, 346);
    lv_obj_set_size(ui->Overview_battery_line_img, 400, 200);
    lv_obj_add_flag(ui->Overview_battery_line_img, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->Overview_battery_line_img, &_ebike_gauge_battery_line_ARGB8888_400x200);
    lv_image_set_pivot(ui->Overview_battery_line_img, 0,0);
    lv_image_set_rotation(ui->Overview_battery_line_img, 0);

    //Write style for Overview_battery_line_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->Overview_battery_line_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->Overview_battery_line_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_battery_icon
    ui->Overview_battery_icon = lv_image_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_battery_icon, 217, 410);
    lv_obj_set_size(ui->Overview_battery_icon, 38, 33);
    lv_obj_add_flag(ui->Overview_battery_icon, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->Overview_battery_icon, &_ebike_gauge_battery_ARGB8888_38x33);
    lv_image_set_pivot(ui->Overview_battery_icon, 0,0);
    lv_image_set_rotation(ui->Overview_battery_icon, 0);

    //Write style for Overview_battery_icon, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->Overview_battery_icon, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->Overview_battery_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_battery_label
    ui->Overview_battery_label = lv_label_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_battery_label, 276, 414);
    lv_obj_set_size(ui->Overview_battery_label, 66, 28);
    lv_label_set_text(ui->Overview_battery_label, "56%");
    lv_label_set_long_mode(ui->Overview_battery_label, LV_LABEL_LONG_WRAP);

    //Write style for Overview_battery_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Overview_battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Overview_battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Overview_battery_label, lv_color_hex(0x0080ff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Overview_battery_label, &lv_font_montserratMedium_28, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Overview_battery_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Overview_battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Overview_battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Overview_battery_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Overview_battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Overview_battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Overview_battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Overview_battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Overview_battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Overview_battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_battery_arc
    ui->Overview_battery_arc = lv_arc_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_battery_arc, 96, 171);
    lv_obj_set_size(ui->Overview_battery_arc, 345, 348);
    lv_arc_set_mode(ui->Overview_battery_arc, LV_ARC_MODE_REVERSE);
    lv_arc_set_range(ui->Overview_battery_arc, 0, 100);
    lv_arc_set_bg_angles(ui->Overview_battery_arc, 49, 130);
    lv_arc_set_value(ui->Overview_battery_arc, 56);
    lv_arc_set_rotation(ui->Overview_battery_arc, 0);

    //Write style for Overview_battery_arc, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Overview_battery_arc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->Overview_battery_arc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui->Overview_battery_arc, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->Overview_battery_arc, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->Overview_battery_arc, lv_color_hex(0x0080ff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui->Overview_battery_arc, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Overview_battery_arc, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Overview_battery_arc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Overview_battery_arc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Overview_battery_arc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Overview_battery_arc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Overview_battery_arc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for Overview_battery_arc, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_arc_width(ui->Overview_battery_arc, 12, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->Overview_battery_arc, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->Overview_battery_arc, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui->Overview_battery_arc, true, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for Overview_battery_arc, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Overview_battery_arc, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui->Overview_battery_arc, 5, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes Overview_gauge_label_1
    ui->Overview_gauge_label_1 = lv_label_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_gauge_label_1, 212, 282);
    lv_obj_set_size(ui->Overview_gauge_label_1, 120, 69);
    lv_label_set_text(ui->Overview_gauge_label_1, "0");
    lv_label_set_long_mode(ui->Overview_gauge_label_1, LV_LABEL_LONG_WRAP);

    //Write style for Overview_gauge_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Overview_gauge_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Overview_gauge_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Overview_gauge_label_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Overview_gauge_label_1, &lv_font_montserratMedium_75, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Overview_gauge_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Overview_gauge_label_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Overview_gauge_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Overview_gauge_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Overview_gauge_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Overview_gauge_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Overview_gauge_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Overview_gauge_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Overview_gauge_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Overview_gauge_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Overview_gauge_label_2
    ui->Overview_gauge_label_2 = lv_label_create(ui->Overview);
    lv_obj_set_pos(ui->Overview_gauge_label_2, 226, 356);
    lv_obj_set_size(ui->Overview_gauge_label_2, 95, 29);
    lv_label_set_text(ui->Overview_gauge_label_2, "km/h");
    lv_label_set_long_mode(ui->Overview_gauge_label_2, LV_LABEL_LONG_WRAP);

    //Write style for Overview_gauge_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Overview_gauge_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Overview_gauge_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Overview_gauge_label_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Overview_gauge_label_2, &lv_font_montserratMedium_26, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Overview_gauge_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Overview_gauge_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Overview_gauge_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Overview_gauge_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Overview_gauge_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Overview_gauge_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Overview_gauge_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Overview_gauge_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Overview_gauge_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Overview_gauge_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of Overview.


    //Update current screen layout.
    lv_obj_update_layout(ui->Overview);

    //Init events for screen.
    events_init_Overview(ui);
}
