/*
 * Copyright 2024, NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/** ZephyrOS include group */
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/cache.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/video.h>

/** C STD include group */
#include <cinttypes>
#include <cstring>
#include <cstdio>
#include <cstring>

/** CPP STD/STL include group */
#include <vector>

#include "zx_scan.h"
#include <lvgl.h>
#include "../generated/gui_guider.h"
#include "../generated/events_init.h"
#include "../custom/custom.h"


#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main_tr, CONFIG_BARCODE_MAIN_LOG_LEVEL);

const struct device *video_dev = DEVICE_DT_GET_ONE(nxp_imx_isi);
struct video_buffer *my_video_buffer_pool[5];

const struct device *lv_disp_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

K_THREAD_STACK_DEFINE(zx_scan_thread_stack, ZX_SCAN_THREAD_STACK_SIZE);
struct k_thread zx_scan_thread;
zx_scan_param_t zx_param =
{
    .prio_normal = ZX_SCAN_THREAD_NORMAL_PRIO,
    .width = 810,
    .height = 720,
};
zx_scan_t zx_scan;

lv_ui guider_ui;

lv_img_dsc_t img_preview_desc = {
    .header = {
        .cf = LV_IMG_CF_TRUE_COLOR_ALPHA,
        .always_zero = 0,
        .reserved = 0,
        .w = 810,
        .h = 720,
    },
  .data_size = 810 * 720 * 4,
  .data = nullptr,
};

#define TEXTAREA_BUF_SIZE (16384)
char textarea_strbuf[TEXTAREA_BUF_SIZE];

#ifdef CONFIG_BARCODE_TIME_MEASUREMENT
/** used by time measurement */
uint64_t stime, etime;
#endif // CONFIG_BARCODE_TIME_MEASUREMENT

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

int disp_dev_config(struct device const * const dev, struct display_buffer_descriptor *desc)
{
    struct display_capabilities capabilities;

    if (!device_is_ready(dev)) {
        LOG_ERR("Device %s not found", dev->name);
        return 0;
    }

    display_get_capabilities(dev, &capabilities);
    if (capabilities.x_resolution != 1280
        || capabilities.y_resolution != 800)
    {
        LOG_ERR("Device %s resolution mismatch", dev->name);
    }

    display_set_pixel_format(dev, PIXEL_FORMAT_ARGB_8888);

    size_t rect_w = /*capabilities.x_resolution*/1280;
    size_t rect_h = /*capabilities.y_resolution*/800;

    size_t buf_size = rect_w * rect_h * 4;
    size_t pitch = rect_w * 4;

    LOG_INF("rect_w=%ld, rect_h=%ld, buf_size=%ld", rect_w, rect_h, buf_size);

    desc->buf_size = buf_size;
    desc->pitch = pitch;
    desc->width = rect_w;
    desc->height = rect_h;

    return 0;
}

int video_dev_config(struct device const * const dev, struct video_format * const fmt)
{
    struct video_caps caps;

    if (!device_is_ready(video_dev)) {
        LOG_ERR("Device %s not ready",
            video_dev->name);
        return 0;
    }

    /* Get capabilities */
    if (video_get_caps(video_dev, VIDEO_EP_OUT, &caps)) {
        LOG_ERR("Unable to get video capabilities\n");
        return 0;
    }
    /* Set format */
    memset(fmt, 0, sizeof(struct video_format));
    //fmt.pixelformat = VIDEO_PIX_FMT_RGB565;
    //fmt.pixelformat = VIDEO_PIX_FMT_YUYV;
    fmt->pixelformat = VIDEO_PIX_FMT_BGRA;
    fmt->width = 810;
    fmt->height = 720;
    fmt->pitch = fmt->width * 4;
    //LOG_DBG("****** set pixelformat: %c%c%c%c, wxh: %dx%d, pitch: %d ******\n",
    //            (char)fmt.pixelformat, (char)(fmt.pixelformat >> 8),
    //            (char)(fmt.pixelformat >> 16), (char)(fmt.pixelformat >> 24),
    //            fmt.width, fmt.height, fmt.pitch);
    if (video_set_format(video_dev, VIDEO_EP_OUT, fmt)) {
        LOG_ERR("Unable to set video format\n");
        return 0;
    }
    memset(fmt, 0, sizeof(struct video_format));
    if (video_get_format(video_dev, VIDEO_EP_OUT, fmt)) {
        LOG_ERR("Unable to get video format\n");
        return 0;
    }
    //LOG_INF("****** get pixelformat: %c%c%c%c, wxh: %dx%d, pitch: %d ******\n",
    //            (char)fmt.pixelformat, (char)(fmt.pixelformat >> 8),
    //            (char)(fmt.pixelformat >> 16), (char)(fmt.pixelformat >> 24),
    //            fmt.width, fmt.height, fmt.pitch);
}


int main(void)
{
    struct display_buffer_descriptor buf_desc;
    struct video_format fmt_desc;

    k_thread_priority_set(k_current_get(), 5);

    video_dev_config(video_dev, &fmt_desc);
    /* Size to allocate for each buffer */
    int bsize = fmt_desc.pitch * fmt_desc.height;
    LOG_INF("bsize=%d, buffer_num=%d\n", bsize, ARRAY_SIZE(my_video_buffer_pool));
    /* Alloc video buffers and enqueue for capture */
    for (int i = 0; i < ARRAY_SIZE(my_video_buffer_pool); i++) {
        my_video_buffer_pool[i] = video_buffer_alloc(bsize) ;
        if (my_video_buffer_pool[i] == NULL) {
            LOG_ERR("Unable to alloc video buffer %d", i);
            return 0;
        }
#ifdef CONFIG_BARCODE_VIDEO_FRAME_CHECK
        ((uint32_t*)my_video_buffer_pool[i]->buffer)[0] = 0x00f0f0f0;
#endif // CONFIG_BARCODE_VIDEO_FRAME_CHECK
        video_enqueue(video_dev, VIDEO_EP_OUT, my_video_buffer_pool[i]);
        LOG_INF("frame buffers[%d]/%d, 0x%8.8p", i, ARRAY_SIZE(my_video_buffer_pool),
            (void *)my_video_buffer_pool[i]->buffer);
    }

    disp_dev_config(lv_disp_dev, &buf_desc);

    /** we use board-level backlight control */
    board_enable_backlight();
    //display_blanking_off(lv_disp_dev);

    k_thread_create(&zx_scan_thread, zx_scan_thread_stack,
        ZX_SCAN_THREAD_STACK_SIZE,
        (k_thread_entry_t)ZX_ScanTask, (void*)&zx_scan, (void*)&zx_param, NULL,
        ZX_SCAN_THREAD_START_PRIO, 0, K_NO_WAIT
    );
    k_yield();

    if (video_stream_start(video_dev)) {
        LOG_ERR("Unable to start capture\n");
        return 0;
    }
    LOG_INF("****** video_stream_start succeeded ******");

    setup_ui(&guider_ui);
   	events_init(&guider_ui);
    lv_img_set_src(guider_ui.scanner_image_camera_preview, &img_preview_desc);

	lv_task_handler();
	//display_blanking_off(lv_disp_dev);

    int frame = 0;
    struct video_buffer *vbuf = nullptr, *vbuf_in_use = nullptr;
    size_t result_str_len = 0;
    bool preserve_old_result = true;
    while (true) {

        if (video_dequeue(video_dev, VIDEO_EP_OUT, &vbuf, K_FOREVER)) {
            LOG_ERR("Unable to dequeue video buf");
            break;
        }

#ifdef CONFIG_BARCODE_VIDEO_FRAME_CHECK
        if(((uint32_t*)vbuf->buffer)[0] == 0x00f0f0f0)
        {
            LOG_ERR("frame invalid, skipped");
            video_enqueue(video_dev, VIDEO_EP_OUT, vbuf);
            continue;
        }
#endif // CONFIG_BARCODE_VIDEO_FRAME_CHECK

        LOG_DBG("display frame %d", frame);\

        /** send frame to zxing */
        if ((frame % 6) == 0 && k_mutex_lock(&zx_scan.lock, K_NO_WAIT) == 0)
        {
            for (auto it = zx_scan.results.begin(); it < zx_scan.results.end(); ++it)
            {
                if (static_cast<int>(it->error().type()) != 0)
                {
                    it = zx_scan.results.erase(it);
                }
            }
            if (zx_scan.results.empty())
            {
                if(preserve_old_result == false)
                {
                    preserve_old_result = true;
                    result_str_len += snprintf((textarea_strbuf + result_str_len),
                        (TEXTAREA_BUF_SIZE - result_str_len),"no qr-code found. preserve old result.\r\n");
                }
            }
            else
            {
                uint32_t result_no = 0;
                result_str_len = 0;
                preserve_old_result = false;
                result_str_len += snprintf((textarea_strbuf + result_str_len),
                        (TEXTAREA_BUF_SIZE - result_str_len),"scan result from frame %8.8d :\r\n", zx_scan.frame_no);
                for (auto&& result : zx_scan.results)
                {
                    result_str_len += snprintf((textarea_strbuf + result_str_len),
                        (TEXTAREA_BUF_SIZE - result_str_len),"qr-code no.%2.2d :\r\n", result_no);
                    result_str_len += ZX_ResultFormatString(
                        textarea_strbuf + result_str_len, TEXTAREA_BUF_SIZE - result_str_len, result);
                    ++result_no;
                }
            }

            textarea_strbuf[result_str_len] = '\0';
            lv_textarea_set_text(guider_ui.scanner_text_results, textarea_strbuf);
            lv_textarea_set_cursor_pos(guider_ui.scanner_text_results, 0);

            LOG_DBG("zx_scan send frame %d", frame);
            memcpy(zx_scan.frame, vbuf->buffer, vbuf->bytesused);
            zx_scan.frame_no = frame;
            k_condvar_signal(&zx_scan.cond);
            k_mutex_unlock(&zx_scan.lock);
        }
        else
        {
            LOG_DBG("zx_scan skip frame %d", frame);
        }

        /** send frame to lvgl */
        img_preview_desc.data = vbuf->buffer;
        lv_img_set_src(guider_ui.scanner_image_camera_preview, &img_preview_desc);
        //lv_obj_invalidate(guider_ui.scanner_image_camera_preview);
        update_scan_anim(&guider_ui);

        if (vbuf_in_use != nullptr)
        {
#ifdef CONFIG_BARCODE_VIDEO_FRAME_CHECK
            ((uint32_t*)vbuf_in_use->buffer)[0] = 0x00f0f0f0;
#endif // CONFIG_BARCODE_VIDEO_FRAME_CHECK
            if (video_enqueue(video_dev, VIDEO_EP_OUT, vbuf_in_use)) {
                LOG_ERR("Unable to enqueue video buf\n");
                break;
            }
        }

        vbuf_in_use = vbuf;
        frame++;
        LOG_INF("** exit frame %d process **", frame);

        //lv_task_handler();
        lv_refr_now(NULL);

#ifdef CONFIG_BARCODE_TIME_MEASUREMENT
        etime = k_cyc_to_us_near64(k_cycle_get_64());
        LOG_WRN("ts: %lld us, mainloop: %lld us", etime, etime - stime);
        stime = etime;
#endif // CONFIG_BARCODE_TIME_MEASUREMENT
    }

    if (video_stream_stop(video_dev)) {
        LOG_ERR("Unable to stop capture\n");
        return 0;
    }
    LOG_INF("****** video_stream_stop succeeded ******");

    return 0;
}
