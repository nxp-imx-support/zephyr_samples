#include "control.h"
#include "custom.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(edc_control, 3U);

#ifdef CONFIG_CAN_FD_MODE
#define CTRL_CAN_FILTER_FLAGS (CAN_FILTER_FDF | CAN_FILTER_DATA)
#else
#define CTRL_CAN_FILTER_FLAGS (CAN_FILTER_DATA)
#endif // CONFIG_CAN_FD_MODE

struct can_filter const edc_ctrl_canRxFilter[] =
{
    {
        .flags = CTRL_CAN_FILTER_FLAGS,
        .id = edc_ctrl_canMsg_timeSync,
        .mask = CAN_STD_ID_MASK,
    },
    {
        .flags = CTRL_CAN_FILTER_FLAGS,
        .id = edc_ctrl_canMsg_driveMode,
        .mask = CAN_STD_ID_MASK,
    },
    {
        .flags = CTRL_CAN_FILTER_FLAGS,
        .id = edc_ctrl_canMsg_currSpeed,
        .mask = CAN_STD_ID_MASK,
    },
};

void EDC_CtrlPubWorkHandler(struct k_work * work)
{
    edc_ctrl_t *ctrl = CONTAINER_OF(work, edc_ctrl_t, pub_work);
    EDC_DataModelPublish(ctrl->model);
}

void EDC_CtrlCanBusRxCb(struct device const *const dev, struct can_frame *frame, void *userData)
{
    static edc_data_t update_data;

    edc_ctrl_t * const ctrl = (edc_ctrl_t*)userData;
    LOG_DBG("can rx frame: id=0x%3.3x, dlc=%d",
           frame->id, frame->dlc);

    switch (frame->id)
    {
    case edc_ctrl_canMsg_timeSync:
        LOG_DBG("update timeSync %d", frame->data_32[0]);
        if(frame->dlc == 8U)
        {
            EDC_DataModelTimeSyncCalculate(ctrl->model, &frame->data_32[0]);
        }
        else
        {
            LOG_ERR("timeSync: frame format error, expect dlc = 8");
        }
        break;
    case edc_ctrl_canMsg_driveMode:
        LOG_DBG("update driveMode %d", frame->data_32[0]);
        if(frame->dlc == 4U)
        {
            update_data.modeFlag = frame->data_32[0];
            EDC_DataModelUpdate(ctrl->model, &update_data, edc_dataFlag_modeFlag);
            k_wakeup(ctrl->thread_id);
        }
        else
        {
            LOG_ERR("driveMode: frame format error, expect dlc = 4");
        }
        break;
    case edc_ctrl_canMsg_currSpeed:
        LOG_DBG("update currSpeed %d", frame->data_32[0]);
        if(frame->dlc == 4U)
        {
            if (k_timer_remaining_get(&ctrl->ts_timer) == 0U)
            {
                /** timeSync not started until first currSpeed arrives */
                LOG_DBG("got first frame, enable timeSync");
                k_timer_start(&ctrl->ts_timer, K_MSEC(125), K_MSEC(125));
            }
            update_data.currentSpeed = frame->data_32[0];
            update_data.timeStamp = k_uptime_get();
            EDC_DataModelUpdate(ctrl->model, &update_data, edc_dataFlag_currentSpeed);
            k_wakeup(ctrl->thread_id);
        }
        else
        {
            LOG_ERR("currSpeed: frame format error, expect dlc = 4");
        }
        break;
    default:
        break;
    }
}

void EDC_CtrlCanBusTxCb(struct device const *const dev, int32_t error, void *userData)
{
    //edc_ctrl_t * const ctrl = (edc_ctrl_t*)userData;
    if (error != 0)
    {
        LOG_DBG("can tx error [%d]", error);
    }
    else
    {
        LOG_DBG("can tx frame");
    }
}

int32_t EDC_CtrlCanBusSend(edc_ctrl_t * const ctrl, struct can_frame * const frame)
{
    return can_send(ctrl->can_dev, frame, K_FOREVER, EDC_CtrlCanBusTxCb, ctrl);
}

int32_t EDC_CtrlCanBusConfig(edc_ctrl_t * const ctrl)
{
    int32_t ret;
    ctrl->can_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_canbus));
    if(!device_is_ready(ctrl->can_dev))
    {
        LOG_ERR("can device not ready [%d]\n", -ENODEV);
        return -ENODEV;
    }

    struct can_timing timing;
    ret = can_stop(ctrl->can_dev);
    ret = can_calc_timing(ctrl->can_dev, &timing, 1000000, 875);
    ret = can_set_timing(ctrl->can_dev, &timing);
#ifdef CONFIG_CAN_FD_MODE
    ret = can_calc_timing_data(ctrl->can_dev, &timing, 2000000, 875);
    ret = can_set_timing_data(ctrl->can_dev, &timing);
#endif // CONFIG_CAN_FD_MODE
    if (ret != 0)
    {
        LOG_ERR("set can timing fail [%d]\n", ret);
    }

    uint32_t const filter_count = sizeof(edc_ctrl_canRxFilter) / sizeof(struct can_filter);
    for(uint32_t i = 0U; i < filter_count; ++i)
    {
        ret = can_add_rx_filter(ctrl->can_dev, EDC_CtrlCanBusRxCb, ctrl, &edc_ctrl_canRxFilter[i]);
        if(ctrl->can_filterId < 0)
        {
            LOG_ERR("add can filter fail [%d]\n", ret);
            return ret;
        }
        ctrl->can_filterId[i] = ret;
    }

    return 0;
}

void EDC_CtrlTsWork(struct k_work *item)
{
    int ret;
    edc_ctrl_t *ctrl =
        CONTAINER_OF(item, edc_ctrl_t, ts_work);
    LOG_DBG("timesync work");

    static struct can_frame frame = {
#ifdef CONFIG_CAN_FD_MODE
	    .flags = CAN_FRAME_FDF | CAN_FRAME_BRS,
#else
	    .flags = 0U,
#endif // CONFIG_CAN_FD_MODE
        .id = edc_ctrl_canMsg_timeSync,
    };
    ret = EDC_DataModelTimeSyncPrepare(ctrl->model, &frame.data_32[0], &frame.dlc);
    if(ret)
    {
        LOG_ERR("ts work is blocked, timer stopped [%d]", ret);
        k_timer_stop(&ctrl->ts_timer);
        return;
    }
    ret = EDC_CtrlCanBusSend(ctrl, &frame);
    if(ret)
    {
        LOG_ERR("ts work can send fail [%d]", ret);
        return;
    }
}

void EDC_CtrlTsTimerWork(struct k_timer *item)
{
    edc_ctrl_t *ctrl =
        CONTAINER_OF(item, edc_ctrl_t, ts_timer);
    k_work_submit_to_queue(&ctrl->work_q, &ctrl->ts_work);
}

K_THREAD_STACK_DEFINE(edc_viewDisplay_thread_stack, EDC_VIEWDISPLAY_THREAD_STACK_SIZE);
struct k_thread edc_viewDisplay_thread;
void EDC_ViewDisplayTask(edc_ctrl_t * const ctrl, void*, void*); // defined in custom/custom.c

K_THREAD_STACK_DEFINE(edc_ctrl_wq_stack, EDC_CTRL_WQ_STACK_SIZE);

void EDC_CtrlTask(edc_ctrl_t *const ctrl, edc_dataModel_t *const model, void*)
{
    assert(ctrl);
    int ret;

    LOG_INF("edc ctrl task");
    ctrl->thread_id = k_current_get();
    k_thread_name_set(ctrl->thread_id, "edc_ctrl");

    EDC_DataModelInit(model);
    ctrl->model = model;
    k_work_init(&ctrl->pub_work, EDC_CtrlPubWorkHandler);

    k_work_queue_init(&ctrl->work_q);
    k_work_queue_start(&ctrl->work_q, edc_ctrl_wq_stack,
                   EDC_CTRL_WQ_STACK_SIZE, EDC_CTRL_THREAD_PRIO + 1,
                   NULL);

    k_work_init(&ctrl->ts_work, EDC_CtrlTsWork);
    k_timer_init(&ctrl->ts_timer, EDC_CtrlTsTimerWork, NULL);
    /** timeSync not started until first currSpeed arrives */

    LOG_INF("edc ctrl init ok");

    ret = EDC_CtrlCanBusConfig(ctrl);
    if(ret != 0)
    {
        return ret;
    }
    LOG_INF("edc ctrl can config ok");

	/**
	 * create edc_viewDisplay thread as cooperative thread, then
	 * yield() to let it run first. It will set itself to
	 * a lower priority preemptive thread once initialized.
	 */
	k_thread_create(&edc_viewDisplay_thread, edc_viewDisplay_thread_stack,
        EDC_VIEWDISPLAY_THREAD_STACK_SIZE,
        (k_thread_entry_t)EDC_ViewDisplayTask, (void*)ctrl, NULL, NULL,
        EDC_VIEWDISPLAY_THREAD_START_PRIO, 0, K_NO_WAIT
    );
	k_yield();

    LOG_INF("edc view start ok");

    ret = can_start(ctrl->can_dev);
    if (ret != 0)
    {
        LOG_DBG("can start fail [%d]\n", ret);
        return;
    }

    /** return to normal priority */
    k_thread_priority_set(k_current_get(), EDC_CTRL_THREAD_PRIO);

    while (true)
    {
        EDC_DataModelPublish(ctrl->model);
        k_thread_suspend(ctrl->thread_id);
    }

}
