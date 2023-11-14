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


k_tid_t tid_main;

{

int main(void)
{
	int ret = 0;
	tid_main = k_current_get();

	LOG_INF("**** ebike data source demo ****\n"
	);



	while (true)
	{
		k_sleep(K_MSEC(1000));

	}

	return 0;
}
