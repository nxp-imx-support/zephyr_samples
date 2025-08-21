/*
 * Copyright 2024,2025 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "zx_scan.h"

#include <iostream>
#include <sstream>
#include <string>
#include <functional>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(zx_scan, CONFIG_BARCODE_ZXING_LOG_LEVEL);

size_t ZX_ResultFormatString(char *const result_str, size_t str_len,
                             const ZXing::Result &result)
{
    size_t result_str_len = 0;

    // Format basic information
    result_str_len += snprintf(
        result_str + result_str_len, str_len - result_str_len,
        "  Text      : \"%s\"\n"
        "  Format    : %s\n"
        "  Identifier: %s\n"
        "  Position  : %3.3dx%3.3d %3.3dx%3.3d\n"
        "              %3.3dx%3.3d %3.3dx%3.3d\n"
        "  Rotation  : %d deg\n"
        "  EC Level  : %s\n"
        "  Version   : %s\n",
        std::string(result.bytes().asString()).c_str(),
        ZXing::ToString(result.format()).c_str(),
        result.symbologyIdentifier().c_str(),
        result.position().topLeft().x, result.position().topLeft().y,
        result.position().bottomLeft().x, result.position().bottomLeft().y,
        result.position().bottomRight().x, result.position().bottomRight().y,
        result.position().topRight().x, result.position().topRight().y,
        result.orientation(),
        result.ecLevel().empty() ? "N/A" : result.ecLevel().c_str(),
        result.version().empty() ? "N/A" : result.version().c_str()
    );

    return result_str_len;
}

int ZX_SendFrame(zx_scan_t *const scan, uint32_t frame_no, uint8_t *frame_buf,
                 size_t frame_size)
{
    if (k_mutex_lock(&scan->lock, K_NO_WAIT) == 0) {
        LOG_DBG("ZXing send frame %d", frame_no);
        scan->frame_no = frame_no;
        memcpy(scan->frame_buffer, frame_buf, frame_size);
        k_mutex_unlock(&scan->lock);
        k_condvar_signal(&scan->cond);
        return 0;
    } else {
        LOG_WRN("ZXing skip frame %d (busy)", frame_no);
        return -EBUSY;
    }
}

void ZX_ScanTask(zx_scan_t *const scan, const zx_scan_param_t *const param, void *)
{
    assert(scan);

#ifdef CONFIG_BARCODE_TIME_MEASUREMENT
    uint64_t stime, etime;
#endif

    LOG_INF("ZXing task starting");
    scan->thread_id = k_current_get();
    k_thread_name_set(scan->thread_id, "zx_scan");
    k_mutex_init(&scan->lock);
    k_condvar_init(&scan->cond);

    // Allocate frame buffer (RGBX, 4 bytes per pixel)
    size_t frame_size = param->width * param->height * 4;
    scan->frame_buffer = new (std::nothrow) uint8_t[frame_size];
    if (!scan->frame_buffer) {
        LOG_ERR("ZXing failed to allocate frame buffer");
        return;
    }

    // RAII guard for automatic cleanup
    std::unique_ptr<uint8_t[], std::function<void(uint8_t *)>> frame_alloc_guard(
        scan->frame_buffer, [](uint8_t *ptr) { delete[] ptr; }
    );

    // Allocate result string buffer
    scan->results_str = new (std::nothrow) char[ZX_SCAN_RESULTS_BUF_SIZE];
    if (!scan->results_str) {
        LOG_ERR("ZXing failed to allocate result string buffer");
        return;
    }

    // RAII guard for automatic cleanup
    std::unique_ptr<char[], std::function<void(char *)>> results_str_alloc_guard(
        scan->results_str, [](char *ptr) { delete[] ptr; }
    );

    // Initialize result string
    scan->results_str_len = 0;
    scan->preserve_old_result = true;
    scan->results_str_len += snprintf(
        scan->results_str + scan->results_str_len,
        ZX_SCAN_RESULTS_BUF_SIZE - scan->results_str_len,
        "No QR-Code detected yet.\n"
    );

    // Configure decoder hints
    ZXing::DecodeHints hints;
    hints.setTryHarder(true);
    hints.setTryRotate(true);
    hints.setTryInvert(false);
    hints.setTryDownscale(true);
    hints.setReturnErrors(false);
    hints.setFormats(ZXing::BarcodeFormat::MatrixCodes);
    hints.setTextMode(ZXing::TextMode::Plain);
    hints.setEanAddOnSymbol(ZXing::EanAddOnSymbol::Read);

    LOG_INF("ZXing task started successfully");
    k_thread_priority_set(k_current_get(), param->prio_normal);

    // Main loop
    while (true) {
        // Wait for new frame
        k_mutex_lock(&scan->lock, K_FOREVER);
        k_condvar_wait(&scan->cond, &scan->lock, K_FOREVER);

#ifdef CONFIG_BARCODE_TIME_MEASUREMENT
        stime = k_cyc_to_us_near64(k_cycle_get_64());
#endif // CONFIG_BARCODE_TIME_MEASUREMENT

        LOG_DBG("ZXing processing frame %d", scan->frame_no);

        // Create image view from frame buffer
        ZXing::ImageView qr_image(
            (const uint8_t *)scan->frame_buffer,
            param->width,
            param->height,
            ZXing::ImageFormat::RGBX
        );

        LOG_DBG("ZXing scanning (w: %d, h: %d)", param->width, param->height);

        // Perform barcode detection
        scan->results = ZXing::ReadBarcodes(qr_image, hints);

        // Format results
        if (scan->results.empty()) {
            if (!scan->preserve_old_result) {
                scan->preserve_old_result = true;
                scan->results_str_len += snprintf(
                    scan->results_str + scan->results_str_len,
                    ZX_SCAN_RESULTS_BUF_SIZE - scan->results_str_len,
                    "\nNo QR-Code found. Preserving last result.\n"
                );
            }
        } else {
            uint32_t result_no = 0;
            scan->results_str_len = 0;
            scan->preserve_old_result = false;

            // Header
            scan->results_str_len += snprintf(
                scan->results_str + scan->results_str_len,
                ZX_SCAN_RESULTS_BUF_SIZE - scan->results_str_len,
                "Scan result from frame %8.8d :\n",
                scan->frame_no
            );

            // Process up to MAX results
            for (auto&& result : scan->results) {
                scan->results_str_len += snprintf(
                    scan->results_str + scan->results_str_len,
                    ZX_SCAN_RESULTS_BUF_SIZE - scan->results_str_len,
                    "\nQR-Code NO.%d\n", result_no
                );

                scan->results_str_len += ZX_ResultFormatString(
                    scan->results_str + scan->results_str_len,
                    ZX_SCAN_RESULTS_BUF_SIZE - scan->results_str_len,
                    result
                );

                if (++result_no >= ZX_SCAN_RESULTS_NUM_MAX) {
                    break;
                }
            }

            // Final summary
            scan->results_str_len += snprintf(
                scan->results_str + scan->results_str_len,
                ZX_SCAN_RESULTS_BUF_SIZE - scan->results_str_len,
                "\nFound %d QR-Code(s)",
                scan->results.size()
            );

            if (scan->results.size() > ZX_SCAN_RESULTS_NUM_MAX) {
                scan->results_str_len += snprintf(
                    scan->results_str + scan->results_str_len,
                    ZX_SCAN_RESULTS_BUF_SIZE - scan->results_str_len,
                    ", %d result(s) omitted.\n",
                    scan->results.size() - ZX_SCAN_RESULTS_NUM_MAX
                );
            } else {
                scan->results_str_len += snprintf(
                    scan->results_str + scan->results_str_len,
                    ZX_SCAN_RESULTS_BUF_SIZE - scan->results_str_len,
                    ".\n"
                );
            }
        }

        // Null-terminate string
        scan->results_str[scan->results_str_len] = '\0';

#ifdef CONFIG_BARCODE_TIME_MEASUREMENT
        etime = k_cyc_to_us_near64(k_cycle_get_64());
        LOG_WRN("ZXing process frame %d took %lld us", scan->frame_no, etime - stime);
#endif // CONFIG_BARCODE_TIME_MEASUREMENT

        LOG_DBG("ZXing scan complete");

        k_mutex_unlock(&scan->lock);
    }
}
