#include "comm.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(eds_control, 4U);

struct can_filter const eds_comm_canRxFilter[] =
{
    {
        .flags = CAN_FILTER_DATA,
        .id = eds_comm_msg_timeSync,
        .mask = CAN_STD_ID_MASK,
    },
};

void EDS_CommCanBusRxCb(struct device const *const dev, struct can_frame *frame, void *userData)
{

    eds_comm_t * const comm = (eds_comm_t*)userData;
    LOG_DBG("can rx frame: id=0x%3.3x, dlc=%d\n",
           frame->id, frame->dlc);

    switch (frame->id)
    {
    case eds_comm_msg_timeSync:
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
        LOG_DBG("can device not ready [%d]\n", -ENODEV);
        return -ENODEV;
    }

    struct can_timing timing;
    ret = can_stop(comm->can_dev);
    ret = can_calc_timing(comm->can_dev, &timing, 1000000, 875);
    ret = can_set_timing(comm->can_dev, &timing);
#ifdef CONFIG_CAN_FD_MODE
    ret = can_calc_timing_data(comm->can_dev, &timing, 8000000, 875);
    ret = can_set_timing_data(comm->can_dev, &timing);
#endif // CONFIG_CAN_FD_MODE
    ret = can_start(comm->can_dev);
    if (ret != 0)
    {
        LOG_DBG("set can timing fail [%d]\n", ret);
        return ret;
    }

    uint32_t const filter_count = sizeof(eds_comm_canRxFilter) / sizeof(struct can_filter);
    for(uint32_t i = 0U; i < filter_count; ++i)
    {
        ret = can_add_rx_filter(comm->can_dev, EDS_CommCanBusRxCb, comm, &eds_comm_canRxFilter[i]);
        if(comm->can_filterId < 0)
        {
            LOG_DBG("add can filter fail [%d]\n", ret);
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
                LOG_ERR("send fail at seq % [%d]", len, ret);
            }
            break;
        default:
            frame.id = seq[len].msg;
            memcpy(&frame.data, seq[len].data, 8U);
            frame.dlc = 8U;
            ret = EDS_CommCanBusSend(comm, &frame);
            if (ret)
            {
                LOG_ERR("send fail at seq % [%d]", len, ret);
            }
            break;
        }

        ++len;
    }

    return -EINVAL; // should never reach here
}

void EDS_CommTask(eds_comm_t *const comm, void* p2, void* p3)
{
    int32_t ret;

    LOG_INF("eds comm task\n");
    comm->thread_id = k_current_get();

    ret = EDS_CommCanBusConfig(comm);
    if(ret != 0)
    {
        return;
    }

    LOG_INF("eds comm can ok\n");

    while (true)
    {
        extern eds_commSeq_t const seq_anim1[];
        k_thread_suspend(comm->thread_id);
        EDS_CommSendSequence(comm, &seq_anim1);
    }

}

eds_comm_t eds_comm;
#define EDS_COMM_THREAD_PRIO (5U)
#define EDS_COMM_THREAD_STACK_SIZE (0x4000U)
K_THREAD_DEFINE(eds_control_task, EDS_COMM_THREAD_STACK_SIZE,
                EDS_CommTask, &eds_comm, NULL, NULL,
                EDS_COMM_THREAD_PRIO, 0, 0);


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
