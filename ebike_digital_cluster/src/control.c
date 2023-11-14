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
    LOG_DBG("can rx frame: id=0x%3.3x, dlc=%d\n",
           frame->id, frame->dlc);

    switch (frame->id)
    {
    case edc_ctrl_canMsg_timeSync:
        break;
    case edc_ctrl_canMsg_driveMode:
        LOG_DBG("update driveMode %d\n", frame->data_32[0]);
        update_data.modeFlag = frame->data_32[0];
        EDC_DataModelUpdate(ctrl->model, &update_data, edc_dataFlag_modeFlag);
        //k_work_submit(&ctrl->pub_work);
        k_wakeup(ctrl->thread_id);
        break;
    case edc_ctrl_canMsg_currSpeed:
        LOG_DBG("update currSpeed %d\n", frame->data_32[0]);
        update_data.currentSpeed = frame->data_32[0];
        update_data.timestamp = k_uptime_get();
        EDC_DataModelUpdate(ctrl->model, &update_data, edc_dataFlag_currentSpeed);
        //k_work_submit(&ctrl->pub_work);
        k_wakeup(ctrl->thread_id);
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


K_THREAD_STACK_DEFINE(edc_viewDisplay_thread_stack, EDC_VIEWDISPLAY_THREAD_STACK_SIZE);
struct k_thread edc_viewDisplay_thread;
void EDC_ViewDisplayTask(edc_ctrl_t * const ctrl, void*, void*); // defined in custom/custom.c

void EDC_CtrlTask(edc_ctrl_t *const ctrl, edc_dataModel_t *const model, void*)
{
    assert(ctrl);
    int ret;


    LOG_INF("edc ctrl task\n");
    ctrl->thread_id = k_current_get();
    k_thread_name_set(ctrl->thread_id, "edc_ctrl");

    EDC_DataModelInit(model);
    ctrl->model = model;
    k_work_init(&ctrl->pub_work, EDC_CtrlPubWorkHandler);

    LOG_INF("edc view start ok\n");

    ret = EDC_CtrlCanBusConfig(ctrl);
    if(ret != 0)
    {
        return ret;
    }
    LOG_INF("edc ctrl can config ok\n");

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
	LOG_INF("main thread resume");

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
