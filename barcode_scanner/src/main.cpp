/*
 * Copyright 2024,2025 NXP
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

const struct device *video_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_camera));
struct video_buffer *my_video_buffer_pool[CONFIG_VIDEO_BUFFER_POOL_NUM_MAX];

const struct device *lv_disp_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

K_THREAD_STACK_DEFINE(zx_scan_thread_stack, ZX_SCAN_THREAD_STACK_SIZE);
struct k_thread zx_scan_thread;
zx_scan_param_t zx_param =
{
    .prio_normal = ZX_SCAN_THREAD_NORMAL_PRIO,
    .width = 640,
    .height = 600,
};
zx_scan_t zx_scan;

lv_ui guider_ui;

lv_image_dsc_t img_preview_desc = {
    .header = {
        .magic = LV_IMAGE_HEADER_MAGIC,
        .cf = LV_COLOR_FORMAT_ARGB8888,
        .flags = 0,
        .w = 640,
        .h = 600,
        .stride = 640 * 4,
        .reserved_2 = 0,
    },
  .data_size = 640 * 600 * 4,
  .data = nullptr,
};

#ifdef CONFIG_BARCODE_TIME_MEASUREMENT
/** used by time measurement */
uint64_t stime, etime;
#endif // CONFIG_BARCODE_TIME_MEASUREMENT

int disp_dev_config(struct device const * const dev, struct display_buffer_descriptor *desc)
{
    struct display_capabilities capabilities;
    int ret = 0;

    if (!device_is_ready(dev)) {
        LOG_ERR("Device %s not found", dev->name);
        return -ENODEV;
    }

    LOG_INF("Display device: %s", dev->name);

    display_get_capabilities(dev, &capabilities);

    LOG_INF("- Capabilities:");
	LOG_INF("  x_resolution = %u, y_resolution = %u, supported_pixel_formats = %u"
	       "  current_pixel_format = %u, current_orientation = %u",
	       capabilities.x_resolution, capabilities.y_resolution,
	       capabilities.supported_pixel_formats, capabilities.current_pixel_format,
	       capabilities.current_orientation);

    if (capabilities.x_resolution != 1024
        || capabilities.y_resolution != 600)
    {
        LOG_ERR("Device %s resolution is not supported", dev->name);
    }

    ret = display_set_pixel_format(dev, PIXEL_FORMAT_ARGB_8888);

    if (ret) {
		LOG_ERR("Unable to set display format");
		return ret;
	}

    size_t rect_w = /*capabilities.x_resolution*/1024;
    size_t rect_h = /*capabilities.y_resolution*/600;

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
    struct video_format output_fmt;
#if CONFIG_DT_HAS_NXP_IMX_ISI_ENABLED
	struct video_format camera_fmt;
#endif
    struct video_frmival frmival;
	struct video_frmival_enum fie;
    int i = 0;

    if (!device_is_ready(dev)) {
        LOG_ERR("Device %s not ready",
            dev->name);
        return 0;
    }

#if CONFIG_DT_HAS_NXP_IMX_ISI_ENABLED
	/* Get capabilities for input endpoint */
	if (video_get_caps(dev, VIDEO_EP_IN, &caps)) {
		LOG_ERR("Unable to retrieve video capabilities");
		return 0;
	}

	LOG_INF("- Input Capabilities:");
	while (caps.format_caps[i].pixelformat) {
		const struct video_format_cap *fcap = &caps.format_caps[i];
		/* fourcc to string */
		LOG_INF("  %c%c%c%c width [%u; %u; %u] height [%u; %u; %u]",
		       (char)fcap->pixelformat, (char)(fcap->pixelformat >> 8),
		       (char)(fcap->pixelformat >> 16), (char)(fcap->pixelformat >> 24),
		       fcap->width_min, fcap->width_max, fcap->width_step, fcap->height_min,
		       fcap->height_max, fcap->height_step);
		i++;
	}
	i = 0;
#endif

    /* Get capabilities for output endpoint */
	if (video_get_caps(dev, VIDEO_EP_OUT, &caps)) {
		LOG_ERR("Unable to retrieve video capabilities");
		return 0;
	}

	LOG_INF("- Output Capabilities:");
	while (caps.format_caps[i].pixelformat) {
		const struct video_format_cap *fcap = &caps.format_caps[i];
		/* fourcc to string */
		LOG_INF("  %c%c%c%c width [%u; %u; %u] height [%u; %u; %u]",
		       (char)fcap->pixelformat, (char)(fcap->pixelformat >> 8),
		       (char)(fcap->pixelformat >> 16), (char)(fcap->pixelformat >> 24),
		       fcap->width_min, fcap->width_max, fcap->width_step, fcap->height_min,
		       fcap->height_max, fcap->height_step);
		i++;
	}

#if CONFIG_DT_HAS_NXP_IMX_ISI_ENABLED
	/* Get default/native format for input endpoint */
	if (video_get_format(dev, VIDEO_EP_IN, &camera_fmt)) {
		LOG_ERR("Unable to retrieve video format");
		return 0;
	}

	camera_fmt.pixelformat = VIDEO_FOURCC_FROM_STR("UYVY");
	camera_fmt.width = 640;
	camera_fmt.height = 600;
	camera_fmt.pitch = fmt->width * video_bits_per_pixel(fmt->pixelformat) / BITS_PER_BYTE;


	LOG_INF("- Input Video format: %c%c%c%c %ux%u", (char)camera_fmt.pixelformat,
	       (char)(camera_fmt.pixelformat >> 8), (char)(camera_fmt.pixelformat >> 16),
	       (char)(camera_fmt.pixelformat >> 24), camera_fmt.width, camera_fmt.height);

	/* Set format for input endpoint */
	if (video_set_format(dev, VIDEO_EP_IN, &camera_fmt)) {
		LOG_ERR("Unable to set format for the camera");
		return 0;
	}
#endif

	/* Get default/native format for output endpoint */
	if (video_get_format(dev, VIDEO_EP_OUT, &output_fmt)) {
		LOG_ERR("Unable to retrieve video format");
		return 0;
	}

	output_fmt.pixelformat = VIDEO_FOURCC_FROM_STR("AR24");
	output_fmt.width = 640;
	output_fmt.height = 600;
	output_fmt.pitch = output_fmt.width * video_bits_per_pixel(output_fmt.pixelformat) / BITS_PER_BYTE;

	LOG_INF("- Output Video format: %c%c%c%c %ux%u", (char)output_fmt.pixelformat,
	       (char)(output_fmt.pixelformat >> 8), (char)(output_fmt.pixelformat >> 16),
	       (char)(output_fmt.pixelformat >> 24), output_fmt.width, output_fmt.height);

	if (video_set_format(dev, VIDEO_EP_OUT, &output_fmt)) {
		LOG_ERR("Unable to set format");
		return 0;//FIXME
	}

    memcpy(fmt, &output_fmt, sizeof(struct video_format));

	if (!video_get_frmival(dev, VIDEO_EP_OUT, &frmival)) {
		LOG_INF("- Default frame rate : %f fps",
		       1.0 * frmival.denominator / frmival.numerator);
	}

	LOG_INF("- Supported frame intervals for the default format:");
	memset(&fie, 0, sizeof(fie));
#if CONFIG_DT_HAS_NXP_IMX_ISI_ENABLED
	fie.format = &camera_fmt;
#else
	fie.format = &outout_fmt;
#endif

	while (1) {
        int ret = video_enum_frmival(dev, VIDEO_EP_OUT, &fie);
        if (ret) {
            break;
        }
        LOG_INF("fie.index = %u, ret = %d", fie.index, ret);
		if (fie.type == VIDEO_FRMIVAL_TYPE_DISCRETE) {
			LOG_INF("   %u/%u ", fie.discrete.numerator, fie.discrete.denominator);
		} else {
			LOG_INF("   [min = %u/%u; max = %u/%u; step = %u/%u]",
			       fie.stepwise.min.numerator, fie.stepwise.min.denominator,
			       fie.stepwise.max.numerator, fie.stepwise.max.denominator,
			       fie.stepwise.step.numerator, fie.stepwise.step.denominator);
		}
		fie.index++;
	}

    frmival.numerator = 30;
    frmival.denominator = 1;

    if (!video_set_frmival(dev, VIDEO_EP_OUT, &frmival)) {
		LOG_INF("- Set frame rate : %f fps",
		       1.0 * frmival.denominator / frmival.numerator);
	} else {
        LOG_ERR("- Set frame rate : %f fps failed",
		       1.0 * frmival.denominator / frmival.numerator);
        return 0;//FIXME
    }

    LOG_INF("video device config done");
    return 0;
}


int main(void)
{
    struct display_buffer_descriptor buf_desc;
    struct video_format fmt_desc;
    volatile bool zx_frame_copied = false;
    int frame = 0;
    struct video_buffer *vbuf = nullptr;

    k_thread_priority_set(k_current_get(), 5);

    video_dev_config(video_dev, &fmt_desc);
    /* Size to allocate for each buffer */
    int bsize = fmt_desc.pitch * fmt_desc.height;
    LOG_INF("bsize=%ld, buffer_num=%ld\n", bsize, ARRAY_SIZE(my_video_buffer_pool));
    /* Alloc video buffers and enqueue for capture */
    for (int i = 0; i < ARRAY_SIZE(my_video_buffer_pool); i++) {
        my_video_buffer_pool[i] = video_buffer_aligned_alloc(bsize, CONFIG_VIDEO_BUFFER_POOL_ALIGN,
            K_FOREVER);
        if (my_video_buffer_pool[i] == NULL) {
            LOG_ERR("Unable to alloc video buffer %d", i);
            return 0;
        }
#ifdef CONFIG_BARCODE_VIDEO_FRAME_CHECK
        ((uint32_t*)my_video_buffer_pool[i]->buffer)[0] = 0x00f0f0f0;
#endif // CONFIG_BARCODE_VIDEO_FRAME_CHECK
        video_enqueue(video_dev, VIDEO_EP_OUT, my_video_buffer_pool[i]);
        LOG_INF("frame buffers[%ld]/%ld, 0x%8.8p", i, ARRAY_SIZE(my_video_buffer_pool),
            (void *)my_video_buffer_pool[i]->buffer);
    }

    disp_dev_config(lv_disp_dev, &buf_desc);

    /** we use board-level backlight control */
    //display_blanking_off(lv_disp_dev);

    k_thread_create(&zx_scan_thread, zx_scan_thread_stack,
        ZX_SCAN_THREAD_STACK_SIZE,
        (k_thread_entry_t)ZX_ScanTask, (void*)&zx_scan, (void*)&zx_param, NULL,
        ZX_SCAN_THREAD_START_PRIO, 0, K_NO_WAIT
    );
    k_yield();

    setup_ui(&guider_ui);
   	events_init(&guider_ui);
    lv_img_set_src(guider_ui.scanner_image_camera_preview, &img_preview_desc);
    //lv_label_set_text(guider_ui.scanner_text_results, "No QR-Code found yet.");

    if (video_stream_start(video_dev)) {
        LOG_ERR("Unable to start capture\n");
        return 0;
    }
    LOG_INF("****** video_stream_start succeeded ******");

	//lv_task_handler();
	//display_blanking_off(lv_disp_dev);

#ifdef CONFIG_BARCODE_TIME_MEASUREMENT
        etime = k_cyc_to_us_near64(k_cycle_get_64());
        LOG_WRN("boot time: %lld us", k_cyc_to_us_near64(k_cycle_get_64()));
#endif // CONFIG_BARCODE_TIME_MEASUREMENT

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

#ifdef CONFIG_BARCODE_TIME_MEASUREMENT
        stime = k_cyc_to_us_near64(k_cycle_get_64());
#endif // CONFIG_BARCODE_TIME_MEASUREMENT

        LOG_DBG("display frame %d", frame);

        /** Process Zxing results */
        if ((zx_frame_copied == true) && (k_mutex_lock(&zx_scan.lock, K_NO_WAIT) == 0)) {
            
            /** Zxing is ready for next frame */
            zx_frame_copied = false;

            /** Update LVGL textarea */
            lv_label_set_text(guider_ui.scanner_text_results, zx_scan.results_str);
            LOG_DBG("scan results processed: %s", zx_scan.results_str);

            k_mutex_unlock(&zx_scan.lock);
        }

        /** Send frame to ZXing */
        if (((frame % 6) == 0) &&
            (ZX_SendFrame(&zx_scan, frame, vbuf->buffer, vbuf->bytesused) == 0)) {
                zx_frame_copied = true;
        }

        /** Send frame to LVGL */
        img_preview_desc.data = vbuf->buffer;
        lv_img_set_src(guider_ui.scanner_image_camera_preview, &img_preview_desc);
        //lv_obj_invalidate(guider_ui.scanner_image_camera_preview);
        update_scan_anim(&guider_ui);

        //lv_task_handler();
        lv_refr_now(NULL);

        if (vbuf != nullptr)
        {
#ifdef CONFIG_BARCODE_VIDEO_FRAME_CHECK
            ((uint32_t*)vbuf->buffer)[0] = 0x00f0f0f0;
#endif // CONFIG_BARCODE_VIDEO_FRAME_CHECK
            if (video_enqueue(video_dev, VIDEO_EP_OUT, vbuf)) {
                LOG_ERR("Unable to enqueue video buf\n");
                break;
            }
        }

        //lv_task_handler();
        lv_refr_now(NULL);

#ifdef CONFIG_BARCODE_TIME_MEASUREMENT
        etime = k_cyc_to_us_near64(k_cycle_get_64());
        LOG_WRN("ts: %lld us, mainloop: %lld us", etime, etime - stime);
#endif // CONFIG_BARCODE_TIME_MEASUREMENT
        
        LOG_INF("** exit frame %d process **", frame);
        frame++;
    }

    if (video_stream_stop(video_dev)) {
        LOG_ERR("Unable to stop capture\n");
        return 0;
    }
    LOG_INF("****** video_stream_stop succeeded ******");

    return 0;
}
