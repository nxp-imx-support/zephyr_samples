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
#include <lvgl.h>

#include "../generated/gui_guider.h"
#include "../generated/events_init.h"

#include "control.h"
#include "custom.h"

LOG_MODULE_REGISTER(main, 4U);

#define SW0_NODE DT_ALIAS(sw0)
#if !DT_NODE_HAS_STATUS(SW0_NODE, okay)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios,
							      {0});
static struct gpio_callback button_cb_data;

static struct gpio_dt_spec led = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios,
						     {0});

k_tid_t tid_main;
lv_ui guider_ui;

edc_ctrl_t edc_ctrl;
edc_dataModel_t edc_model;

K_THREAD_STACK_DEFINE(edc_ctrl_thread_stack, EDC_CTRL_THREAD_STACK_SIZE);
struct k_thread edc_ctrl_thread;

void button_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	extern edc_ctrl_t edc_ctrl;
	printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
	k_thread_resume(edc_ctrl.thread_id);
	//EDC_DataModelPublish(&edc_model);
}

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
	tid_main = k_current_get();

	if (!gpio_is_ready_dt(&button)) {
		printk("Error: button device %s is not ready\n",
		       button.port->name);
		return -ENODEV;
	}

	if (led.port && !device_is_ready(led.port)) {
		ret = -ENODEV;
		printk("Error %d: LED device %s is not ready; ignoring it\n",
		       ret, led.port->name);
		led.port = NULL;
	}

	printk("**** ebike digital cluster demo ****\n"
	);

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       ret, button.port->name, button.pin);
		return ret;
	}
	ret = gpio_pin_interrupt_configure_dt(&button,
					      GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			ret, button.port->name, button.pin);
		return ret;
	}
	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb_data);
	printk("Set up button at %s pin %d\n", button.port->name, button.pin);

	if (led.port) {
		ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT);
		if (ret != 0) {
			printk("Error %d: failed to configure LED device %s pin %d\n",
			       ret, led.port->name, led.pin);
			led.port = NULL;
		} else {
			printk("Set up LED at %s pin %d\n", led.port->name, led.pin);
		}
	}

	const struct device *display_dev;

	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device not ready, aborting test");
		return -ENODEV;
	}

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

	setup_ui(&guider_ui);
   	events_init(&guider_ui);
	custom_init(&guider_ui);

	lv_task_handler();
	//display_blanking_off(display_dev);
	board_enable_backlight();

	LOG_INF("main ok");

	while (true)
	{
		lv_task_handler();
		k_sleep(K_MSEC(10));
	}

	return 0;
}
