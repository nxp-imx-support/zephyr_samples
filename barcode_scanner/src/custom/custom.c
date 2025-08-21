/*
* Copyright 2024,2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
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

/**
 * @defgroup scan_animation_config Animation Configuration Macros
 * @brief Configuration parameters for the scan bar animation.
 * @{
 */

/**
 * @def LV_UI_SCAN_ANIM_POS_BEGIN
 * @brief Starting Y position of the scan animation (in pixels).
 *
 * Defines the initial vertical position of the scan bar at the start of the animation.
 * Typically set near the top of the scanning area.
 *
 * @note Must be a compile-time constant. The animation begins here.
 */
#define LV_UI_SCAN_ANIM_POS_BEGIN (135U)

/**
 * @def LV_UI_SCAN_ANIM_POS_END
 * @brief Final Y position of the scan animation (in pixels).
 *
 * Defines the end of the scan path. After reaching this point, the animation resets.
 *
 * @note This value is used to detect when the animation should reset.
 */
#define LV_UI_SCAN_ANIM_POS_END (LV_UI_SCAN_ANIM_POS_BEGIN + 330U)

/**
 * @def LV_UI_SCAN_ANIM_FADE_HEIGHT
 * @brief Defines the distance (in pixels) over which the scan bar fades in and out.
 *
 * This constant determines the length of the fade-in and fade-out zones in the scan animation.
 * Both fade-in and fade-out transitions occur over this distance, creating a smooth visual effect.
 *
 * - Fade-in starts `LV_UI_SCAN_ANIM_FADE_HEIGHT` pixels after `LV_UI_SCAN_ANIM_POS_BEGIN`.
 * - Fade-out starts `LV_UI_SCAN_ANIM_FADE_HEIGHT` pixels before `LV_UI_SCAN_ANIM_POS_END`.
 *
 * A larger value results in a longer, smoother fade transition, while a smaller value creates a more abrupt change.
 *
 * @note This value must be consistent with the animation speed (`LV_UI_SCAN_ANIM_POS_STEP`) and opacity step (`LV_UI_SCAN_ANIM_OPA_STEP`)
 *       to ensure smooth and predictable transitions.
 */
#define LV_UI_SCAN_ANIM_FADE_HEIGHT (45U)

/**
 * @def LV_UI_SCAN_ANIM_POS_STEP
 * @brief Step size for Y position update per animation frame (in pixels).
 *
 * Controls the speed of the scan bar movement. Smaller values result in smoother motion.
 *
 * @note Must be a positive integer. Default value is 5 pixels per frame.
 * @note Should be chosen carefully to match the target frame rate.
 */
#define LV_UI_SCAN_ANIM_POS_STEP (5)

/**
 * @def LV_UI_SCAN_ANIM_OPA_STEP
 * @brief Step size for opacity (alpha) change per animation frame.
 *
 * Determines how quickly the opacity changes during fade-in and fade-out phases.
 * Each step changes the opacity by this amount (in range 0–255).
 *
 * @note A value of 8 means opacity changes by 8 units per frame.
 * @note Smaller values produce smoother transitions but may reduce responsiveness.
 */
#define LV_UI_SCAN_ANIM_OPA_STEP (20)

/**
 * @def LV_UI_SCAN_ANIM_BLANK_CNT
 * @brief Defines the number of animation frames the scan bar remains hidden after completing a full cycle.
 *
 * This constant determines the duration (in frames) that the scan bar is hidden after reaching the end of its animation path.
 * During this period, the scan bar is effectively "off" and not visible on screen, creating a pause before the animation restarts.
 *
 * @note This value should be chosen to create a natural pause, typically a few frames, to avoid a too-rapid repetition.
 * @note A value of 0 would result in an immediate restart with no pause.
 * @note The actual pause duration depends on the frame rate of the system (e.g., 30 FPS → 1 frame ≈ 33ms).
 */
# define LV_UI_SCAN_ANIM_BLANK_CNT (15)

/**
 * @def LV_UI_SCAN_ANIM_POS_FADE_IN
 * @brief Y position where the scan bar starts fading in (in pixels).
 *
 * The scan bar gradually becomes visible as it approaches this position.
 *
 * @note This creates a soft entrance effect. Opacity increases from 0 to 255.
 */
#define LV_UI_SCAN_ANIM_POS_FADE_IN \
        (LV_UI_SCAN_ANIM_POS_BEGIN + LV_UI_SCAN_ANIM_FADE_HEIGHT)

/**
 * @def LV_UI_SCAN_ANIM_POS_FADE_OUT
 * @brief Y position where the scan bar starts fading out (in pixels).
 *
 * The scan bar gradually becomes invisible as it approaches this position.
 *
 * @note This creates a soft exit effect. Opacity decreases from 255 to 0.
 */
#define LV_UI_SCAN_ANIM_POS_FADE_OUT \
        (LV_UI_SCAN_ANIM_POS_END - LV_UI_SCAN_ANIM_FADE_HEIGHT)

/**
 * @def LV_UI_SCAN_ANIM_OPA_BY_DISTANCE
 * @brief Calculates the opacity based on distance from fade-in/out transition points.
 *
 * This macro computes the opacity value dynamically based on how far the scan bar is
 * from the fade-in or fade-out start positions. It ensures smooth transitions by
 * linearly adjusting opacity over a defined distance.
 *
 * The formula is:
 *   opacity = 255 - (distance * OPA_STEP / POS_STEP)
 *
 * Where:
 *   - `distance` is the number of pixels from the current position to the fade transition point.
 *   - `OPA_STEP` is the opacity increment per pixel (in range 0–255).
 *   - `POS_STEP` is the movement step size per frame (in pixels).
 *
 * This allows the scan bar to fade in smoothly as it approaches `LV_UI_SCAN_ANIM_POS_FADE_IN`
 * and fade out as it nears `LV_UI_SCAN_ANIM_POS_FADE_OUT`, creating a natural scanning effect.
 *
 * @note The result must be clamped to the valid opacity range [0, 255].
 * @note This macro is used internally in `update_scan_anim()` to control visual transitions.
 */
#define LV_UI_SCAN_ANIM_OPA_BY_DISTANCE(distance) (255 - \
        (distance) * LV_UI_SCAN_ANIM_OPA_STEP / LV_UI_SCAN_ANIM_POS_STEP)

/** @} */ // end of group scan_animation_config

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
    static uint32_t scan_opa = LV_UI_SCAN_ANIM_OPA_BY_DISTANCE(
        LV_UI_SCAN_ANIM_POS_FADE_IN - LV_UI_SCAN_ANIM_POS_BEGIN);
    static uint32_t blank_cnt = 0;

    // Hold animation in hidden state
    if (blank_cnt) {
        --blank_cnt;
        if (blank_cnt == 0U) {
            lv_obj_remove_flag(ui->scanner_scan_bar, LV_OBJ_FLAG_HIDDEN);
        } else {
            return;
        }
    }

    // Advance position
    scan_pos += LV_UI_SCAN_ANIM_POS_STEP;

    // Reset animation if it reaches the end
    if (scan_pos > LV_UI_SCAN_ANIM_POS_END){
        scan_pos = LV_UI_SCAN_ANIM_POS_BEGIN;
        blank_cnt = LV_UI_SCAN_ANIM_BLANK_CNT;
        lv_obj_add_flag(ui->scanner_scan_bar, LV_OBJ_FLAG_HIDDEN);
        return;
    }

    if (scan_pos < LV_UI_SCAN_ANIM_POS_FADE_IN) {
        // Fade-in: opacity increases as we approach fade-in start
        scan_opa = LV_UI_SCAN_ANIM_OPA_BY_DISTANCE(LV_UI_SCAN_ANIM_POS_FADE_IN - scan_pos);
    } else if (scan_pos > LV_UI_SCAN_ANIM_POS_FADE_OUT) {
        // Fade-out: opacity decreases as we approach fade-out end
        scan_opa = LV_UI_SCAN_ANIM_OPA_BY_DISTANCE(scan_pos - LV_UI_SCAN_ANIM_POS_FADE_OUT);
    } else {
        scan_opa = 255;
    }

    // Update Y position of the scan bar
    lv_obj_set_y(ui->scanner_scan_bar, scan_pos);

    // Update opacity of the scan bar
    lv_obj_set_style_image_opa(ui->scanner_scan_bar, scan_opa, LV_PART_MAIN | LV_STATE_DEFAULT);
}
