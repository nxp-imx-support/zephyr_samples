/*
 * Copyright 2024,2025 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ZX_SCAN_H
#define ZX_SCAN_H

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/* C++ Standard Library includes */
#include <cinttypes>
#include <cstring>
#include <cstdlib>
#include <cstdint>

/* ZXing-CPP includes */
#include <BitMatrix.h>
#include <BitMatrixIO.h>
#include <DecodeHints.h>
#include <DecoderResult.h>
#include <ImageView.h>
#include <BinaryBitmap.h>
#include <qrcode/QRReader.h>
#include <qrcode/QRDecoder.h>
#include <MultiFormatReader.h>
#include <ReadBarcode.h>

/* Configuration */
#define ZX_SCAN_THREAD_START_PRIO        (-1)
#define ZX_SCAN_THREAD_NORMAL_PRIO        (8)
#define ZX_SCAN_THREAD_STACK_SIZE       (16384)

#define ZX_SCAN_RESULTS_BUF_SIZE        (16384)
#define ZX_SCAN_RESULTS_NUM_MAX          (2)

/* Scan parameters */
typedef struct _zx_scan_param {
    uint8_t prio_normal;  /**< Thread priority during normal operation */
    uint16_t width;       /**< Frame width in pixels */
    uint16_t height;      /**< Frame height in pixels */
} zx_scan_param_t;

/* Scan context */
typedef struct _zx_scan {
    struct k_mutex lock;            /**< Mutex for frame access */
    struct k_condvar cond;          /**< Condition variable for frame update */
    k_tid_t thread_id;              /**< Task ID of scan thread */

    uint8_t *frame_buffer;          /**< Frame buffer */
    uint32_t frame_no;              /**< Current frame number */

    ZXing::Results results;         /**< Detected barcode results */
    char *results_str;              /**< Formatted result string */
    uint32_t results_str_len;       /**< Length of formatted string */
    bool preserve_old_result;       /**< Whether to preserve prior result on no detection */
} zx_scan_t;

/* Public API */

/**
 * @brief Formats a ZXing::Result into a human-readable string.
 * @param result_str Output buffer
 * @param str_len Length of output buffer
 * @param result Barcode result to format
 * @return Number of characters written (not including null terminator)
 */
size_t ZX_ResultFormatString(char *const result_str, size_t str_len,
                             const ZXing::Result &result);

/**
 * @brief Sends a new video frame to the scanner thread.
 * @param scan Pointer to scan context
 * @param frame_no Frame identifier
 * @param frame_buf Pointer to frame data (RGBX)
 * @param frame_size Size of frame data
 * @return 0 on success, -EBUSY if mutex locked
 */
int ZX_SendFrame(zx_scan_t *const scan, uint32_t frame_no, uint8_t *frame_buf,
                 size_t frame_size);

/**
 * @brief Main barcode scanning task.
 * @param scan Pointer to scan context
 * @param param Scan configuration
 * @param unused Unused parameter (required by k_thread_create)
 */
void ZX_ScanTask(zx_scan_t *const scan, const zx_scan_param_t *const param, void *);

#endif /* ZX_SCAN_H */