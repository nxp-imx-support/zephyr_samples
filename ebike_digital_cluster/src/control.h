#ifndef __EDC_CONTROL_H_
#define __EDC_CONTROL_H__

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <zephyr/kernel.h>
#include <zephyr/kernel_structs.h>
#include <zephyr/spinlock.h>
#include <zephyr/sys/slist.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/can.h>
#include <zephyr/drivers/gpio.h>

#include "model.h"

#define EDC_CTRL_THREAD_START_PRIO (-1)
#define EDC_CTRL_THREAD_PRIO (5)
#define EDC_CTRL_THREAD_STACK_SIZE (0x4000U)
#define EDC_CTRL_WQ_STACK_SIZE (0x800U)

typedef enum _edc_ctrl_canMsg
{
    edc_ctrl_canMsg_timeSync = 0x100,
    edc_ctrl_canMsg_driveMode = 0x101,
    edc_ctrl_canMsg_currSpeed = 0x102,
} edc_ctrl_canMsg_t;

typedef struct _edc_ctrl
{
    k_tid_t thread_id;
    struct device const *can_dev;
    uint32_t can_filterId[8];
    edc_dataModel_t *model;
    struct k_work_q work_q;
    struct k_work ts_work; /** used for time sync */
    struct k_timer ts_timer;

    struct k_work pub_work; //TODO:clean-up work object if not used anymore
} edc_ctrl_t;

int32_t EDC_CtrlCanBusConfig(edc_ctrl_t * const ctrl);

void EDC_CtrlTask(edc_ctrl_t *const ctrl, edc_dataModel_t *const model, void*);

#endif //! __EDC_CONTROL_H__
