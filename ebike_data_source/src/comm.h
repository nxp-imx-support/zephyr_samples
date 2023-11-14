#ifndef __EDS_COMM_H_
#define __EDS_COMM_H__

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

typedef enum _eds_comm_msg
{
    eds_comm_msg_sleep = 0x000,
    eds_comm_msg_abort = 0xfff,

    eds_comm_msg_timeSync = 0x100,
    eds_comm_msg_driveMode = 0x101,
    eds_comm_msg_currSpeed = 0x102,
} eds_comm_msg_t;

typedef struct _eds_comm
{
    k_tid_t thread_id;
    struct device *can_dev;
    uint32_t can_filterId[8];
} eds_comm_t;

typedef enum _eds_driveMode
{
    eds_driveMode_off = 0U,
    eds_driveMode_lock, // child lock / protection
    eds_driveMode_eco,
    eds_driveMode_normal,
    eds_driveMode_sport,
    eds_driveMode_rain,
} eds_driveMode_t;

typedef enum _eds_lightMode
{
    /** main light */
    eds_lightMode_main_off =    0b00U,
    eds_lightMode_main_day =    0b01U,
    eds_lightMode_main_on =     0b10U,
    eds_lightMode_main_max =    0b11U,
    /** turn light */
    eds_lightMode_turn_na =     0b00U,
    eds_lightMode_turn_lt =     0b10U,
    eds_lightMode_turn_rt =     0b01U,
    /** rear light */
    eds_lightMode_rear_off =    0b00U,
    eds_lightMode_rear_dimm =   0b01U,
    eds_lightMode_rear_bright = 0b10U,
} eds_lightMode_t;

#define EDS_DRIVEMODE_GEN(mode, mainLight, turnLight, rearLight) \
    (uint32_t)( ((eds_driveMode_##mode) << 0) \
              | ((eds_lightMode_main_##mainLight) << 3) \
              | ((eds_lightMode_turn_##turnLight) << 5) \
              |  ((eds_lightMode_rear_##rearLight) << 7) \
    )

typedef struct _eds_commSeq
{
    eds_comm_msg_t msg;
    uint32_t data[2];
}eds_commSeq_t;

#define EDS_COMM_SEQ_SLEEP(time_ms) \
{ \
    .msg = eds_comm_msg_sleep, \
    .data = {time_ms, 0}, \
}

#define EDS_COMM_SEQ_ABORT \
{ \
    .msg = eds_comm_msg_abort, \
    .data = {0, 0}, \
}

#define EDS_COMM_SEQ_SEND(comm_msg, data0, data1) \
{ \
    .msg = eds_comm_msg_##comm_msg, \
    .data = {data0, data1}, \
}


#endif //! __EDS_COMM_H__
