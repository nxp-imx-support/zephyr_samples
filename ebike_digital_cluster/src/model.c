#include "model.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(edc_model, 4U);

#define EDC_DATAMODEL_UPDATE_DATA(model, new_data, member) \
    { \
        model->shadow.member = model->data.member; \
        model->data.member = new_data->member; \
    }

int32_t EDC_DataModelInit(edc_dataModel_t * const model)
{
    memset(model, 0U, sizeof(edc_dataModel_t));
    k_event_init(&model->event);
    sys_slist_init(&model->eventSubList);
    model->eventSubMask = 0U;
    return 0U;
}

void EDC_DataModelUpdate(edc_dataModel_t * const model, edc_data_t const * const data, uint32_t flag)
{
    assert(model);
    assert(data);
    //assert(flag);
    k_spinlock_key_t key = k_spin_lock(&model->lock);

    if (flag & edc_dataFlag_modeFlag)
    {
        EDC_DATAMODEL_UPDATE_DATA(model, data, modeFlag);
    }
    if (flag & edc_dataFlag_CurrentSpeed)
    {
        EDC_DATAMODEL_UPDATE_DATA(model, data, currentSpeed);
    }
    if (flag & edc_dataFlag_averageSpeed)
    {
        EDC_DATAMODEL_UPDATE_DATA(model, data, averageSpeed);
    }
    if (flag & edc_dataFlag_distance)
    {
        EDC_DATAMODEL_UPDATE_DATA(model, data, distance);
    }

    k_spin_unlock(&model->lock, key);
}

int32_t EDC_DataModelSubscribe(edc_dataModelSub_t * const sub,
                                       edc_dataModel_t * const model)
{
    assert(model);
    assert(sub);
    if (sub->dataModel != NULL)
        return -EPERM;
    k_spinlock_key_t key = k_spin_lock(&model->lock);

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
        k_spin_unlock(&model->lock, key);
        return -EAGAIN;
    }

    sub->dataModel = model;
    sys_slist_append(&model->eventSubList, &sub->node);
    k_spin_unlock(&model->lock, key);
    return 0U;
}

int32_t EDC_DataModelUnsubscribe(edc_dataModelSub_t * const sub)
{
    assert(sub);

    if (sub->dataModel == NULL)
        return -EINVAL;
    edc_dataModel_t * model = sub->dataModel;

    k_spinlock_key_t key = k_spin_lock(&model->lock);
    //FIXME: what if a thread is waiting on this event?
    //optionally use high 16 bits for termination indicator
    bool remove = sys_slist_find_and_remove(&model->eventSubList, &sub->node);
    if(remove == false)
    {
        k_spin_unlock(&model->lock, key);
        return -ENODATA;
    }

    model->eventSubMask &= ~(1U << sub->event_bit);
    sub->dataModel = NULL;
    k_spin_unlock(&model->lock, key);
    return 0U;
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

    LOG_DBG("dm event 0x%8.8x publish 0x%x from thread %s\n",
           (uint32_t)(&model->event), model->eventSubMask,
           k_thread_name_get(k_current_get()));

    //k_spinlock_key_t key = k_spin_lock(&model->lock);
    k_event_post(&model->event, model->eventSubMask);
    //k_spin_unlock(&model->lock, key);
    LOG_DBG("dm publish done\n");
}
