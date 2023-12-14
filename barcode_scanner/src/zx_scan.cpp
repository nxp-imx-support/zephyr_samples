/*
 * Copyright 2024, NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "zx_scan.h"

#include <iostream>
#include <sstream>
#include <string>
#include <functional>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(zx_scan, LOG_LEVEL_INF);

void ZX_ScanTask(zx_scan_t *const scan, zx_scan_param_t const *const param, void*)
{
    assert(scan);
    int32_t ret;

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

    memset(scan->frame, 0, frame_size);

    char * result_str = new char[1024];
    uint32_t result_str_len = 0;
    if(result_str == nullptr)
    {
        LOG_ERR("frame buffer alloc fail");
        return;
    }
    /** release resources on exit */
    std::unique_ptr<char*, std::function<void(char**)>> result_str_alloc_guard(
        &(result_str), [](char** ptr) { delete[] (*ptr); }
    );

    ZXing::DecodeHints hints = ZXing::DecodeHints();
    hints.setTryHarder(true);
    hints.setTryRotate(true);
    hints.setTryInvert(false);
    hints.setTryDownscale(true);
    //hints.setIsPure(true);
	//hints.setBinarizer(Binarizer::FixedThreshold);
    hints.setReturnErrors(true);
    hints.setFormats(ZXing::BarcodeFormat::MatrixCodes);
    hints.setTextMode(ZXing::TextMode::Plain);
	hints.setEanAddOnSymbol(ZXing::EanAddOnSymbol::Read);

    LOG_INF("zx_scan task start ok\n");
    /** return to normal priority */
    k_thread_priority_set(k_current_get(), param->prio_normal);

    while (true)
    {
        k_mutex_lock(&scan->lock, K_FOREVER);

        /** wait for frame is copied */
        k_condvar_wait(&scan->cond, &scan->lock, K_FOREVER);

        LOG_DBG("scan new frame");
        extern const uint8_t qr_code_array_rgb_24bit[];
        ZXing::ImageView qr_image(
            (const uint8_t*)scan->frame,
            param->width,
            param->height,
            ZXing::ImageFormat::RGBX //FIXME
        );

        LOG_DBG("scan start (w: %d, h: %d)", param->width, param->height);

        auto results = ZXing::ReadBarcodes(qr_image, hints);

        LOG_INF("scan end, found %d result", results.size());

        int result_no = 1;

        for (auto&& result : results)
        {
            result_str_len = 0;
            result_str_len += snprintf((result_str + result_str_len)
            , (1024 - result_str_len),
                "barcode found:\r\n"
                "Text       :\"%s\"\r\n"
                "Bytes      :%s\r\n"
                "Format     :%s\r\n"
                "Identifier :%s\r\n"
                "Content    :%s\r\n"
                "HasECI     :%s\r\n"
                "Position   :%dx%d %dx%d %dx%d %dx%d\r\n"
                //, result.text(ZXing::TextMode::HRI).c_str()
                , std::string((hints.textMode() == ZXing::TextMode::ECI ?
                            result.bytesECI() : result.bytes()).asString()).c_str()
                , ZXing::ToHex(
                           hints.textMode() == ZXing::TextMode::ECI ?
                            result.bytesECI() : result.bytes()).c_str()
                , ZXing::ToString(result.format()).c_str()
                , result.symbologyIdentifier().c_str()
                , ZXing::ToString(result.contentType()).c_str()
                , result.hasECI() ? "true" : "false"
                /** Position Group */
                    , result.position().topLeft().x, result.position().topLeft().y
                    , result.position().bottomLeft().x, result.position().bottomLeft().y
                    , result.position().bottomRight().x, result.position().bottomRight().y
                    , result.position().topRight().x, result.position().topRight().y
                /** End Position */
            );
            result_str_len += snprintf((result_str + result_str_len)
            , (1024 - result_str_len),
                "Rotation   :%d deg\r\n"
                "IsMirrored :%s\r\n"
                "IsInverted :%s\r\n"
                "EC Level   :%s\r\n"
                "Version    :%s\r\n"
                "Error      :%s\r\n"
                , result.orientation()
                , result.isMirrored() ? "true" : "false"
                , result.isInverted() ? "true" : "false"
                , result.ecLevel().empty() ? "N/A" : result.ecLevel().c_str()
                , result.version().empty() ? "N/A" : result.version().c_str()
                , ZXing::ToString(result.error()).empty() ? "N/A"
                    : ZXing::ToString(result.error()).c_str()
            );

            LOG_INF("result %d\r\n: %s", result_no, result_str);
            ++result_no;
        }

        LOG_DBG("scan finish");

        k_mutex_unlock(&scan->lock);
        //k_thread_suspend(scan->thread_id);
    }

}
