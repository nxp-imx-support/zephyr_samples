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

void button_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	extern eds_comm_t eds_comm;;
	printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
	k_thread_resume(eds_comm.thread_id);
}

int main(void)
{
	int ret = 0;
	tid_main = k_current_get();

	if (!gpio_is_ready_dt(&button)) {
		LOG_ERR("Error: button device %s is not ready\n",
		       button.port->name);
		return -ENODEV;
	}

	if (led.port && !device_is_ready(led.port)) {
		LOG_INF("LED device %s is not ready; ignoring it\n",
		       led.port->name);
		led.port = NULL;
	}

	LOG_INF("**** ebike data source demo ****\n"
	);

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret != 0) {
		LOG_ERR("Error %d: failed to configure %s pin %d\n",
		       ret, button.port->name, button.pin);
		return ret;
	}
	ret = gpio_pin_interrupt_configure_dt(&button,
					      GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		LOG_ERR("%d: failed to configure interrupt on %s pin %d\n",
			ret, button.port->name, button.pin);
		return ret;
	}
	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb_data);
	LOG_INF("Set up button at %s pin %d\n", button.port->name, button.pin);

	if (led.port) {
		ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT);
		if (ret != 0) {
			LOG_ERR("%d: failed to configure LED device %s pin %d\n",
			       ret, led.port->name, led.pin);
			led.port = NULL;
		} else {
			LOG_INF("Set up LED at %s pin %d\n", led.port->name, led.pin);
		}
	}

	while (true)
	{
		k_sleep(K_MSEC(1000));

	}

	return 0;
}
