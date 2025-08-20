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

size_t ZX_ResultFormatString(char * const result_str, size_t str_len, ZXing::Result const & result)
{
    size_t result_str_len = 0;
    result_str_len += snprintf((result_str + result_str_len)
    , (str_len - result_str_len),
        "  Text       :\"%s\"\n"
        //"  Bytes      :%s\n"
        "  Format     :%s\n"
        "  Identifier :%s\n"
        //"  Content    :%s\n"
        //"  HasECI     :%s\n"
        "  Position   :%3.3dx%3.3d %3.3dx%3.3d\n"
        "              %3.3dx%3.3d %3.3dx%3.3d\n"
        //, result.text(ZXing::TextMode::HRI).c_str()
        , std::string(result.bytes().asString()).c_str()
        //, ZXing::ToHex(result.bytes()).c_str()
        , ZXing::ToString(result.format()).c_str()
        , result.symbologyIdentifier().c_str()
        //, ZXing::ToString(result.contentType()).c_str()
        //, result.hasECI() ? "true" : "false"
        /** Position Group */
            , result.position().topLeft().x, result.position().topLeft().y
            , result.position().bottomLeft().x, result.position().bottomLeft().y
            , result.position().bottomRight().x, result.position().bottomRight().y
            , result.position().topRight().x, result.position().topRight().y
        /** End Position */
    );
    if (result_str_len < str_len)
    result_str_len += snprintf((result_str + result_str_len)
    , (str_len - result_str_len),
        "  Rotation   :%d deg\n"
        //"  IsMirrored :%s\n"
        //"  IsInverted :%s\n"
        "  EC Level   :%s\n"
        "  Version    :%s\n"
        //"  Error      :%s\n"
        , result.orientation()
        //, result.isMirrored() ? "true" : "false"
        //, result.isInverted() ? "true" : "false"
        , result.ecLevel().empty() ? "N/A" : result.ecLevel().c_str()
        , result.version().empty() ? "N/A" : result.version().c_str()
        //, ZXing::ToString(result.error()).empty() ? "N/A"
        //    : ZXing::ToString(result.error()).c_str()
    );
    return result_str_len;
}

int ZX_SendFrame(zx_scan_t *const scan, uint32_t frame_no, uint8_t *frame_buf, size_t frame_size)
{
    if (k_mutex_lock(&scan->lock, K_NO_WAIT) == 0) {
        LOG_DBG("zx_scan send frame %d", frame_no);
        scan->frame_no = frame_no;
        memcpy(scan->frame, frame_buf, frame_size);
        k_mutex_unlock(&scan->lock);
        k_condvar_signal(&scan->cond);
        return 0;
    } else {
        LOG_WRN("zx_scan skip frame %d", frame_no);
        return -EBUSY;
    }
}

void ZX_ScanTask(zx_scan_t *const scan, zx_scan_param_t const *const param, void*)
{
    assert(scan);
    int32_t ret;
#ifdef CONFIG_BARCODE_TIME_MEASUREMENT
    /** used by time measurement */
    uint64_t stime, etime;
#endif // CONFIG_BARCODE_TIME_MEASUREMENT

    LOG_INF("zx_scan task start\n");
    scan->thread_id = k_current_get();
    k_thread_name_set(scan->thread_id, "zx_scan");
    k_mutex_init(&scan->lock);
    k_condvar_init(&scan->cond);

    size_t frame_size = param->width * param->height * 4;

    scan->frame = new uint8_t[frame_size];
    if(scan->frame == nullptr)
    {
        LOG_ERR("frame buffer alloc fail");
        return;
    }
    /** release resources on exit */
    std::unique_ptr<uint8_t*, std::function<void(uint8_t**)>> frame_alloc_guard(
        &(scan->frame), [](uint8_t** ptr) { delete[] (*ptr); }
    );

    //memset(scan->frame, 0, frame_size);
    scan->results_str = new char[ZX_SCAN_RESULTS_BUF_SIZE];
    scan->results_str_len = 0;
    if (scan->results_str == nullptr)
    {
        LOG_ERR("results_str alloc fail");
        return;
    }
    /** release resources on exit */
    std::unique_ptr<char*, std::function<void(char**)>> results_str_alloc_guard(
        &(scan->results_str), [](char** ptr) { delete[] (*ptr); }
    );
    scan->results_str_len = 0;
    scan->preserve_old_result = true;
    scan->results_str_len += snprintf((scan->results_str + scan->results_str_len),
                    (ZX_SCAN_RESULTS_BUF_SIZE - scan->results_str_len),"No QR-Code detected yet.\n");

    ZXing::DecodeHints hints = ZXing::DecodeHints();
    hints.setTryHarder(true);
    hints.setTryRotate(true);
    hints.setTryInvert(false);
    hints.setTryDownscale(true);
    //hints.setIsPure(true);
	//hints.setBinarizer(Binarizer::FixedThreshold);
    hints.setReturnErrors(false);
    hints.setFormats(ZXing::BarcodeFormat::MatrixCodes);
    hints.setTextMode(ZXing::TextMode::Plain);
	hints.setEanAddOnSymbol(ZXing::EanAddOnSymbol::Read);

    LOG_INF("zx_scan task start ok\n");
    /** return to normal priority */
    k_thread_priority_set(k_current_get(), param->prio_normal);

    while (true)
    {
        /** wait for frame is copied */
        k_mutex_lock(&scan->lock, K_FOREVER);
        k_condvar_wait(&scan->cond, &scan->lock, K_FOREVER);

#ifdef CONFIG_BARCODE_TIME_MEASUREMENT
        stime = k_cyc_to_us_near64(k_cycle_get_64());
#endif // CONFIG_BARCODE_TIME_MEASUREMENT

        LOG_DBG("scan new frame");

        ZXing::ImageView qr_image(
            (const uint8_t*)scan->frame,
            param->width,
            param->height,
            ZXing::ImageFormat::RGBX //FIXME
        );

        LOG_DBG("scan start (w: %d, h: %d)", param->width, param->height);

        scan->results = ZXing::ReadBarcodes(qr_image, hints);

        /** Format results to results_str */
        if (scan->results.empty()) {
            if(scan->preserve_old_result == false) {
                scan->preserve_old_result = true;
                scan->results_str_len += snprintf((scan->results_str + scan->results_str_len),
                    (ZX_SCAN_RESULTS_BUF_SIZE - scan->results_str_len),"\nNo QR-Code found. Preserve last result.\n");
            }
        } else {
            uint32_t result_no = 0;
            scan->results_str_len = 0;
            scan->preserve_old_result = false;
            scan->results_str_len += snprintf((scan->results_str + scan->results_str_len),
                    (ZX_SCAN_RESULTS_BUF_SIZE - scan->results_str_len), "Scan result from frame %8.8d :\n", scan->frame_no);
            for (auto&& result : scan->results) {
            //auto&& result = scan->results[0];
                scan->results_str_len += snprintf((scan->results_str + scan->results_str_len),
                    (ZX_SCAN_RESULTS_BUF_SIZE - scan->results_str_len), "\nQR-Code NO.%d :\n", result_no);
                scan->results_str_len += ZX_ResultFormatString(
                    scan->results_str + scan->results_str_len, ZX_SCAN_RESULTS_BUF_SIZE - scan->results_str_len, result);
                if (++result_no >= ZX_SCAN_RESULTS_NUM_MAX) {
                    break;
                }
            }
            scan->results_str_len += snprintf((scan->results_str + scan->results_str_len),
                (ZX_SCAN_RESULTS_BUF_SIZE - scan->results_str_len), "\nFound %d QR-Code(s)", scan->results.size());
            if (scan->results.size() > ZX_SCAN_RESULTS_NUM_MAX) {
                scan->results_str_len += snprintf((scan->results_str + scan->results_str_len),
                    (ZX_SCAN_RESULTS_BUF_SIZE - scan->results_str_len), ", %d result(s) omitted.\n",
                    scan->results.size() - ZX_SCAN_RESULTS_NUM_MAX);
            } else {
                scan->results_str_len += snprintf((scan->results_str + scan->results_str_len),
                    (ZX_SCAN_RESULTS_BUF_SIZE - scan->results_str_len), ".\n", scan->results.size() - 2);
            }
        }
        scan->results_str[scan->results_str_len] = '\0';

//        for (auto it = scan->results.begin(); it < scan->results.end(); ++it)
//        {
//            if (static_cast<int>(it->error().type()) != 0)
//            {
//                it = scan->results.erase(it);
//            }
//        }
//
//        LOG_INF("scan end, found %d result", scan->results.size());
//
//        int result_no = 0;
//
//        for (auto&& result : scan->results)
//        {
//            ZX_ResultFormatString(result_str, 1024, result);
//
//            LOG_INF("result %d\n: %s", result_no, result_str);
//            ++result_no;
//        }

#ifdef CONFIG_BARCODE_TIME_MEASUREMENT
        etime = k_cyc_to_us_near64(k_cycle_get_64());
        LOG_WRN("zx_scan: process frame %d takes %lld us", scan->frame_no, etime - stime);
#endif // CONFIG_BARCODE_TIME_MEASUREMENT

        LOG_DBG("scan finish");

        k_mutex_unlock(&scan->lock);
    }
}
