/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <lvgl.h>
#include <stdio.h>
#include <string.h>
#include <zephyr/kernel.h>
#include "gui_guider.h"
#include "custom.h"
#include "events_init.h"

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app);

lv_ui guider_ui;

int main(void)
{
	const struct device *display_dev;

	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device not ready, aborting test");
		return 0;
	}
	LOG_INF("ebike lvgl demo");

	lv_timer_handler();
	display_blanking_off(display_dev);

	setup_ui(&guider_ui);
	events_init(&guider_ui);
	custom_init(&guider_ui);

	while (1) {
		lv_timer_handler();
		k_sleep(K_MSEC(10));
	}
}
