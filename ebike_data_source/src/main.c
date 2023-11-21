/*
 * Copyright (c) 2023 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/can.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "comm.h"

LOG_MODULE_REGISTER(main, CONFIG_EDS_MAIN_LOG_LEVEL);

#ifdef CONFIG_EDC_USER_INPUT
BUILD_ASSERT(DT_PROP_LEN(DT_PATH(zephyr_user), keypad_r_gpios) == 4U,
	"keypad_gpio_r must have 4 elements");
const struct gpio_dt_spec keypad_gpio_r[] =
{
	DT_FOREACH_PROP_ELEM_SEP(DT_PATH(zephyr_user), keypad_r_gpios,
                                     GPIO_DT_SPEC_GET_BY_IDX, (,))
};

BUILD_ASSERT(DT_PROP_LEN(DT_PATH(zephyr_user), keypad_c_gpios) == 4U,
	"keypad_gpio_c must have 4 elements");
const struct gpio_dt_spec keypad_gpio_c[] =
{
	DT_FOREACH_PROP_ELEM_SEP(DT_PATH(zephyr_user), keypad_c_gpios,
                                     GPIO_DT_SPEC_GET_BY_IDX, (,))
};
#endif // CONFIG_EDC_USER_INPUT

typedef struct _keypad_4x4
{
	const struct gpio_dt_spec *gpio_r;
	const struct gpio_dt_spec *gpio_c;
	uint16_t shadow, data;
}keypad_4x4_t;

int keypad_4x4_init(keypad_4x4_t *kp)
{
	int ret = 0;
	for (int i = 0; i < 4; ++i)
	{
		ret = gpio_pin_configure_dt(&kp->gpio_r[i], GPIO_OUTPUT_ACTIVE);
		if(ret)
			LOG_ERR("keypad init fail R%d", i);
		ret = gpio_pin_configure_dt(&kp->gpio_c[i], GPIO_INPUT);
		if(ret)
			LOG_ERR("keypad init fail C%d", i);
	}
	return 0;
}

void keypad_4x4_select_row(keypad_4x4_t *kp, uint8_t row_mask)
{
	for (int i = 0; i < 4; ++i)
	{
		if(row_mask & BIT(i))
		{
			gpio_pin_set_dt(&kp->gpio_r[i], 1);
		}
		else
		{
			gpio_pin_set_dt(&kp->gpio_r[i], 0);
		}
	}
}

void keypad_4x4_scan(keypad_4x4_t *kp)
{
	kp->shadow = kp->data;
	kp->data = 0U;

	LOG_DBG("keypad scan");

	for (int i = 0; i < 4; ++i)
	{
		keypad_4x4_select_row(kp, BIT(i));
		k_busy_wait(100);
		for (int j = 0; j < 4; ++j)
		{
			if(gpio_pin_get_dt(&kp->gpio_c[j]))
			{
				kp->data |= BIT(i * 4 + j);
				LOG_DBG("keypad %2.2d pressed", i * 4 + j);
			}
		}
	}
	keypad_4x4_select_row(kp, 0b1111);
}

#define KEYPAD_FOREACH_KEY(keys_u16, state_b, op) \
	for (int keys_u16##_i = 0; keys_u16##_i < 16; ++keys_u16##_i) { \
		if ((keys_u16 >> keys_u16##_i) & 0x1 == state_b) { \
			op; \
		} \
	}


k_tid_t tid_main;

#ifdef CONFIG_EDC_USER_INPUT
keypad_4x4_t keypad =
{
	.gpio_r = keypad_gpio_r,
	.gpio_c = keypad_gpio_c,
	.shadow = 0U,
	.data = 0U,
};
#endif // CONFIG_EDC_USER_INPUT

eds_comm_t eds_comm;
K_THREAD_STACK_DEFINE(eds_comm_thread_stack, EDS_COMM_THREAD_STACK_SIZE);
struct k_thread eds_comm_thread;

int main(void)
{
	int ret = 0;
	tid_main = k_current_get();
	struct k_timer kpScan_timer;

	printk("**** ebike data source demo ****\n"
	);

	LOG_INF("spawn eds_com thread");

	/**
	 * create eds_comm thread as cooperative thread, then
	 * yield() to let it run first. It will set itself to
	 * a lower priority preemptive thread once initialized.
	 */
	k_thread_create(&eds_comm_thread, eds_comm_thread_stack,
        EDS_COMM_THREAD_STACK_SIZE,
        (k_thread_entry_t)EDS_CommTask, (void*)&eds_comm, NULL, NULL,
        EDS_COMM_THREAD_START_PRIO, 0, K_NO_WAIT
    );
	k_yield();
	LOG_INF("main thread resume");

#ifdef CONFIG_EDC_USER_INPUT
	keypad_4x4_init(&keypad);
#endif // CONFIG_EDC_USER_INPUT

	k_timer_init(&kpScan_timer, NULL, NULL);
	k_timer_start(&kpScan_timer, K_MSEC(10), K_MSEC(10));

	while (true)
	{
#ifdef CONFIG_EDC_USER_INPUT
		static uint8_t key_hold_timer[16];
		/** only enable long press for SP and SN */
		const uint16_t key_hold_enable = eds_comm_keyCode_SP | eds_comm_keyCode_SN;
#endif // CONFIG_EDC_USER_INPUT

		/** wait for kpScan_timer to trigger scan */
		k_timer_status_sync(&kpScan_timer);

#ifdef CONFIG_EDC_USER_INPUT
		keypad_4x4_scan(&keypad);
		uint16_t key_pressed = (keypad.data ^ keypad.shadow) & keypad.data;
		KEYPAD_FOREACH_KEY(key_pressed, 1, {key_hold_timer[key_pressed_i] = 0U; })
		uint16_t key_released = (keypad.data ^ keypad.shadow) & keypad.shadow;
		uint16_t key_hold = keypad.data & keypad.shadow & key_hold_enable;
		KEYPAD_FOREACH_KEY(key_hold, 1, {
			key_hold_timer[key_hold_i] ++;
			if (key_hold_timer[key_hold_i] > 24)
			{
				key_released |= BIT(key_hold_i);
				key_hold_timer[key_hold_i] = 0U;
			}
		})
		EDS_CommKeyInput(&eds_comm, key_released);
#endif // CONFIG_EDC_USER_INPUT
	}

	return 0;
}
