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
    edc_driveMode_eco,
    edc_driveMode_normal,
    edc_driveMode_rain,
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
    edc_dataFlag_CurrentSpeed = (1U << 2),
    edc_dataFlag_averageSpeed = (1U << 3),
    edc_dataFlag_distance = (1U << 4),
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

    int32_t currentSpeed, averageSpeed;
    int32_t distance;
    uint64_t timestamp; // TODO
};

typedef struct _edc_data edc_data_t;

#endif //! __EDC_MODEL_H__
