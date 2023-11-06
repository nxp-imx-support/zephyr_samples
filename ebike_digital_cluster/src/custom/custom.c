// SPDX-License-Identifier: MIT
// Copyright 2020 NXP

/**
 * @file custom.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "lvgl.h"
#include "custom.h"

#include "control.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(view_display, 4U);

/*********************
 *      DEFINES
 *********************/

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
}

void EDC_ViewDisplayUpdate(edc_dataModelSub_t *const dm_sub)
{
    switch (dm_sub->dataModel->data.modeData.driveMode)
    {
    case edc_driveMode_off:
        lv_obj_set_style_text_color(guider_ui.screen_drive_mode, lv_color_hex(0x909090), 0);
        lv_label_set_text(guider_ui.screen_drive_mode, "P. OFF");
        break;
    case edc_driveMode_eco:
        lv_obj_set_style_text_color(guider_ui.screen_drive_mode, lv_color_hex(0x90ee90), 0);
        lv_label_set_text(guider_ui.screen_drive_mode, "E.C.O");
        break;
    case edc_driveMode_normal:
        lv_obj_set_style_text_color(guider_ui.screen_drive_mode, lv_color_hex(0xffffff), 0);
        lv_label_set_text(guider_ui.screen_drive_mode, "Normal");
        break;
    case edc_driveMode_sport:
        lv_obj_set_style_text_color(guider_ui.screen_drive_mode, lv_color_hex(0x00bfff), 0);
        lv_label_set_text(guider_ui.screen_drive_mode, "Sport");
        break;
    }
	lv_label_set_text_fmt(guider_ui.screen_speed, "%d.%d", (dm_sub->dataModel->data.currentSpeed) / 1000, dm_sub->dataModel->data.currentSpeed % 1000);
    lv_meter_set_indicator_value(guider_ui.screen_meter, guider_ui.screen_meter_scale_1_ndimg_0, (dm_sub->dataModel->data.currentSpeed) / 1000);
}

void EDC_ViewDisplayTask(edc_ctrl_t * const ctrl, void*, void*)
{
    int32_t ret;
    edc_dataModelSub_t dm_sub;
    dm_sub.dataModel = NULL;

    k_thread_suspend(k_current_get());

    LOG_INF("subscribe model\n");

    ret = EDC_DataModelSubscribe(&dm_sub, ctrl->model);
    if (ret != 0U)
    {
        LOG_INF("subscribe fail %d\n", ret);
        return;
    }
    LOG_INF("subscribe ok\n");

    while (true)
    {
        EDC_DataModelEventWait(&dm_sub, K_FOREVER);
        // TODO: update display according to model
        EDC_ViewDisplayUpdate(&dm_sub);

    }
}

extern edc_ctrl_t edc_ctrl;
extern edc_dataModel_t edc_model;
#define EDC_VIEWDISPLAY_THREAD_PRIO (8U)
#define EDC_VIEWDISPLAY_THREAD_STACK_SIZE (0x1000U)
K_THREAD_DEFINE(edc_viewDisplay_task, EDC_VIEWDISPLAY_THREAD_STACK_SIZE,
                EDC_ViewDisplayTask, &edc_ctrl, NULL, NULL,
                EDC_VIEWDISPLAY_THREAD_PRIO, 0, 0);
