#include "model.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(edc_model, CONFIG_EDC_MODEL_LOG_LEVEL);

int32_t EDC_DataModelInit(edc_dataModel_t * const model)
{
    memset(model, 0U, sizeof(edc_dataModel_t));
    k_event_init(&model->event);
    sys_slist_init(&model->eventSubList);
    model->eventSubMask = 0U;
    k_mutex_init(&model->lock);
    k_sem_init(&model->ts.is_free, 1, 1);
    return 0U;
}

void EDC_DataModelUpdate(edc_dataModel_t * const model, edc_data_t const * const data, uint32_t flag)
{
    assert(model);
    assert(data);
    //assert(flag);
    k_mutex_lock(&model->lock, K_FOREVER);

    if (flag & edc_dataFlag_modeFlag)
    {
        EDC_DATAMODEL_UPDATE_MEMBER_FROM_DATA(model, data, modeFlag);
        if (model->shadow.modeData.driveMode == edc_driveMode_off
            && data->modeData.driveMode != edc_driveMode_off)
        {
            model->data.currentSpeed = model->shadow.currentSpeed = 0;
            model->data.averageSpeed = model->shadow.averageSpeed = 0;
            model->data.distance = model->shadow.distance = 0U;
            model->data.elapsedTime = model->shadow.elapsedTime = 0U;
        }
    }
    if (flag & edc_dataFlag_currentSpeed)
    {
        EDC_DATAMODEL_UPDATE_MEMBER_FROM_DATA(model, data, currentSpeed);

        if(model->shadow.modeData.driveMode != edc_driveMode_off)
        {
            uint64_t delta_elapsedTime /** ms */ = 125; // 125ms
            uint64_t new_elapsedTime /** ms */ = model->data.elapsedTime + delta_elapsedTime;
            EDC_DATAMODEL_UPDATE_MEMBER(model, elapsedTime, new_elapsedTime);

            uint64_t delta_distance /** mm */ = ((double)delta_elapsedTime) * ((double)model->data.currentSpeed) / 3600.0;
            uint32_t new_distance /** mm */ = model->data.distance + delta_distance;
            EDC_DATAMODEL_UPDATE_MEMBER(model, distance, new_distance);

            int32_t new_averageSpeed;
            if (new_elapsedTime != 0)
            {
                new_averageSpeed = ((double)new_distance * 3600) / ((double)new_elapsedTime);
            }
            else
            {
                new_averageSpeed = 0;
            }
            EDC_DATAMODEL_UPDATE_MEMBER(model, averageSpeed, new_averageSpeed);
        }
    }

    k_mutex_unlock(&model->lock);
}

int32_t EDC_DataModelSubscribe(edc_dataModelSub_t * const sub,
                                       edc_dataModel_t * const model)
{
    assert(model);
    assert(sub);
    if (sub->dataModel != NULL)
        return -EPERM;
    k_mutex_lock(&model->lock, K_FOREVER);

    bool event_bit_found = false;
    for (uint32_t i = 0U; i < 32U; ++i)
    {
        if (0U == ((model->eventSubMask >> i) & 1U))
        {
            model->eventSubMask |= (1U << i);
            sub->event_bit = i;
            event_bit_found = true;
            break;
        }
    }
    if (event_bit_found == false)
    {
        k_mutex_unlock(&model->lock);
        LOG_ERR("thread %s sub fail", k_thread_name_get(k_current_get()));
        return -EAGAIN;
    }

    sub->dataModel = model;
    sys_slist_append(&model->eventSubList, &sub->node);
    k_mutex_unlock(&model->lock);

    LOG_DBG("thread %s sub with event_bit %d", k_thread_name_get(k_current_get()), sub->event_bit);
    return 0U;
}

int32_t EDC_DataModelUnsubscribe(edc_dataModelSub_t * const sub)
{
    assert(sub);

    if (sub->dataModel == NULL)
        return -EINVAL;
    edc_dataModel_t * model = sub->dataModel;

    k_mutex_lock(&model->lock, K_FOREVER);
    //FIXME: what if a thread is waiting on this event?
    //optionally use high 16 bits for termination indicator
    bool remove = sys_slist_find_and_remove(&model->eventSubList, &sub->node);
    if(remove == false)
    {
        k_mutex_unlock(&model->lock);
        LOG_ERR("thread %s unsub failed", k_thread_name_get(k_current_get()));
        return -ENODATA;
    }

    model->eventSubMask &= ~(1U << sub->event_bit);
    sub->dataModel = NULL;
    k_mutex_unlock(&model->lock);
    LOG_DBG("thread %s unsub event_bit %d", k_thread_name_get(k_current_get()), sub->event_bit);
    return 0U;
}

int32_t EDC_DataModelEventWait(edc_dataModelSub_t * const sub, k_timeout_t timeout)
{
    LOG_DBG("thread %s wait event 0x%8.8x"
            , k_thread_name_get(k_current_get())
            , (uint32_t)(&sub->dataModel->event));
    int32_t ret = k_event_wait(&sub->dataModel->event, (1U << sub->event_bit), true, timeout);
    if(ret)
    {
        LOG_DBG("thread %s get event 0x%8.8x"
                , k_thread_name_get(k_current_get())
                , (uint32_t)(&sub->dataModel->event));
    }
    else
    {
        LOG_ERR("thread %s wait event 0x%8.8x timeout"
                , k_thread_name_get(k_current_get())
                , (uint32_t)(&sub->dataModel->event));
    }
    return ret;
}

void EDC_DataModelPublish(edc_dataModel_t * const model)
{
#if (KERNEL_VERSION_NUMBER >= 0x030400)
    // k_event_test is only available after zephyr 3.4.0.
    // FIXME these code are not tested!
    uint32_t pending_task = k_event_test(&model->event, model->eventSubMask);
    if(0U != pending_task)
    {
        LOG_DBG("W dm event is not fully cleared, 0x%x\n", pending_task);
    }
#endif // ! KERNEL_VERSION_NUMBER

    LOG_DBG("event 0x%8.8x publish 0x%8.8x from thread %s",
           (uint32_t)(&model->event), model->eventSubMask,
           k_thread_name_get(k_current_get()));

    //k_mutex_lock(&model->lock, K_FOREVER);
    k_event_post(&model->event, model->eventSubMask);
    //k_mutex_unlock(&model->lock);
}

int EDC_DataModelTimeSyncPrepare(edc_dataModel_t * const model, uint32_t* can_frame_data, uint8_t *dlc)
{
    int ret = k_sem_take(&model->ts.is_free, K_NO_WAIT);
    if(ret)
    {
        LOG_ERR("another time sync task already in progress! [%d]", ret);
        return ret;
    }
    model->ts.t0 = k_cyc_to_us_near32(k_cycle_get_64());
    can_frame_data[0] = model->ts.t0;
    *dlc = 4U;
    return 0;
}

void EDC_DataModelTimeSyncCalculate(edc_dataModel_t * const model, uint32_t* can_frame_data)
{
    model->ts.t3 = k_cyc_to_us_near32(k_cycle_get_64());
    model->ts.t1 = can_frame_data[0];
    model->ts.t2 = can_frame_data[1];

    int64_t t0 = model->ts.t0;
    int64_t t1 = model->ts.t1;
    int64_t t2 = model->ts.t2;
    int64_t t3 = model->ts.t3;

    uint32_t new_ping = (t3 - t0) - (t2 - t1); /** in us */
    EDC_DATAMODEL_UPDATE_MEMBER(model, timeSync.ping, new_ping);

    int32_t new_offset = ((t1 - t0) + (t2 - t3)) / 2; /** in us */
    EDC_DATAMODEL_UPDATE_MEMBER(model, timeSync.offset, new_offset);

    LOG_DBG("timeSync ping = %d us, offset = %d us", new_ping, new_offset);

    k_sem_give(&model->ts.is_free);
}
