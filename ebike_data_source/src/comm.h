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
#include <zephyr/drivers/uart.h>

#include "model.h"

#define EDS_COMM_THREAD_PRIO (5)
#define EDS_COMM_THREAD_START_PRIO (-1)
#define EDS_COMM_THREAD_STACK_SIZE (0x4000U)
#define EDS_COMM_WQ_STACK_SIZE (0x800U)

#define CONFIG_EDS_COMM_WFM_OUT (1)
#define CONFIG_EDS_COMM_WFM_OUT_DEVICE lpuart2

#define EDS_CAN_FRAME_INTERVAL_MS (50)

typedef enum _eds_comm_msg
{
    eds_comm_msg_sleep = 0x000,
    eds_comm_msg_abort = 0xfff,

    eds_comm_msg_timeSync =     CONFIG_EDS_CAN_FRAME_ID_BASE + 0,
    eds_comm_msg_driveMode =    CONFIG_EDS_CAN_FRAME_ID_BASE + 1,
    eds_comm_msg_currSpeed =    CONFIG_EDS_CAN_FRAME_ID_BASE + 2,
} eds_comm_msg_t;

typedef enum _eds_driveMode
{
    eds_driveMode_off = 0U,
    eds_driveMode_lock, // child lock / protection
    //eds_driveMode_eco,
    eds_driveMode_normal,
    //eds_driveMode_rain,
    eds_driveMode_sport,
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

typedef struct _eds_comm
{
    k_tid_t thread_id;
    struct device const *can_dev;
    uint32_t can_filterId[8];
    struct k_mutex lock;
    struct k_timer send_timer;
    struct k_work_q work_q;
    struct k_work ts_work; /** used for time sync */
#ifdef CONFIG_EDS_COMM_WFM_OUT
    struct device const *uart_dev;
#endif // ! CONFIG_EDS_COMM_WFM_OUT

    struct
    {
        bool is_demo;
        eds_driveMode_t drive_mode;
        int32_t target_accel; /** accel +/- button set nominal_accel, and then apply random swing on it */
        int32_t nominal_accel;
        int32_t curr_speed;
        uint32_t ts_recv;
    }state;

    struct
    {
        double deccel_coeff, deccel_coeff_2;
        int32_t accel_swing_permillage; /** maximum permillage average_accel can randomly swing */
        double accel_ctrl_kp, accel_ctrl_ki;
        int32_t accel_ctrl_err, accel_ctrl_err_intg;
    }kinetics;
} eds_comm_t;

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

/**
 * @brief Key code on 4x4 key pad
 *
 * eds_comm_keyCode_MP : mode +
 * eds_comm_keyCode_MN : mode -
 *
 * eds_comm_keyCode_SP : speed +
 * eds_comm_keyCode_SN : speed -
 *
 * eds_comm_keyCode_FL : front light
 * eds_comm_keyCode_RL : rear light
 * eds_comm_keyCode_LT : left trun
 * eds_comm_keyCode_RT : right turn
 *
 */
typedef enum _eds_commKeyCode
{
    eds_comm_keyCode_MP = BIT(0), eds_comm_keyCode_FL = BIT(1), eds_comm_keyCode_SP  = BIT(2), eds_comm_keyCode_3 = BIT(3),
    eds_comm_keyCode_4 = BIT(4), eds_comm_keyCode_LT = BIT(5), eds_comm_keyCode_6 = BIT(6), eds_comm_keyCode_RT = BIT(7),
    eds_comm_keyCode_MN = BIT(8), eds_comm_keyCode_RL = BIT(9), eds_comm_keyCode_SN = BIT(10), eds_comm_keyCode_b = BIT(11),
    eds_comm_keyCode_KP_N = BIT(12), eds_comm_keyCode_KP_P = BIT(13), eds_comm_keyCode_KI_N = BIT(14), eds_comm_keyCode_KI_P = BIT(15),
}eds_commKeyCode_t;

void EDS_CommKeyInput(eds_comm_t *const comm, uint16_t keycode);

void EDS_CommTask(eds_comm_t *const comm, void* p2, void* p3);


#endif //! __EDS_COMM_H__
