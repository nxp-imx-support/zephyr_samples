#include "comm.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(eds_control, CONFIG_EDS_COMM_LOG_LEVEL);

struct can_filter const eds_comm_canRxFilter[] =
{
    {
        .flags = CAN_FILTER_DATA,
        .id = eds_comm_msg_timeSync,
        .mask = CAN_STD_ID_MASK,
    },
};

const char* eds_comm_drive_mode_name[] =
{
    "eds_driveMode_off",
    "eds_driveMode_lock",
    //"eds_driveMode_eco",
    "eds_driveMode_normal",
    //"eds_driveMode_rain",
    "eds_driveMode_sport",
};

void EDS_CommCanBusRxCb(struct device const *const dev, struct can_frame *frame, void *userData)
{

    eds_comm_t * const comm = (eds_comm_t*)userData;
    LOG_DBG("can rx frame: id=0x%3.3x, dlc=%d\n",
           frame->id, frame->dlc);

    switch (frame->id)
    {
    case eds_comm_msg_timeSync:
        /** send local timestamp */
        comm->state.ts_recv = k_cyc_to_us_near32(k_cycle_get_64());
        k_work_submit_to_queue(&comm->work_q, &comm->ts_work);
        break;
    default:
        break;
    }
}

void EDS_CommCanBusTxCb(struct device const *const dev, int32_t error, void *userData)
{
    eds_comm_t * const comm = (eds_comm_t*)userData;
    if (error != 0)
    {
        LOG_ERR("can tx error [%d]", error);
    }
}

int32_t EDS_CommCanBusSend(eds_comm_t * const comm, struct can_frame * const frame)
{
    return can_send(comm->can_dev, frame, K_FOREVER, EDS_CommCanBusTxCb, comm);
}

int32_t EDS_CommCanBusConfig(eds_comm_t * const comm)
{
    int32_t ret;
    comm->can_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_canbus));
    if(!device_is_ready(comm->can_dev))
    {
        LOG_ERR("can device not ready [%d]\n", -ENODEV);
        return -ENODEV;
    }

    struct can_timing timing;
    ret = can_stop(comm->can_dev);
    ret = can_calc_timing(comm->can_dev, &timing, 1000000, 875);
    ret = can_set_timing(comm->can_dev, &timing);
#ifdef CONFIG_CAN_FD_MODE
    ret = can_calc_timing_data(comm->can_dev, &timing, 2000000, 875);
    ret = can_set_timing_data(comm->can_dev, &timing);
#endif // CONFIG_CAN_FD_MODE
    ret = can_start(comm->can_dev);
    if (ret != 0)
    {
        LOG_ERR("set can timing fail [%d]\n", ret);
    }

    uint32_t const filter_count = sizeof(eds_comm_canRxFilter) / sizeof(struct can_filter);
    for(uint32_t i = 0U; i < filter_count; ++i)
    {
        ret = can_add_rx_filter(comm->can_dev, EDS_CommCanBusRxCb, comm, &eds_comm_canRxFilter[i]);
        if(comm->can_filterId < 0)
        {
            LOG_ERR("add can filter fail [%d]\n", ret);
            return ret;
        }
        comm->can_filterId[i] = ret;
    }

    return 0;
}

int EDS_CommSendSequence(eds_comm_t *const comm, eds_commSeq_t const * seq)
{
    int ret = 0;
    size_t len = 0U;
    struct can_frame frame;

#ifdef CONFIG_CAN_FD_MODE
	frame.flags = CAN_FRAME_FDF | CAN_FRAME_BRS;
#else
	frame.flags = 0U;
#endif // CONFIG_CAN_FD_MODE

    while(true)
    {
        switch (seq[len].msg)
        {
        case eds_comm_msg_sleep:
            k_sleep(K_MSEC(seq[len].data[0]));
            break;
        case eds_comm_msg_abort:
            return 0;
        case eds_comm_msg_timeSync:
            uint64_t time_64 = k_uptime_get();
            frame.id = seq[len].msg;
            memcpy(&frame.data, &time_64, 8U);
            frame.dlc = 8U;
            ret = EDS_CommCanBusSend(comm, &frame);
            if (ret)
            {
                LOG_ERR("send fail at seq %d [%d]", len, ret);
            }
            break;
        default:
            frame.id = seq[len].msg;
            memcpy(&frame.data, seq[len].data, 8U);
            frame.dlc = 8U;
            ret = EDS_CommCanBusSend(comm, &frame);
            if (ret)
            {
                LOG_ERR("send fail at seq %d [%d]", len, ret);
            }
            break;
        }

        ++len;
    }

    return -EINVAL; // should never reach here
}

#ifdef CONFIG_EDS_COMM_WFM_OUT
void EDS_CommSendUartStr(eds_comm_t *const comm, char *str)
{
	int msg_len = strlen(str);

	for (int i = 0; i < msg_len; i++) {
		uart_poll_out(comm->uart_dev, str[i]);
	}

	//printk("Device %s sent: \"%s\"\n", uart->name, str);
}
#endif // CONFIG_EDS_COMM_WFM_OUT

void EDS_CommKineticsModelUpdate(eds_comm_t *const comm)
{
    static int demo_frame = 0;
    if(comm->state.is_demo == true)
    {
        switch (demo_frame)
        {
        case (16):
            comm->state.drive_mode = eds_driveMode_lock;
            break;
        case 24:
            comm->state.drive_mode = eds_driveMode_normal;
            break;
        case 28:
            comm->state.target_accel = 250;
            break;
        case 64:
            comm->state.target_accel = 1000;
            break;
        case 96:
            comm->state.target_accel = 2000;
            break;
        case 128:
            comm->state.target_accel = 3000;
            break;
        case 152:
            comm->state.drive_mode = eds_driveMode_sport;
            break;
        case 160:
            comm->state.target_accel = 9000;
            break;
        case 192:
            comm->state.target_accel = 15000;
            break;
        case 216:
            comm->state.target_accel = 18000;
            break;
        case 240:
            comm->state.target_accel = 20000;
            break;
        case 264:
            comm->state.target_accel = 16000;
            break;
        case 288:
            comm->state.target_accel = 8000;
            break;
        case 320:
            comm->state.target_accel = 10000;
            break;
        case 384:
            comm->state.drive_mode = eds_driveMode_normal;
            comm->state.target_accel = 6000;
            break;
        case 400:
            comm->state.target_accel = 4000;
            break;
        case 416:
            comm->state.target_accel = 2000;
            break;
        case 432:
            comm->state.target_accel = 0;
            break;
        case 464:
            comm->state.drive_mode = eds_driveMode_off;
            break;

        }

        demo_frame = (++demo_frame) % 480;
    }
    //else if(comm->state.drive_mode == eds_driveMode_sport)
    //{
    //    comm->state.curr_speed = ((double)comm->state.target_accel) * 2.25;
    //    /** let's assume accel_ctrl_err = 0 to prevent issue */
    //    comm->kinetics.accel_ctrl_err = 0;
    //    comm->kinetics.accel_ctrl_err_intg = 0;
    //    comm->state.nominal_accel = comm->state.curr_speed * comm->kinetics.deccel_coeff
    //                     + comm->state.curr_speed * comm->state.curr_speed * comm->kinetics.deccel_coeff_2;
    //    return;
    //}

    /** calculate PI controller */
    comm->kinetics.accel_ctrl_err = (comm->state.target_accel - comm->state.nominal_accel);
    comm->kinetics.accel_ctrl_err_intg += comm->kinetics.accel_ctrl_err;
    double nominal_accel_control = comm->kinetics.accel_ctrl_err * comm->kinetics.accel_ctrl_kp
                     + comm->kinetics.accel_ctrl_err_intg * comm->kinetics.accel_ctrl_ki;
    comm->state.nominal_accel += nominal_accel_control;

    /** calculate real accel, consider swing */
    int32_t real_accel = comm->state.nominal_accel; //TODO use rand() to randomize this

    /** calculate drag deccel */
    int32_t real_deccel = comm->kinetics.deccel_coeff * comm->state.curr_speed
                         + comm->kinetics.deccel_coeff_2 * comm->state.curr_speed * comm->state.curr_speed;

    /** calculate curr_speed */
    comm->state.curr_speed = comm->state.curr_speed + real_accel - real_deccel;
    if (comm->state.curr_speed < 0)
    {
        comm->state.curr_speed = 0;
        comm->kinetics.accel_ctrl_err = 0;
        comm->kinetics.accel_ctrl_err_intg = 0;
    }
    else if(comm->state.curr_speed > 56000)
    {
        comm->state.curr_speed = 50000;
    }

#ifdef CONFIG_EDS_COMM_WFM_OUT
    static char str_buf[256];

    snprintf(str_buf, 256,
        "target_accel=%d,"
        "real_accel=%d,"
        "accel_err=%d,"
        "real_deccel=%d,"
        "curr_speed=%d,"
        "\r\n"
        , comm->state.target_accel
        , real_accel
        , comm->kinetics.accel_ctrl_err
        , real_deccel
        , comm->state.curr_speed
    );
    EDS_CommSendUartStr(comm, str_buf);
#endif // CONFIG_EDS_COMM_WFM_OUT
}

void EDS_CommKeyInput(eds_comm_t *const comm, uint16_t keycode)
{
    k_mutex_lock(&comm->lock, K_FOREVER);

    if(comm->state.is_demo == true && keycode != 0)
    {
        comm->state.is_demo = false;
        comm->state.drive_mode = eds_driveMode_off;
        comm->state.target_accel = 0;
        comm->state.nominal_accel = 0;
        comm->state.curr_speed = 0;
        comm->kinetics.accel_ctrl_err = 0;
        comm->kinetics.accel_ctrl_err_intg = 0;
        LOG_INF("demo mode exit");
        k_mutex_unlock(&comm->lock);
        return;
    }

    if (keycode & eds_comm_keyCode_MP && !(keycode & eds_comm_keyCode_MN)
        && comm->state.drive_mode < eds_driveMode_sport)
    {
        comm->state.drive_mode ++;
        LOG_DBG("drive mode +, curr %s", eds_comm_drive_mode_name[comm->state.drive_mode]);
    }
    if (keycode & eds_comm_keyCode_MN && !(keycode & eds_comm_keyCode_MP)
        && comm->state.drive_mode > eds_driveMode_off)
    {
        if(comm->state.drive_mode == eds_driveMode_lock + 1)
        {
            comm->state.drive_mode -= 2;
        }
        else
        {
            comm->state.drive_mode --;
        }
        if(comm->state.drive_mode == eds_driveMode_off)
        {
            comm->state.target_accel = 0;
        }
        LOG_DBG("drive mode -, curr %s", eds_comm_drive_mode_name[comm->state.drive_mode]);
    }


    if (keycode & eds_comm_keyCode_SP && !(keycode & eds_comm_keyCode_SN)
        && comm->state.drive_mode > eds_driveMode_lock
        && comm->state.target_accel < 20000)
    {
        if(comm->state.drive_mode == eds_driveMode_sport)
        {
            comm->state.target_accel += 1000;
            comm->state.target_accel -= comm->state.target_accel % 1000;
        }
        else
        {
            comm->state.target_accel += 250;
        }
        LOG_DBG("speed +250, curr %d", comm->state.target_accel);
    }
    if (keycode & eds_comm_keyCode_SN && !(keycode & eds_comm_keyCode_SP)
        && comm->state.drive_mode > eds_driveMode_lock
        && comm->state.target_accel != 0)
    {
        if(comm->state.drive_mode == eds_driveMode_sport)
        {
            comm->state.target_accel -= 1000;
            comm->state.target_accel -= comm->state.target_accel % 1000;
        }
        else
        {
            comm->state.target_accel -= 250;
        }
        LOG_DBG("speed -250, curr %d", comm->state.target_accel);
    }

    k_mutex_unlock(&comm->lock);
}

void EDS_CommTsWork(struct k_work *item)
{
    int ret;
    eds_comm_t *comm =
        CONTAINER_OF(item, eds_comm_t, ts_work);
    LOG_DBG("timesync work");

        static struct can_frame frame = {
#ifdef CONFIG_CAN_FD_MODE
	    .flags = CAN_FRAME_FDF | CAN_FRAME_BRS,
#else
	    .flags = 0U,
#endif // CONFIG_CAN_FD_MODE
        .id = eds_comm_msg_timeSync,
        .dlc = 8U,
    };
    frame.data_32[0] = comm->state.ts_recv;
    frame.data_32[1] = k_cyc_to_us_near32(k_cycle_get_64());

    ret = EDS_CommCanBusSend(comm, &frame);

}

K_THREAD_STACK_DEFINE(eds_comm_wq_stack, EDS_COMM_WQ_STACK_SIZE);

void EDS_CommTask(eds_comm_t *const comm, void* p2, void* p3)
{
    int32_t ret;
    struct can_frame frame;

    LOG_INF("eds comm task\n");
    comm->thread_id = k_current_get();

    k_mutex_init(&comm->lock);

    comm->state.is_demo = true;
    comm->state.drive_mode = eds_driveMode_off;
    comm->state.target_accel = 0;
    comm->state.nominal_accel = 0;
    comm->state.curr_speed = 0;

    comm->kinetics.deccel_coeff = 0.00175;
    comm->kinetics.deccel_coeff_2 = 0.00001;
    comm->kinetics.accel_swing_permillage = 50;
    comm->kinetics.accel_ctrl_kp = 0.1;
    comm->kinetics.accel_ctrl_ki = 0.002;
    comm->kinetics.accel_ctrl_err = 0;
    comm->kinetics.accel_ctrl_err_intg = 0;

    k_work_queue_init(&comm->work_q);
    k_work_queue_start(&comm->work_q, eds_comm_wq_stack,
                   EDS_COMM_WQ_STACK_SIZE, EDS_COMM_THREAD_PRIO + 1,
                   NULL);

    k_work_init(&comm->ts_work, EDS_CommTsWork);

    k_timer_init(&comm->send_timer, NULL, NULL);

    ret = EDS_CommCanBusConfig(comm);
    if(ret != 0)
    {
        return;
    }
#ifdef CONFIG_CAN_FD_MODE
	frame.flags = CAN_FRAME_FDF | CAN_FRAME_BRS;
#else
	frame.flags = 0U;
#endif // CONFIG_CAN_FD_MODE

#ifdef CONFIG_EDS_COMM_WFM_OUT
    comm->uart_dev = DEVICE_DT_GET(DT_PROP(DT_PATH(zephyr_user), wfm_out_uart));
    if(!device_is_ready(comm->uart_dev))
    {
        LOG_ERR("uart device not ready [%d]\n", -ENODEV);
        return;
    }
#endif // CONFIG_EDS_COMM_WFM_OUT

    LOG_INF("eds comm can ok\n");

    k_timer_start(&comm->send_timer, K_MSEC(125), K_MSEC(125));

    /** return to normal priority */
    k_thread_priority_set(k_current_get(), EDS_COMM_THREAD_PRIO);

    while (true)
    {
        /** wait for send_timer to trigger send */
		k_timer_status_sync(&comm->send_timer);
        k_mutex_lock(&comm->lock, K_FOREVER);

        /** send can frame to set drive_mode */
        frame.id = eds_comm_msg_driveMode;
        frame.data_32[0] = comm->state.drive_mode;
        frame.dlc = 4U;
        ret = EDS_CommCanBusSend(comm, &frame);
        LOG_INF("send driveMode %d", comm->state.drive_mode);

        /** send can frame to set curr_speed */
        EDS_CommKineticsModelUpdate(comm);
        frame.id = eds_comm_msg_currSpeed;
        frame.data_32[0] = comm->state.curr_speed;
        frame.dlc = 4U;
        ret = EDS_CommCanBusSend(comm, &frame);
        LOG_INF("send currSpeed %d", comm->state.curr_speed);

        k_mutex_unlock(&comm->lock);
    }

}


eds_commSeq_t const seq_anim1[] =
{
    EDS_COMM_SEQ_SEND(driveMode, EDS_DRIVEMODE_GEN(normal, off, na, off), 0), // normal, off, off, off
    EDS_COMM_SEQ_SEND(currSpeed, 0, 0),
    EDS_COMM_SEQ_SEND(timeSync, 0, 0),
    EDS_COMM_SEQ_SLEEP(3000),

    EDS_COMM_SEQ_SEND(currSpeed, 1895, 0),
    EDS_COMM_SEQ_SEND(timeSync, 0, 0),
    EDS_COMM_SEQ_SLEEP(250),

    EDS_COMM_SEQ_SEND(currSpeed, 3650, 0),
    EDS_COMM_SEQ_SEND(timeSync, 0, 0),
    EDS_COMM_SEQ_SLEEP(250),

    EDS_COMM_SEQ_SEND(currSpeed, 10350, 0),
    EDS_COMM_SEQ_SEND(timeSync, 0, 0),
    EDS_COMM_SEQ_SLEEP(250),

    EDS_COMM_SEQ_SEND(currSpeed, 12457, 0),
    EDS_COMM_SEQ_SEND(timeSync, 0, 0),
    EDS_COMM_SEQ_SLEEP(250),

    EDS_COMM_SEQ_SEND(currSpeed, 13106, 0),
    EDS_COMM_SEQ_SEND(timeSync, 0, 0),
    EDS_COMM_SEQ_SLEEP(250),

    EDS_COMM_SEQ_SEND(currSpeed, 10507, 0),
    EDS_COMM_SEQ_SEND(timeSync, 0, 0),
    EDS_COMM_SEQ_SLEEP(250),

    EDS_COMM_SEQ_SEND(driveMode, EDS_DRIVEMODE_GEN(sport, day, na, dimm), 0), // sport day, off, dimm
    EDS_COMM_SEQ_SEND(currSpeed, 11306, 0),
    EDS_COMM_SEQ_SEND(timeSync, 0, 0),
    EDS_COMM_SEQ_SLEEP(250),

    EDS_COMM_SEQ_SEND(currSpeed, 14601, 0),
    EDS_COMM_SEQ_SEND(timeSync, 0, 0),
    EDS_COMM_SEQ_SLEEP(250),

    EDS_COMM_SEQ_SEND(currSpeed, 19480, 0),
    EDS_COMM_SEQ_SEND(timeSync, 0, 0),
    EDS_COMM_SEQ_SLEEP(250),

    EDS_COMM_SEQ_SEND(currSpeed, 25433, 0),
    EDS_COMM_SEQ_SEND(timeSync, 0, 0),
    EDS_COMM_SEQ_SLEEP(250),

    EDS_COMM_SEQ_ABORT,
};
