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

eds_comm_t eds_comm;
K_THREAD_STACK_DEFINE(eds_comm_thread_stack, EDS_COMM_THREAD_STACK_SIZE);
struct k_thread eds_comm_thread;

int main(void)
{
	int ret = 0;
	tid_main = k_current_get();

	LOG_INF("**** ebike data source demo ****\n"
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

	while (true)
	{
		k_sleep(K_MSEC(1000));

	}

	return 0;
}
