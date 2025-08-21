/*
 * Copyright 2024,2025 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */
/** ZephyrOS include group */
#include <zephyr/cache.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/video.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>

/** C STD include group */
#include <cinttypes>
#include <cstdio>
#include <cstring>

/** CPP STD/STL include group */
#include <vector>

/** GUI-Guider include group */
#include "../custom/custom.h"
#include "../generated/events_init.h"
#include "../generated/gui_guider.h"

/** Project include group */
#include "zx_scan.h"

/** Zephyr logging include group */
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main_tr, CONFIG_BARCODE_MAIN_LOG_LEVEL);

const struct device *video_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_camera));
struct video_buffer *my_video_buffer_pool[CONFIG_VIDEO_BUFFER_POOL_NUM_MAX];

const struct device *display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

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
        LOG_ERR("Display device %s not ready", dev->name);
        return -ENODEV;
    }

    LOG_INF("Configuring display: %s", dev->name);

    // Retrieve display capabilities
    display_get_capabilities(dev, &capabilities);

    LOG_INF("- Capabilities:");
    LOG_INF("  Resolution: %u x %u", capabilities.x_resolution, capabilities.y_resolution);
    LOG_INF("  Supported formats: 0x%08x, Current format: %u", 
            capabilities.supported_pixel_formats, capabilities.current_pixel_format);
    LOG_INF("  Current orientation: %u", capabilities.current_orientation);

    // Verify resolution support
    if (capabilities.x_resolution != 1024 || capabilities.y_resolution != 600) {
        LOG_ERR("Unsupported resolution: %u x %u (expected: 1024 x 600)", 
                capabilities.x_resolution, capabilities.y_resolution);
        return -EINVAL;
    }

    // Set pixel format to ARGB8888
    ret = display_set_pixel_format(dev, PIXEL_FORMAT_ARGB_8888);
    if (ret) {
        LOG_ERR("Failed to set pixel format");
        return ret;
    }

    // Define buffer dimensions (fixed for this display)
    const size_t rect_w = 1024;
    const size_t rect_h = 600;

    // Calculate buffer size and pitch
    const size_t buf_size = rect_w * rect_h * 4;  // 4 bytes per pixel (ARGB8888)
    const size_t pitch = rect_w * 4;

    LOG_INF("Buffer: %zu x %zu, size=%zu bytes, pitch=%zu bytes", 
            rect_w, rect_h, buf_size, pitch);

    // Populate descriptor
    desc->buf_size = buf_size;
    desc->pitch = pitch;
    desc->width = rect_w;
    desc->height = rect_h;

    return 0;
}

/**
 * @brief Configures the video device (camera) with desired format, frame rate, and capabilities.
 *
 * This function:
 * - Queries and logs input/output capabilities.
 * - Sets input (camera) format to UYVY (640x600).
 * - Sets output format to AR24 (640x600).
 * - Sets frame rate to 30 fps.
 * - Enumerates supported frame intervals.
 *
 * @param dev Pointer to the video device.
 * @param fmt Pointer to output format structure to be filled.
 * @return 0 on success, non-zero on failure.
 */
int video_dev_config(const struct device *const dev, struct video_format *const fmt)
{
    struct video_caps caps;
    struct video_format output_fmt;
#if CONFIG_DT_HAS_NXP_IMX_ISI_ENABLED
    struct video_format camera_fmt;
#endif // CONFIG_DT_HAS_NXP_IMX_ISI_ENABLED
    struct video_frmival frmival;
    struct video_frmival_enum fie;
    int i = 0;

    if (!device_is_ready(dev)) {
        LOG_ERR("Video device %s not ready", dev->name);
        return -ENODEV;
    }

#if CONFIG_DT_HAS_NXP_IMX_ISI_ENABLED
    /* Query input endpoint (camera) capabilities */
    if (video_get_caps(dev, VIDEO_EP_IN, &caps)) {
        LOG_ERR("Failed to retrieve input capabilities");
        return -EIO;
    }

    LOG_INF("- Input Capabilities:");
    while (caps.format_caps[i].pixelformat) {
        const struct video_format_cap *fcap = &caps.format_caps[i];
        LOG_INF("  %c%c%c%c | W[%u;%u;%u] | H[%u;%u;%u]",
               (char)fcap->pixelformat,
               (char)(fcap->pixelformat >> 8),
               (char)(fcap->pixelformat >> 16),
               (char)(fcap->pixelformat >> 24),
               fcap->width_min, fcap->width_max, fcap->width_step,
               fcap->height_min, fcap->height_max, fcap->height_step);
        i++;
    }
    i = 0;
#endif // CONFIG_DT_HAS_NXP_IMX_ISI_ENABLED

    /* Query output endpoint (processing) capabilities */
    if (video_get_caps(dev, VIDEO_EP_OUT, &caps)) {
        LOG_ERR("Failed to retrieve output capabilities");
        return -EIO;
    }

    LOG_INF("- Output Capabilities:");
    while (caps.format_caps[i].pixelformat) {
        const struct video_format_cap *fcap = &caps.format_caps[i];
        LOG_INF("  %c%c%c%c | W[%u;%u;%u] | H[%u;%u;%u]",
               (char)fcap->pixelformat,
               (char)(fcap->pixelformat >> 8),
               (char)(fcap->pixelformat >> 16),
               (char)(fcap->pixelformat >> 24),
               fcap->width_min, fcap->width_max, fcap->width_step,
               fcap->height_min, fcap->height_max, fcap->height_step);
        i++;
    }

#if CONFIG_DT_HAS_NXP_IMX_ISI_ENABLED
    /* Configure input (camera) format: UYVY, 640x600 */
    if (video_get_format(dev, VIDEO_EP_IN, &camera_fmt)) {
        LOG_ERR("Failed to retrieve default input format");
        return -EIO;
    }

    camera_fmt.pixelformat = VIDEO_FOURCC_FROM_STR("UYVY");
    camera_fmt.width = 640;
    camera_fmt.height = 600;
    camera_fmt.pitch = fmt->width * video_bits_per_pixel(fmt->pixelformat) / BITS_PER_BYTE;

    LOG_INF("- Input Format: %c%c%c%c @ %ux%u",
           (char)camera_fmt.pixelformat,
           (char)(camera_fmt.pixelformat >> 8),
           (char)(camera_fmt.pixelformat >> 16),
           (char)(camera_fmt.pixelformat >> 24),
           camera_fmt.width, camera_fmt.height);

    if (video_set_format(dev, VIDEO_EP_IN, &camera_fmt)) {
        LOG_ERR("Failed to set input format");
        return -EIO;
    }
#endif // CONFIG_DT_HAS_NXP_IMX_ISI_ENABLED

    /* Configure output format: AR24, 640x600 */
    if (video_get_format(dev, VIDEO_EP_OUT, &output_fmt)) {
        LOG_ERR("Failed to retrieve default output format");
        return -EIO;
    }

    output_fmt.pixelformat = VIDEO_FOURCC_FROM_STR("AR24");
    output_fmt.width = 640;
    output_fmt.height = 600;
    output_fmt.pitch = output_fmt.width * video_bits_per_pixel(output_fmt.pixelformat) / BITS_PER_BYTE;

    LOG_INF("- Output Format: %c%c%c%c @ %ux%u",
           (char)output_fmt.pixelformat,
           (char)(output_fmt.pixelformat >> 8),
           (char)(output_fmt.pixelformat >> 16),
           (char)(output_fmt.pixelformat >> 24),
           output_fmt.width, output_fmt.height);

    if (video_set_format(dev, VIDEO_EP_OUT, &output_fmt)) {
        LOG_ERR("Failed to set output format");
        return -EIO;
    }

    /* Copy final format to output */
    memcpy(fmt, &output_fmt, sizeof(struct video_format));

    /* Query and log default frame rate */
    if (!video_get_frmival(dev, VIDEO_EP_OUT, &frmival)) {
        LOG_INF("- Default frame rate: %.2f fps",
               1.0 * frmival.denominator / frmival.numerator);
    }

    /* List supported frame intervals for the output format */
    LOG_INF("- Supported frame intervals:");
    memset(&fie, 0, sizeof(fie));
#if CONFIG_DT_HAS_NXP_IMX_ISI_ENABLED
    fie.format = &camera_fmt;
#else // CONFIG_DT_HAS_NXP_IMX_ISI_ENABLED
    fie.format = &output_fmt;  // Fixed typo: 'outout_fmt' → 'output_fmt'
#endif // CONFIG_DT_HAS_NXP_IMX_ISI_ENABLED

    while (1) {
        int ret = video_enum_frmival(dev, VIDEO_EP_OUT, &fie);
        if (ret) {
            break;
        }

        LOG_INF("  Index %u: ret=%d", fie.index, ret);
        if (fie.type == VIDEO_FRMIVAL_TYPE_DISCRETE) {
            LOG_INF("    %u/%u", fie.discrete.numerator, fie.discrete.denominator);
        } else {
            LOG_INF("    [min=%u/%u; max=%u/%u; step=%u/%u]",
                   fie.stepwise.min.numerator, fie.stepwise.min.denominator,
                   fie.stepwise.max.numerator, fie.stepwise.max.denominator,
                   fie.stepwise.step.numerator, fie.stepwise.step.denominator);
        }
        fie.index++;
    }

    /* Set frame rate to 30 fps */
    frmival.numerator = 30;
    frmival.denominator = 1;

    if (!video_set_frmival(dev, VIDEO_EP_OUT, &frmival)) {
        LOG_INF("- Frame rate set to %.2f fps", 1.0 * frmival.denominator / frmival.numerator);
    } else {
        LOG_ERR("- Failed to set frame rate to %.2f fps", 1.0 * frmival.denominator / frmival.numerator);
        return -EIO;
    }

    LOG_INF("Video device configuration completed successfully");
    return 0;
}

/**
 * @brief Main application entry point.
 *
 * Initializes:
 * - Video and display devices
 * - Video buffer pool
 * - ZXing scan thread
 * - GUI
 * - Starts video stream
 * - Main loop: dequeue, process, display, enqueue
 */
int main(void)
{
    struct display_buffer_descriptor buf_desc;
    struct video_format fmt_desc;
    volatile bool zx_frame_copied = false;
    int frame_cnt = 0;
    struct video_buffer *vbuf = nullptr;

    // Set thread priority
    k_thread_priority_set(k_current_get(), 5);

    // Configure video device
    if (video_dev_config(video_dev, &fmt_desc) != 0) {
        LOG_ERR("Failed to configure video device");
        return -1;
    }

    int bsize = fmt_desc.pitch * fmt_desc.height;
    LOG_INF("Buffer size: %ld, Pool size: %ld", bsize, ARRAY_SIZE(my_video_buffer_pool));

    // Allocate and enqueue video buffers
    for (int i = 0; i < ARRAY_SIZE(my_video_buffer_pool); i++) {
        my_video_buffer_pool[i] = video_buffer_aligned_alloc(bsize, CONFIG_VIDEO_BUFFER_POOL_ALIGN, K_FOREVER);
        if (!my_video_buffer_pool[i]) {
            LOG_ERR("Failed to allocate video buffer %d", i);
            return -1;
        }

#ifdef CONFIG_BARCODE_VIDEO_FRAME_CHECK
        ((uint32_t*)my_video_buffer_pool[i]->buffer)[0] = 0x00f0f0f0;
#endif // CONFIG_BARCODE_VIDEO_FRAME_CHECK

        if (video_enqueue(video_dev, VIDEO_EP_OUT, my_video_buffer_pool[i])) {
            LOG_ERR("Failed to enqueue buffer %d", i);
            return -1;
        }

        LOG_INF("Buffer %d/%d: 0x%8.8p", i, ARRAY_SIZE(my_video_buffer_pool), (void*)my_video_buffer_pool[i]->buffer);
    }

    // Configure display
    if (disp_dev_config(display_dev, &buf_desc) != 0) {
        LOG_ERR("Failed to configure display");
        return -1;
    }

    // Start ZXing scan thread
    k_thread_create(&zx_scan_thread, zx_scan_thread_stack,
                    ZX_SCAN_THREAD_STACK_SIZE,
                    (k_thread_entry_t)ZX_ScanTask, (void*)&zx_scan, (void*)&zx_param, NULL,
                    ZX_SCAN_THREAD_START_PRIO, 0, K_NO_WAIT);
    k_yield();

    // Initialize GUI
    setup_ui(&guider_ui);
    events_init(&guider_ui);
    lv_img_set_src(guider_ui.scanner_image_camera_preview, &img_preview_desc);

#ifdef CONFIG_BARCODE_TIME_MEASUREMENT
    etime = k_cyc_to_us_near64(k_cycle_get_64());
    LOG_WRN("Boot time: %lld us", etime);
#endif // CONFIG_BARCODE_TIME_MEASUREMENT

    // Start video capture
    if (video_stream_start(video_dev)) {
        LOG_ERR("Failed to start video stream");
        return -1;
    }
    LOG_INF("Video stream started");

    // Main processing loop
    while (true) {
        if (video_dequeue(video_dev, VIDEO_EP_OUT, &vbuf, K_FOREVER)) {
            LOG_ERR("Failed to dequeue video buffer");
            break;
        }

#ifdef CONFIG_BARCODE_VIDEO_FRAME_CHECK
        if (((uint32_t*)vbuf->buffer)[0] == 0x00f0f0f0) {
            LOG_WRN("Invalid frame detected, skipping");
            video_enqueue(video_dev, VIDEO_EP_OUT, vbuf);
            continue;
        }
#endif // CONFIG_BARCODE_VIDEO_FRAME_CHECK

#ifdef CONFIG_BARCODE_TIME_MEASUREMENT
        stime = k_cyc_to_us_near64(k_cycle_get_64());
#endif // CONFIG_BARCODE_TIME_MEASUREMENT

        LOG_DBG("Processing frame %d", frame_cnt);

        // Process ZXing results if available
        if (zx_frame_copied && k_mutex_lock(&zx_scan.lock, K_NO_WAIT) == 0) {
            zx_frame_copied = false;
            lv_label_set_text(guider_ui.scanner_text_results, zx_scan.results_str);
            LOG_DBG("ZXing results: %s", zx_scan.results_str);
            k_mutex_unlock(&zx_scan.lock);
        }

        // Send frame to ZXing every 6th frame
        if ((frame_cnt % 6 == 0) && ZX_SendFrame(&zx_scan,
                frame_cnt, vbuf->buffer, vbuf->bytesused) == 0) {
            zx_frame_copied = true;
        }

        // Update preview image
        img_preview_desc.data = vbuf->buffer;
        lv_img_set_src(guider_ui.scanner_image_camera_preview, &img_preview_desc);
        update_scan_anim(&guider_ui);
        lv_refr_now(NULL);

        // Re-enqueue buffer after processing
        if (vbuf) {
#ifdef CONFIG_BARCODE_VIDEO_FRAME_CHECK
            ((uint32_t*)vbuf->buffer)[0] = 0x00f0f0f0;
#endif // CONFIG_BARCODE_VIDEO_FRAME_CHECK
            if (video_enqueue(video_dev, VIDEO_EP_OUT, vbuf)) {
                LOG_ERR("Failed to enqueue video buffer");
                break;
            }
        }

        // Final refresh
        lv_refr_now(NULL);

#ifdef CONFIG_BARCODE_TIME_MEASUREMENT
        etime = k_cyc_to_us_near64(k_cycle_get_64());
        LOG_WRN("ts: %lld us, loop time: %lld us", etime, etime - stime);
#endif // CONFIG_BARCODE_TIME_MEASUREMENT

        LOG_INF("Frame %d processed", frame_cnt);
        ++frame_cnt;
    }

    // Stop video stream
    if (video_stream_stop(video_dev)) {
        LOG_ERR("Failed to stop video stream");
        return -1;
    }
    LOG_INF("Video stream stopped");

    return 0;
}
