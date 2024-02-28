/*
 * Copyright 2023, NXP
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
#include <lvgl.h>

#include "../generated/gui_guider.h"
#include "../generated/events_init.h"

#include "control.h"
#include "custom.h"

LOG_MODULE_REGISTER(main, CONFIG_EDC_MAIN_LOG_LEVEL);

#ifdef CONFIG_EDC_TIMING_GPIO
static struct gpio_dt_spec timing_gpio =
		GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), timing_gpios);
#endif // CONFIG_EDC_TIMING_GPIO

k_tid_t tid_main;
lv_ui guider_ui;

edc_ctrl_t edc_ctrl;
edc_dataModel_t edc_model;

K_THREAD_STACK_DEFINE(edc_ctrl_thread_stack, EDC_CTRL_THREAD_STACK_SIZE);
struct k_thread edc_ctrl_thread;

void board_enable_backlight(void)
{
	const uint8_t cmd_data[] = {0x0d, 0xbf};
	const uint8_t io_exp_addr = 0x22;
	const struct device *i2c_dev = DEVICE_DT_GET(DT_N_S_soc_S_i2c_44350000);

	if(!device_is_ready(i2c_dev))
	{
		LOG_ERR("I2C: Device not ready.");
	}

	i2c_write(i2c_dev, cmd_data, 2, io_exp_addr);

}

int main(void)
{
	int ret;
	struct k_timer timer;
	tid_main = k_current_get();

	printk("**** ebike digital cluster demo ****\n"
	);

#ifdef CONFIG_EDC_TIMING_GPIO
	if (timing_gpio.port && !device_is_ready(timing_gpio.port)) {
		ret = -ENODEV;
		LOG_ERR("Error %d: timing_gpio device %s is not ready\n",
		       ret, timing_gpio.port->name);
		return ret;
	}

	ret = gpio_pin_configure_dt(&timing_gpio, GPIO_OUTPUT_ACTIVE);
	if (ret != 0) {
		LOG_ERR("Error %d: failed to configure timing_gpio at device %s pin %d\n",
		       ret, timing_gpio.port->name, timing_gpio.pin);
		return ret;
	} else {
		LOG_INF("Set up timing_gpio at device %s pin %d\n", timing_gpio.port->name, timing_gpio.pin);
	}
#endif // CONFIG_EDC_TIMING_GPIO

	const struct device *display_dev;

	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device not ready, aborting test");
		return -ENODEV;
	}

	/** fill: 0x27, 0x1d, 0x16, 0xff */
	extern uint32_t _bg1_alpha_1280x800_map_filled[];
	for(int i = 0; i < (1280 * 800); ++i)
	{
		_bg1_alpha_1280x800_map_filled[i] = 0xff161d27U;
	}

	setup_ui(&guider_ui);
   	events_init(&guider_ui);
	custom_init(&guider_ui);

	lv_task_handler();
	//display_blanking_off(display_dev);
	board_enable_backlight();

	LOG_INF("lvgl display output started");
#ifdef CONFIG_EDC_TIMING_GPIO
	ret = gpio_pin_toggle_dt(&timing_gpio);
	if(ret)	{
		LOG_ERR("toggle timing_gpio err [%d]", ret);
		return ret;
	}
#endif // CONFIG_EDC_TIMING_GPIO

	/**
	 * create edc_ctrl thread as cooperative thread, then
	 * yield() to let it run first. It will set itself to
	 * a lower priority preemptive thread once initialized.
	 */
	k_thread_create(&edc_ctrl_thread, edc_ctrl_thread_stack,
        EDC_CTRL_THREAD_STACK_SIZE,
        (k_thread_entry_t)EDC_CtrlTask, (void*)&edc_ctrl, (void*)&edc_model, NULL,
        EDC_CTRL_THREAD_START_PRIO, 0, K_NO_WAIT
    );
	k_yield();

	LOG_INF("main thread resume");
	k_timer_init(&timer, NULL, NULL);
	k_timer_start(&timer, K_USEC(5000), K_USEC(5000));

	while (true)
	{
		/** wait for timer to trigger display */
		k_timer_status_sync(&timer);
		lv_task_handler();
	}

	return 0;
}
