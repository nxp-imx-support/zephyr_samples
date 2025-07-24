// SPDX-License-Identifier: MIT
// Copyright 2025 NXP

/**
 * @file custom.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lvgl.h"
#include "custom.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void gauge_update(lv_timer_t *timer);
static void screen_update(lv_timer_t *timer);

/**********************
 *  STATIC VARIABLES
 **********************/

/**
 * Create a demo application
 */

void custom_init(lv_ui *ui)
{
	/* Add your codes here */
	lv_timer_create(gauge_update, 50, ui);
	lv_timer_create(screen_update, 1000, ui);
}

const char *turn_dist[] = {"3.1KM", "3.0KM", "2.9KM", "2.8KM", "2.7KM", "2.6KM",
			   "2.5KM", "2.4KM", "2.3KM", "2.2KM", NULL};
const char *trav_dist[] = {"12.7", "12.8", "12.9", "13.0", "13.1", "13.2",
			   "13.3", "13.4", "13.5", "13.6", NULL};
const char *rema_dist[] = {"18.3", "18.2", "18.1", "18.0", "17.9", "17.8",
			   "17.7", "17.6", "17.5", "17.4", NULL};
const char *aver_speed[] = {"16.1", "16.2", "16.1", "16.0", "15.9", "16.0",
			    "16.1", "16.2", "16.3", "16.2", NULL};
const char *elap_time[] = {"46:28", "46:29", "46:30", "46:31", "46:32", "46:33",
			   "46:34", "46:35", "46:36", "46:37", NULL};
const char *elev_gain[] = {"25.1", "25.3", "25.4", "25.2", "25.0", "24.8",
			   "24.7", "25.8", "26.5", "24.2", NULL};
const char *elev_lost[] = {"10.2", "11.8", "12.0", "11.9", "14.7", "16.2",
			   "14.3", "17.5", "13.6", "11.8", NULL};
const char *elev_grap[] = {"15.3", "15.4", "15.5", "15.6", "15.6", "15.5",
			   "15.6", "15.5", "15.4", "15.3", NULL};
const char *calo_burn[] = {"462", "462", "463", "463", "464", "465",
			   "466", "466", "467", "468", NULL};
const char *heart_rate[] = {"118", "119", "120", "124", "123", "121",
			    "119", "118", "121", "124", NULL};
const char *cadence[] = {"86", "84", "83", "85", "88", "92", "96", "98", "89", "86", NULL};
const char *watts[] = {"15.7", "15.8", "15.8", "15.9", "16.0", "16.0",
		       "16.1", "16.2", "16.3", "16.4", NULL};
static int chart_val[100];
const int gauge_val[] = {27, 26, 25, 24, 23, 22, 21, 20, 19, 19, 20, 20, 21, 21, 22, 23, 24, 23, 22,
			 21, 20, 20, 19, 18, 17, 16, 15, 15, 15, 16, 17, 18, 19, 20, 21, 21, 22, 22,
			 21, 20, 19, 19, 19, 20, 21, 22, 23, 24, 25, 26, 26, 27, 28, 28, 28};
const char *speed_num_1[] = {"0",  "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  "10",
			     "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21",
			     "22", "23", "24", "25", "26", "27", "28", "29", NULL};
const char *speed_num_2[] = {"27", "26", "25", "24", "23", "22", "21", "20", "19", "19", "20",
			     "20", "21", "21", "22", "23", "24", "23", "22", "21", "20", "20",
			     "19", "18", "17", "16", "15", "15", "15", "16", "17", "18", "19",
			     "20", "21", "21", "22", "22", "21", "20", "19", "19", "19", "20",
			     "21", "22", "23", "24", "25", "26", "26", "27", "28", "28", "28"};

void screen_update(lv_timer_t *timer)
{
	static unsigned int info_index = 0;
	lv_ui *guider_ui = lv_timer_get_user_data(timer);
	int i;

	i = info_index++ % 10;

	if (lv_scr_act() == guider_ui->Overview) {
		lv_label_set_text(guider_ui->Overview_dist_label, turn_dist[i]);
		lv_label_set_text(guider_ui->Overview_info1_label_3, trav_dist[i]);
		lv_label_set_text(guider_ui->Overview_info2_label_3, aver_speed[i]);
		lv_label_set_text(guider_ui->Overview_info3_label_3, rema_dist[i]);
		lv_label_set_text(guider_ui->Overview_info4_label_3, elap_time[i]);
	}
}

void set_gauge_val(lv_ui *guider_ui)
{
	static unsigned int speed_index = 0;
	int i;

	i = speed_index++ % (sizeof(gauge_val) / sizeof(int));

	lv_img_set_angle(guider_ui->Overview_needle_img, 1500 + (gauge_val[i++]) * 60);
	lv_label_set_text(guider_ui->Overview_gauge_label_1, speed_num_2[i - 1]);
}

void gauge_update(lv_timer_t *timer)
{
	lv_ui *guider_ui = lv_timer_get_user_data(timer);
	static int gauge_cur_val = 0, dir = 0, set_val = 0;

	if (set_val) {
		set_gauge_val(guider_ui);
		return;
	}

	if (gauge_cur_val == 28) {
		dir = 1;
		set_val = 1;
	}

	if (gauge_cur_val == 0) {
		dir = 0;
	}

	if (lv_scr_act() == guider_ui->Overview) {
		if (dir == 0) {
			lv_img_set_angle(guider_ui->Overview_needle_img,
					 1500 + (gauge_cur_val++) * 60);
			lv_label_set_text(guider_ui->Overview_gauge_label_1,
					  speed_num_1[gauge_cur_val - 1]);
		} else {
			lv_img_set_angle(guider_ui->Overview_needle_img,
					 1500 + (gauge_cur_val--) * 60);
			lv_label_set_text(guider_ui->Overview_gauge_label_1,
					  speed_num_1[gauge_cur_val + 1]);
		}
	}
}

void ebkie_init(void)
{
	int cnt = 0, i = 0;

	for (cnt = 0; cnt < 20; cnt++, i++) {
		chart_val[i] = 10 + cnt;
	}

	for (cnt = 0; cnt < 10; cnt++, i++) {
		chart_val[i] = 29 - cnt;
	}

	for (cnt = 0; cnt < 10; cnt++, i++) {
		chart_val[i] = 19 + cnt;
	}

	for (cnt = 0; cnt < 20; cnt++, i++) {
		chart_val[i] = 29 - cnt;
	}

	for (cnt = 0; cnt < 10; cnt++, i++) {
		chart_val[i] = 9 + cnt;
	}

	for (cnt = 0; cnt < 10; cnt++, i++) {
		chart_val[i] = 19 - cnt;
	}

	for (cnt = 0; cnt < 10; cnt++, i++) {
		chart_val[i] = 9 + cnt;
	}

	for (cnt = 0; cnt < 10; cnt++, i++) {
		chart_val[i] = 19 - cnt;
	}
}
