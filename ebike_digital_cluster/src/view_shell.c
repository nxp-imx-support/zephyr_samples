#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <zephyr/shell/shell.h>
#include <zephyr/drivers/can.h>

#include "control.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(view_shell, 4U);

extern edc_ctrl_t edc_ctrl;
extern edc_dataModel_t edc_model;
edc_dataModel_t *model = &edc_model;

char const * const cmd_stringLut_driveMode[] =
{
    "Power OFF",
    "Safety lock",
    "ECO Mode",
    "Normal Mode",
    "Sport Mode",
    "Rain Mode",
};

char const * const cmd_stringLut_mainLight[] =
{
    "OFF",
    "D--",
    "XX-",
    "X<<",
};

char const * const cmd_stringLut_turnLight[] =
{
    "OFF",
    "D--",
    "XX-",
    "X<<",
};

char const * const cmd_stringLut_rearLight[] =
{
    "OFF",
    "DIM",
    "BRT",
};

//void EDC_ViewShellCreateArgList(va_list *arg, ...)
//{
//    va_list myArg;
//    //va_start(myArg, arg);
//    //va_copy(*arg, myArg);
//    //va_end(myArg);
//}
//
//void EDC_ViewShellPrintStatus(char const *fmt, va_list *arg)
//{
//    fmt = "Ebike digital cluster status\n"
//          "thread id: 0x%lx\n"
//          "event subscription: 0x%lx";
//    EDC_ViewShellCreateArgList(arg, model->thread_id, model->eventSubMask);
//}

static int cmd_edc_status(const struct shell *sh, size_t argc, char **argv)
{
	shell_print(sh, "Ebike digital cluster status");
    shell_print(sh, "event subscription: 0x%x", model->eventSubMask);

	return 0;
}

static int cmd_edc_show(const struct shell *sh, size_t argc, char **argv)
{
	shell_print(sh, "Ebike digital cluster data");
    shell_print(sh, "Drive mode: %s", cmd_stringLut_driveMode[model->data.modeData.driveMode]);
    shell_print(sh, "Light: Main %s, Rear %s, Turn %s",
            cmd_stringLut_mainLight[model->data.modeData.mainLight],
            cmd_stringLut_rearLight[model->data.modeData.turnLight],
            cmd_stringLut_turnLight[model->data.modeData.rearLight]);
    shell_print(sh, "Speed: %d.%d km/h", model->data.currentSpeed / 1000, model->data.currentSpeed % 1000);
    shell_print(sh, "Average speed: %d.%d km/h", model->data.averageSpeed / 1000, model->data.averageSpeed % 1000);
	shell_print(sh, "Distance: %d.%d km", model->data.distance / 1000, model->data.distance % 1000);
    return 0;
}


void EDC_CtrlCanBusRxCb(struct device const *const dev, struct can_frame *frame, void *userData);

static int cmd_edc_can_sim(const struct shell *sh, size_t argc, char **argv)
{

    struct can_frame frame;
    uint32_t max_id = CAN_MAX_STD_ID;
	int argidx = 1;
	uint32_t val;
	char *endptr;
	int nbytes;

    /* Defaults */
    frame.flags = 0U;
	frame.dlc = 0;

    /* Parse CAN ID */
	if (argidx >= argc) {
		shell_error(sh, "missing CAN ID parameter");
		shell_help(sh);
		return SHELL_CMD_HELP_PRINTED;
	}

	val = (uint32_t)strtoul(argv[argidx++], &endptr, 16);
	if (*endptr != '\0') {
		shell_error(sh, "failed to parse CAN ID");
		return -EINVAL;
	}

	if (val > max_id) {
		shell_error(sh, "CAN ID 0x%0*x out of range",
			    (frame.flags & CAN_FRAME_IDE) != 0 ? 8 : 3,
			    val);
		return -EINVAL;
	}

	frame.id = val;

    nbytes = argc - argidx;
	if (nbytes > ARRAY_SIZE(frame.data)) {
		shell_error(sh, "excessive amount of data (%d bytes)", nbytes);
		return -EINVAL;
	}

	frame.dlc = can_bytes_to_dlc(nbytes);

    /* Parse data */
	for (uint32_t i = 0; i < nbytes; i++) {
		val = (uint32_t)strtoul(argv[argidx++], &endptr, 16);
		if (*endptr != '\0') {
			shell_error(sh, "failed to parse data %s", argv[argidx++]);
			return -EINVAL;
		}

		if (val > 0xff) {
			shell_error(sh, "data 0x%x out of range", val);
			return -EINVAL;
		}

		frame.data[i] = val;
	}

    EDC_CtrlCanBusRxCb(NULL, &frame, &edc_ctrl);

    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(edc_shell_cmds,
	SHELL_CMD_ARG(status, NULL,
		"Get Ebike digital cluster status\n"
		"Usage: edc status",
		cmd_edc_status, 1, 0),
	SHELL_CMD_ARG(show, NULL,
		"Show Ebike digital cluster data model\n"
		"Usage: edc show",
		cmd_edc_show, 1, 0),
    SHELL_CMD_ARG(can_sim, NULL,
		"Simulate data received via CAN interface\n"
        "Usage: edc can_sim <CAN ID> [data] [...]",
		cmd_edc_can_sim, 2, SHELL_OPT_ARG_CHECK_SKIP),
	SHELL_SUBCMD_SET_END
);
SHELL_CMD_REGISTER(edc, &edc_shell_cmds, "Ebike digital culster commands", NULL);

void EDC_ViewShellTask(edc_ctrl_t * const ctrl, void*, void*)
{
    int32_t ret;
    edc_dataModelSub_t dm_sub;
    dm_sub.dataModel = NULL;

    k_thread_suspend(k_current_get());

    LOG_INF("subscribe model\n");
    ret = EDC_DataModelSubscribe(&dm_sub, ctrl->model);
    if (ret != 0U)
    {
        LOG_INF("subscribe fail %d\n", ret);
        return;
    }
    LOG_INF("subscribe ok\n");

    while (true)
    {
        EDC_DataModelEventWait(&dm_sub, K_FOREVER);
        LOG_INF("edc data:\n");
        LOG_INF("Drive mode: %s\n", cmd_stringLut_driveMode[dm_sub.dataModel->data.modeData.driveMode]);
        LOG_INF("Light: Main %s, Rear %s, Turn %s\n",
                cmd_stringLut_mainLight[dm_sub.dataModel->data.modeData.mainLight],
                cmd_stringLut_rearLight[dm_sub.dataModel->data.modeData.turnLight],
                cmd_stringLut_turnLight[dm_sub.dataModel->data.modeData.rearLight]);
        LOG_INF("Speed: %d.%d km/h\n", dm_sub.dataModel->data.currentSpeed / 1000, dm_sub.dataModel->data.currentSpeed % 1000);
        LOG_INF("Average speed: %d.%d km/h\n", model->data.averageSpeed / 1000, model->data.averageSpeed % 1000);
	    LOG_INF("Distance: %d.%d km\n", dm_sub.dataModel->data.distance / 1000, dm_sub.dataModel->data.distance % 1000);
    }
}

#define EDC_VIEWSHELL_THREAD_PRIO (8U)
#define EDC_VIEWSHELL_THREAD_STACK_SIZE (0x1000U)
K_THREAD_DEFINE(edc_viewShell_task, EDC_VIEWSHELL_THREAD_STACK_SIZE,
                EDC_ViewShellTask, &edc_ctrl, NULL, NULL,
                EDC_VIEWSHELL_THREAD_PRIO, 0, 0);
