/*
* Copyright 2024,2025 NXP
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



void setup_scr_scanner(lv_ui *ui)
{
    //Write codes scanner
    ui->scanner = lv_obj_create(NULL);
    lv_obj_set_size(ui->scanner, 1024, 600);
    lv_obj_set_scrollbar_mode(ui->scanner, LV_SCROLLBAR_MODE_OFF);

    //Write style for scanner, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->scanner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes scanner_image_camera_preview
    ui->scanner_image_camera_preview = lv_image_create(ui->scanner);
    lv_obj_set_pos(ui->scanner_image_camera_preview, 0, 0);
    lv_obj_set_size(ui->scanner_image_camera_preview, 640, 600);
    lv_obj_add_flag(ui->scanner_image_camera_preview, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->scanner_image_camera_preview, &_qr_code_image_ARGB8888_640x600);
    lv_image_set_pivot(ui->scanner_image_camera_preview, 50,50);
    lv_image_set_rotation(ui->scanner_image_camera_preview, 0);

    //Write style for scanner_image_camera_preview, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->scanner_image_camera_preview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->scanner_image_camera_preview, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes scanner_scan_area
    ui->scanner_scan_area = lv_image_create(ui->scanner);
    lv_obj_set_pos(ui->scanner_scan_area, 95, 75);
    lv_obj_set_size(ui->scanner_scan_area, 450, 450);
    lv_obj_add_flag(ui->scanner_scan_area, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->scanner_scan_area, &_scan_area_ARGB8888_450x450);
    lv_image_set_pivot(ui->scanner_scan_area, 50,50);
    lv_image_set_rotation(ui->scanner_scan_area, 0);

    //Write style for scanner_scan_area, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->scanner_scan_area, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->scanner_scan_area, 220, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes scanner_scan_bar
    ui->scanner_scan_bar = lv_image_create(ui->scanner);
    lv_obj_set_pos(ui->scanner_scan_bar, 70, 120);
    lv_obj_set_size(ui->scanner_scan_bar, 500, 15);
    lv_obj_add_flag(ui->scanner_scan_bar, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->scanner_scan_bar, &_scan_bar_ARGB8888_500x15);
    lv_image_set_pivot(ui->scanner_scan_bar, 50,50);
    lv_image_set_rotation(ui->scanner_scan_bar, 0);

    //Write style for scanner_scan_bar, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->scanner_scan_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->scanner_scan_bar, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes scanner_text_fill
    ui->scanner_text_fill = lv_label_create(ui->scanner);
    lv_obj_set_pos(ui->scanner_text_fill, 640, 0);
    lv_obj_set_size(ui->scanner_text_fill, 384, 600);
    lv_label_set_text(ui->scanner_text_fill, "");
    lv_label_set_long_mode(ui->scanner_text_fill, LV_LABEL_LONG_WRAP);

    //Write style for scanner_text_fill, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->scanner_text_fill, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->scanner_text_fill, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->scanner_text_fill, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->scanner_text_fill, &lv_font_CascadiaMono_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->scanner_text_fill, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->scanner_text_fill, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->scanner_text_fill, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->scanner_text_fill, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->scanner_text_fill, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->scanner_text_fill, lv_color_hex(0x69ca00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->scanner_text_fill, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->scanner_text_fill, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->scanner_text_fill, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->scanner_text_fill, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->scanner_text_fill, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->scanner_text_fill, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes scanner_text_results
    ui->scanner_text_results = lv_label_create(ui->scanner);
    lv_obj_set_pos(ui->scanner_text_results, 647, 44);
    lv_obj_set_size(ui->scanner_text_results, 371, 550);
    lv_label_set_text(ui->scanner_text_results, "No QR-Code Detected yet.");
    lv_label_set_long_mode(ui->scanner_text_results, LV_LABEL_LONG_WRAP);

    //Write style for scanner_text_results, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->scanner_text_results, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->scanner_text_results, 12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->scanner_text_results, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->scanner_text_results, &lv_font_CascadiaMono_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->scanner_text_results, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->scanner_text_results, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->scanner_text_results, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->scanner_text_results, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->scanner_text_results, 192, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->scanner_text_results, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->scanner_text_results, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->scanner_text_results, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->scanner_text_results, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->scanner_text_results, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->scanner_text_results, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->scanner_text_results, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes scanner_text_bottom_mask
    ui->scanner_text_bottom_mask = lv_label_create(ui->scanner);
    lv_obj_set_pos(ui->scanner_text_bottom_mask, 645, 594);
    lv_obj_set_size(ui->scanner_text_bottom_mask, 375, 6);
    lv_label_set_text(ui->scanner_text_bottom_mask, "");
    lv_label_set_long_mode(ui->scanner_text_bottom_mask, LV_LABEL_LONG_WRAP);

    //Write style for scanner_text_bottom_mask, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->scanner_text_bottom_mask, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->scanner_text_bottom_mask, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->scanner_text_bottom_mask, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->scanner_text_bottom_mask, &lv_font_CascadiaMono_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->scanner_text_bottom_mask, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->scanner_text_bottom_mask, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->scanner_text_bottom_mask, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->scanner_text_bottom_mask, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->scanner_text_bottom_mask, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->scanner_text_bottom_mask, lv_color_hex(0x69ca00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->scanner_text_bottom_mask, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->scanner_text_bottom_mask, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->scanner_text_bottom_mask, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->scanner_text_bottom_mask, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->scanner_text_bottom_mask, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->scanner_text_bottom_mask, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes scanner_title
    ui->scanner_title = lv_label_create(ui->scanner);
    lv_obj_set_pos(ui->scanner_title, 649, 6);
    lv_obj_set_size(ui->scanner_title, 371, 32);
    lv_label_set_text(ui->scanner_title, "Barcode Scanner");
    lv_label_set_long_mode(ui->scanner_title, LV_LABEL_LONG_WRAP);

    //Write style for scanner_title, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->scanner_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->scanner_title, 12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->scanner_title, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->scanner_title, &lv_font_Poppins_Bold_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->scanner_title, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->scanner_title, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->scanner_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->scanner_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->scanner_title, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->scanner_title, lv_color_hex(0xf9b500), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->scanner_title, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->scanner_title, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->scanner_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->scanner_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->scanner_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->scanner_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of scanner.


    //Update current screen layout.
    lv_obj_update_layout(ui->scanner);

}
