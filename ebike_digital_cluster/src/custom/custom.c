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
LOG_MODULE_REGISTER(view_display, 3U);

/*********************
 *      DEFINES
 *********************/

#define UINT_INTEGER_BASE_1000(i) (i / 1000)
#define UINT_FRAC_BASE_1000_D1(i) ((i % 1000) / 100)

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
    lv_obj_set_style_text_color(guider_ui.screen_drive_mode, lv_color_hex(0x909090), 0);
    lv_label_set_text(guider_ui.screen_drive_mode, "P. OFF");
    /** update currentSpeed */
	lv_label_set_text_fmt(guider_ui.screen_speed, "%2.2d.%1.1d"
            , UINT_INTEGER_BASE_1000(0)
            , UINT_FRAC_BASE_1000_D1(0));
    lv_meter_set_indicator_value(guider_ui.screen_meter, guider_ui.screen_meter_scale_1_ndimg_0
            , 0 / 1000);
    /** update distance */
    lv_label_set_text_fmt(guider_ui.screen_distance_value, "%3d.%1.1d"
            , UINT_INTEGER_BASE_1000(0)
            , UINT_FRAC_BASE_1000_D1(0 % 1000));
    /** update averageSpeed */
    lv_label_set_text_fmt(guider_ui.screen_speed, "%2d.%1.1d"
            , UINT_INTEGER_BASE_1000(0)
            , UINT_FRAC_BASE_1000_D1(0));
}

void EDC_ViewDisplayUpdate(edc_dataModelSub_t *const dm_sub)
{
    edc_dataModel_t *model = dm_sub->dataModel;

    //if(!EDC_DATAMODEL_IS_MEMBER_EQUAL(model, modeData.driveMode))
    //{
        switch (model->data.modeData.driveMode)
        {
        case edc_driveMode_off:
            lv_obj_set_style_text_color(guider_ui.screen_drive_mode, lv_color_hex(0x909090), 0);
            lv_label_set_text(guider_ui.screen_drive_mode, "P. OFF");
            break;
        case edc_driveMode_lock:
            lv_obj_set_style_text_color(guider_ui.screen_drive_mode, lv_color_hex(0xa14242), 0);
            lv_label_set_text(guider_ui.screen_drive_mode, "LOCK");
            break;
        case edc_driveMode_eco:
            lv_obj_set_style_text_color(guider_ui.screen_drive_mode, lv_color_hex(0x90ee90), 0);
            lv_label_set_text(guider_ui.screen_drive_mode, "E.C.O");
            break;
        case edc_driveMode_normal:
            lv_obj_set_style_text_color(guider_ui.screen_drive_mode, lv_color_hex(0xffffff), 0);
            lv_label_set_text(guider_ui.screen_drive_mode, "NORMAL");
            break;
        case edc_driveMode_sport:
            lv_obj_set_style_text_color(guider_ui.screen_drive_mode, lv_color_hex(0x00bfff), 0);
            lv_label_set_text(guider_ui.screen_drive_mode, "SPORT");
            break;
        case edc_driveMode_rain:
            lv_obj_set_style_text_color(guider_ui.screen_drive_mode, lv_color_hex(0x00ffff), 0);
            lv_label_set_text(guider_ui.screen_drive_mode, "RAIN");
            break;
        }
    //}
    //if(!EDC_DATAMODEL_IS_MEMBER_EQUAL(model, modeData.driveMode))
    //{
        /** update currentSpeed */
	    lv_label_set_text_fmt(guider_ui.screen_speed, "%2.2d.%1.1d"
                , UINT_INTEGER_BASE_1000(model->data.currentSpeed)
                , UINT_FRAC_BASE_1000_D1(model->data.currentSpeed));
        lv_meter_set_indicator_value(guider_ui.screen_meter, guider_ui.screen_meter_scale_1_ndimg_0
                , model->data.currentSpeed / 1000);
        /** update distance */
        lv_label_set_text_fmt(guider_ui.screen_distance_value, "%3d.%1.1d"
                , UINT_INTEGER_BASE_1000(model->data.distance)
                , UINT_FRAC_BASE_1000_D1(model->data.distance % 1000));
        /** update averageSpeed */
        lv_label_set_text_fmt(guider_ui.screen_avg_speed, "%2d.%1.1d"
                , UINT_INTEGER_BASE_1000(model->data.averageSpeed)
                , UINT_FRAC_BASE_1000_D1(model->data.averageSpeed));
    //}
}

void EDC_ViewDisplayTask(edc_ctrl_t * const ctrl, void*, void*)
{
    int32_t ret;
    edc_dataModelSub_t dm_sub;
    dm_sub.dataModel = NULL;

    k_thread_name_set(k_current_get(), "edc_viewDisp");

    LOG_INF("subscribe model\n");

    ret = EDC_DataModelSubscribe(&dm_sub, ctrl->model);
    if (ret != 0U)
    {
        LOG_INF("subscribe fail %d\n", ret);
        return;
    }
    LOG_INF("subscribe ok");

    /** return to normal priority */
    k_thread_priority_set(k_current_get(), EDC_VIEWDISPLAY_THREAD_PRIO);

    while (true)
    {
        EDC_DataModelEventWait(&dm_sub, K_FOREVER);
        EDC_ViewDisplayUpdate(&dm_sub);
    }
}
