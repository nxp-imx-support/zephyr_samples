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
    struct k_work pub_work; //TODO:clean-up work object if not used anymore
} edc_ctrl_t;


#endif //! __EDC_CONTROL_H__
