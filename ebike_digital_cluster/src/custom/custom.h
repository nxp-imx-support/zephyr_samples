// SPDX-License-Identifier: MIT
// Copyright 2020 NXP

/*
 * custom.h
 *
 *  Created on: July 29, 2020
 *      Author: nxf53801
 */

#ifndef __CUSTOM_H_
#define __CUSTOM_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "gui_guider.h"

#define EDC_VIEWDISPLAY_THREAD_START_PRIO (-1)
#define EDC_VIEWDISPLAY_THREAD_PRIO (4)
#define EDC_VIEWDISPLAY_THREAD_STACK_SIZE (0x4000U)

void custom_init(lv_ui *ui);

#ifdef __cplusplus
}
#endif
#endif /* EVENT_CB_H_ */
