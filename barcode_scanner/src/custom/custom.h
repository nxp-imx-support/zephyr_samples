/*
* Copyright 2024,2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef __CUSTOM_H_
#define __CUSTOM_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "gui_guider.h"

/**
 * @defgroup lvgl_custom LVGL Custom Functions
 * @brief Custom functions to alter the LVGL UI behavior
 * @{
 */

/**
 * @brief Initialize the barcode scanner UI components and start animation.
 *
 * This function is called during system initialization to set up the UI
 * and trigger the first update of the scan animation.
 */
void custom_init(lv_ui *ui);

/**
 * @brief Update the position and opacity of the scan bar animation.
 *
 * This function implements a continuous, looped animation of a scan bar
 * that moves vertically down the screen, with fade-in and fade-out effects
 * at the beginning and end of the scan path.
 *
 * The animation is implemented using a finite state machine approach with:
 * - A static position counter (`scan_pos`)
 * - A static opacity counter (`scan_opa`)
 *
 * The scan bar is represented by an `lv_obj_t` (typically an image or line)
 * stored in `ui->scanner_scan_bar`. The function updates its Y position and opacity
 * based on the current animation step.
 *
 * @param ui Pointer to the UI structure containing the scan bar object.
 *
 * @note This function must be called periodically (e.g., from a timer or task loop)
 *       to advance the animation frame-by-frame.
 *
 * @note The animation resets when `scan_pos` exceeds `LV_UI_SCAN_ANIM_POS_END`.
 *
 * @note Opacity is adjusted dynamically:
 *       - Before `LV_UI_SCAN_ANIM_POS_FADE_IN`: gradually fades in (opacity increases).
 *       - After `LV_UI_SCAN_ANIM_POS_FADE_OUT`: gradually fades out (opacity decreases).
 *       - Between the two: fully opaque.
 *
 * @warning The opacity calculation assumes that `LV_UI_SCAN_ANIM_OPA_STEP` is positive.
 *          The function does not handle negative step values.
 *
 * @see LV_UI_SCAN_ANIM_POS_BEGIN, LV_UI_SCAN_ANIM_POS_FADE_IN, LV_UI_SCAN_ANIM_POS_FADE_OUT,
 *      LV_UI_SCAN_ANIM_POS_END, LV_UI_SCAN_ANIM_POS_STEP, LV_UI_SCAN_ANIM_OPA_STEP
 */
void update_scan_anim(lv_ui *ui);

/** @} */ // end of group lvgl_custom

#ifdef __cplusplus
}
#endif
#endif /* EVENT_CB_H_ */
