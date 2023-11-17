#ifndef __EDC_MODEL_H__
#define __EDC_MODEL_H__
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <zephyr/kernel.h>
#include <zephyr/kernel_structs.h>
#include <zephyr/spinlock.h>
#include <zephyr/sys/slist.h>

typedef enum _edc_driveMode
{
    edc_driveMode_off = 0U,
    edc_driveMode_lock, // child lock / protection
    //edc_driveMode_eco,
    edc_driveMode_normal,
    //edc_driveMode_rain,
    edc_driveMode_sport,
} edc_driveMode_t;

typedef enum _edc_lightMode
{
    /** main light */
    edc_lightMode_main_off =    0b00U,
    edc_lightMode_main_day =    0b01U,
    edc_lightMode_main_on =     0b10U,
    edc_lightMode_main_max =    0b11U,
    /** turn light */
    edc_lightMode_turn_na =     0b00U,
    edc_lightMode_turn_lt =     0b10U,
    edc_lightMode_turn_rt =     0b01U,
    /** rear light */
    edc_lightMode_rear_off =    0b00U,
    edc_lightMode_rear_dimm =   0b01U,
    edc_lightMode_rear_bright = 0b10U,
} edc_lightMode_t;

typedef enum _edc_dataFlag
{
    edc_dataFlag_modeFlag = (1U <<  1),
    edc_dataFlag_currentSpeed = (1U << 2),
} edc_dataFlag_t;

//__attribute__((__packed))
struct _edc_data
{
    union
    {
        uint32_t modeFlag;
        struct
        {
            edc_driveMode_t driveMode : 3;
            edc_lightMode_t mainLight : 2;
            edc_lightMode_t turnLight : 2;
            edc_lightMode_t rearLight : 2;
        } modeData;
    };

    int32_t currentSpeed, averageSpeed /** speed in m/h */;
    uint64_t distance /** distance in mm */, elapsedTime /** time in ms */;

    struct
    {
        int32_t ping;
        int32_t offset;
    } timeSync;
};

typedef struct _edc_data edc_data_t;
typedef struct _edc_ts
{
    struct k_sem is_free;
    uint32_t t0, t1, t2, t3;
}edc_timeSync_t;

typedef struct _edc_dataModel
{
    /** service section */
    struct k_event event;
    sys_slist_t eventSubList;
    uint32_t eventSubMask;

    struct k_mutex lock;

    /** storage section */
    edc_timeSync_t ts;
    edc_data_t data, shadow;

} edc_dataModel_t;


/** edc_dataModel_t helper macro */
#define EDC_DATAMODEL_UPDATE_MEMBER(model, member, member_data) \
    { \
        model->shadow.member = model->data.member; \
        model->data.member = member_data; \
    }

#define EDC_DATAMODEL_UPDATE_MEMBER_FROM_DATA(model, new_data, member) \
    { \
        model->shadow.member = model->data.member; \
        model->data.member = new_data->member; \
    }

#define EDC_DATAMODEL_IS_MEMBER_EQUAL(model, member) \
    (model->shadow.member == model->data.member)


typedef struct _edc_dataModelSub
{
    sys_snode_t node;           /**! slist node */
    uint32_t event_bit;         /**! dataModel event bit shift */
    edc_dataModel_t* dataModel; /**! dataModel subscribed to */
} edc_dataModelSub_t;

/**
 * @brief Init dataModel for Ebike digital cluster
 *
 * @param model     Pointer to the dataModel structure to be initialized.
 * @retval 0 If successful
 */
int32_t EDC_DataModelInit(edc_dataModel_t * const model);

/**
 * @brief Update data fields inside dataModel according to flags.
 *
 * @param model     Pointer to the dataModel structure to be updated
 * @param data      Pointer to structure containing new data.
 * @param flag      Bitfield indicating which data to be update.
 */
void EDC_DataModelUpdate(edc_dataModel_t * const model, edc_data_t const * const data, uint32_t flag);

int32_t EDC_DataModelSubscribe(edc_dataModelSub_t * const sub,
                                edc_dataModel_t * const model);

int32_t EDC_DataModelUnsubscribe(edc_dataModelSub_t * const sub);

int32_t EDC_DataModelEventWait(edc_dataModelSub_t * const sub, k_timeout_t timeout);

void EDC_DataModelPublish(edc_dataModel_t * const model);

int EDC_DataModelTimeSyncPrepare(edc_dataModel_t * const model, uint32_t* can_frame_data, uint8_t *dlc);

void EDC_DataModelTimeSyncCalculate(edc_dataModel_t * const model, uint32_t* can_frame_data);

#endif //! __EDC_MODEL_H__
