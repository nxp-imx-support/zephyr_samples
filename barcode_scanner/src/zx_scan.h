/*
 * Copyright 2024, NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ZX_SCAN_H
#define ZX_SCAN_H

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/** C STD include group */
#include <cinttypes>
#include <cstring>
#include <cstdlib>
#include <cstdint>

/** CPP STD/STL include group */
#include <vector>

/** ZXing-CPP include group */
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

#define ZX_SCAN_THREAD_START_PRIO (-1)
#define ZX_SCAN_THREAD_NORMAL_PRIO (8)
#define ZX_SCAN_THREAD_STACK_SIZE (65536)

typedef struct _zx_scan_param
{
    uint8_t prio_normal; /** specifies thread prio at normal run. */
    uint16_t width, height;
}zx_scan_param_t;

typedef struct _zx_scan
{
    struct k_mutex lock;
    struct k_condvar cond;
    k_tid_t thread_id;

    uint8_t * frame;
}zx_scan_t;

void ZX_ScanTask(zx_scan_t *const scan, zx_scan_param_t const *const param, void*);

#endif // ZX_SCAN_H
