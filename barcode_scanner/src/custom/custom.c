/*
* Copyright 2024, NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/


/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include <stdint.h>
#include "lvgl.h"
#include "custom.h"

/*********************
 *      DEFINES
 *********************/

#define LV_UI_SCAN_ANIM_POS_BEGIN (200U)
#define LV_UI_SCAN_ANIM_POS_FADE_IN (240U)
#define LV_UI_SCAN_ANIM_POS_FADE_OUT (560U)
#define LV_UI_SCAN_ANIM_POS_END (600U)

#define LV_UI_SCAN_ANIM_POS_STEP (8)
#define LV_UI_SCAN_ANIM_OPA_STEP (6)

#define LV_UI_SCAN_ANIM_OPA_BEGIN (255 - (LV_UI_SCAN_ANIM_POS_FADE_IN - LV_UI_SCAN_ANIM_POS_BEGIN) * LV_UI_SCAN_ANIM_OPA_STEP)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**
 * Create a demo application
 */

void custom_init(lv_ui *ui)
{
    /* Add your codes here */
    update_scan_anim(ui);
}

void update_scan_anim(lv_ui *ui)
{
    static uint32_t scan_pos = LV_UI_SCAN_ANIM_POS_BEGIN;
    static uint32_t scan_opa = LV_UI_SCAN_ANIM_OPA_BEGIN;

    lv_obj_set_y(ui->scanner_scan_bar, scan_pos);
    lv_obj_set_style_img_opa(ui->scanner_scan_bar, scan_opa, LV_PART_MAIN|LV_STATE_DEFAULT);

    scan_pos += LV_UI_SCAN_ANIM_POS_STEP;
    if (scan_pos > LV_UI_SCAN_ANIM_POS_END)
    {
        scan_pos = LV_UI_SCAN_ANIM_POS_BEGIN;
        scan_opa = LV_UI_SCAN_ANIM_OPA_BEGIN;
    }
    if (scan_pos < LV_UI_SCAN_ANIM_POS_FADE_IN)
        scan_opa = 255 - LV_UI_SCAN_ANIM_OPA_STEP * (LV_UI_SCAN_ANIM_POS_FADE_IN - scan_pos);
    if (scan_pos > LV_UI_SCAN_ANIM_POS_FADE_OUT)
        scan_opa = 255 - LV_UI_SCAN_ANIM_OPA_STEP * (scan_pos - LV_UI_SCAN_ANIM_POS_FADE_OUT);

}

