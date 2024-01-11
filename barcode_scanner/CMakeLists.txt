# Copyright 2024, NXP
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

set(SHIELD nxp_x_rpi_cam_int)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(barcode_scanner)

# include directories for headers
set (ZXING_HEADERS_DIR
  "src/zxing-cpp/core/src"
#  "src/zxing-cpp/core/src/oned"
#  "src/zxing-cpp/core/src/qrcode"
)

# build readers
#  set (ZXING_HEADERS_DIR ${ZXING_HEADERS_DIR}
#      src/zxing-cpp/core/src/qrcode/
#  )

# build writers
#  set (ZXING_HEADERS_DIR ${ZXING_HEADERS_DIR}
#      src/zxing-cpp/core/src/BitMatrix.h
#      src/zxing-cpp/core/src/BitMatrixIO.h
#      src/zxing-cpp/core/src/Matrix.h
#      src/zxing-cpp/core/src/MultiFormatWriter.h
#      src/zxing-cpp/core/src/Range.h
#  )

# end of public header set

set (ZXING_SOURCES
  # base
  "src/zxing-cpp/core/src/BarcodeFormat.cpp"
  "src/zxing-cpp/core/src/BitArray.cpp"
  "src/zxing-cpp/core/src/BitMatrix.cpp"
  "src/zxing-cpp/core/src/BitMatrixIO.cpp"
  "src/zxing-cpp/core/src/CharacterSet.cpp"
  "src/zxing-cpp/core/src/ConcentricFinder.cpp"
  "src/zxing-cpp/core/src/ECI.cpp"
  "src/zxing-cpp/core/src/GenericGF.cpp"
  "src/zxing-cpp/core/src/GenericGFPoly.cpp"
  "src/zxing-cpp/core/src/GTIN.cpp"
  "src/zxing-cpp/core/src/Utf.cpp"
  "src/zxing-cpp/core/src/ZXBigInteger.cpp"

  #libzueci
  "src/zxing-cpp/core/src/libzueci/zueci.c"

  #aztec common
  # no such files

  # datamatrix common
  "src/zxing-cpp/core/src/datamatrix/DMBitLayout.cpp"
  "src/zxing-cpp/core/src/datamatrix/DMVersion.cpp"

  #oned commmon
  "src/zxing-cpp/core/src/oned/ODUPCEANCommon.cpp"
  "src/zxing-cpp/core/src/oned/ODCode128Patterns.cpp"

  #qr-code common
  "src/zxing-cpp/core/src/qrcode/QRCodecMode.cpp"
  "src/zxing-cpp/core/src/qrcode/QRErrorCorrectionLevel.cpp"
  "src/zxing-cpp/core/src/qrcode/QRVersion.cpp"
)
#zephyr_linker_sources(SECTIONS linker_chre.ld)

# build readers
  zephyr_compile_definitions(ZXING_BUILD_READERS)
  set (ZXING_SOURCES ${ZXING_SOURCES}
    #reader
    "src/zxing-cpp/core/src/BinaryBitmap.cpp"
    "src/zxing-cpp/core/src/BitSource.cpp"
    "src/zxing-cpp/core/src/Content.cpp"
    "src/zxing-cpp/core/src/DecodeHints.cpp"
    "src/zxing-cpp/core/src/GlobalHistogramBinarizer.cpp"
    "src/zxing-cpp/core/src/GridSampler.cpp"
    "src/zxing-cpp/core/src/HRI.cpp"
    "src/zxing-cpp/core/src/HybridBinarizer.cpp"
    "src/zxing-cpp/core/src/MultiFormatReader.cpp"
    "src/zxing-cpp/core/src/PerspectiveTransform.cpp"
    "src/zxing-cpp/core/src/ReadBarcode.cpp"
    "src/zxing-cpp/core/src/ReedSolomonDecoder.cpp"
    "src/zxing-cpp/core/src/Result.cpp"
    "src/zxing-cpp/core/src/ResultPoint.cpp"
    "src/zxing-cpp/core/src/TextDecoder.cpp"
    "src/zxing-cpp/core/src/WhiteRectDetector.cpp"

    #aztec reader
    "src/zxing-cpp/core/src/aztec/AZDecoder.cpp"
    "src/zxing-cpp/core/src/aztec/AZDetector.cpp"
    "src/zxing-cpp/core/src/aztec/AZReader.cpp"

    #datamatrix reader
    "src/zxing-cpp/core/src/datamatrix/DMDataBlock.cpp"
    "src/zxing-cpp/core/src/datamatrix/DMDecoder.cpp"
    "src/zxing-cpp/core/src/datamatrix/DMDetector.cpp"
    "src/zxing-cpp/core/src/datamatrix/DMReader.cpp"

    #maxicode reader
    "src/zxing-cpp/core/src/maxicode/MCBitMatrixParser.cpp"
    "src/zxing-cpp/core/src/maxicode/MCDecoder.cpp"
    "src/zxing-cpp/core/src/maxicode/MCReader.cpp"

    #oned reader
    "src/zxing-cpp/core/src/oned/ODCodabarReader.cpp"
    "src/zxing-cpp/core/src/oned/ODCode39Reader.cpp"
    "src/zxing-cpp/core/src/oned/ODCode93Reader.cpp"
    "src/zxing-cpp/core/src/oned/ODCode128Reader.cpp"
    "src/zxing-cpp/core/src/oned/ODDataBarCommon.cpp"
    "src/zxing-cpp/core/src/oned/ODDataBarReader.cpp"
    "src/zxing-cpp/core/src/oned/ODDataBarExpandedBitDecoder.cpp"
    "src/zxing-cpp/core/src/oned/ODDataBarExpandedReader.cpp"
    "src/zxing-cpp/core/src/oned/ODITFReader.cpp"
    "src/zxing-cpp/core/src/oned/ODMultiUPCEANReader.cpp"
    "src/zxing-cpp/core/src/oned/ODReader.cpp"
    "src/zxing-cpp/core/src/oned/ODRowReader.cpp"

    #pdf417 reader
    "src/zxing-cpp/core/src/pdf417/PDFBarcodeValue.cpp"
    "src/zxing-cpp/core/src/pdf417/PDFBoundingBox.cpp"
    "src/zxing-cpp/core/src/pdf417/PDFCodewordDecoder.cpp"
    "src/zxing-cpp/core/src/pdf417/PDFDecoder.cpp"
    "src/zxing-cpp/core/src/pdf417/PDFDetectionResult.cpp"
    "src/zxing-cpp/core/src/pdf417/PDFDetectionResultColumn.cpp"
    "src/zxing-cpp/core/src/pdf417/PDFDetector.cpp"
    "src/zxing-cpp/core/src/pdf417/PDFModulusGF.cpp"
    "src/zxing-cpp/core/src/pdf417/PDFModulusPoly.cpp"
    "src/zxing-cpp/core/src/pdf417/PDFReader.cpp"
    "src/zxing-cpp/core/src/pdf417/PDFScanningDecoder.cpp"

    #qr-code reader
    "src/zxing-cpp/core/src/qrcode/QRBitMatrixParser.cpp"
    "src/zxing-cpp/core/src/qrcode/QRDataBlock.cpp"
    "src/zxing-cpp/core/src/qrcode/QRDecoder.cpp"
    "src/zxing-cpp/core/src/qrcode/QRDetector.cpp"
    "src/zxing-cpp/core/src/qrcode/QRFormatInformation.cpp"
    "src/zxing-cpp/core/src/qrcode/QRReader.cpp"
  )

# build writers
#  zephyr_compile_definitions(ZXING_BUILD_WRITERS)
#  set (ZXING_SOURCES ${ZXING_SOURCES}
#    #writer
#    "src/zxing-cpp/core/src/ReedSolomonEncoder.cpp"
#    "src/zxing-cpp/core/src/TextEncoder.cpp"
#    "src/zxing-cpp/core/src/MultiFormatWriter.cpp"
#
#    # oned writer
#    "src/zxing-cpp/core/src/oned/ODCodabarWriter.cpp"
#    "src/zxing-cpp/core/src/oned/ODCode39Writer.cpp"
#    "src/zxing-cpp/core/src/oned/ODCode93Writer.cpp"
#    "src/zxing-cpp/core/src/oned/ODCode128Writer.cpp"
#    "src/zxing-cpp/core/src/oned/ODEAN8Writer.cpp"
#    "src/zxing-cpp/core/src/oned/ODEAN13Writer.cpp"
#    "src/zxing-cpp/core/src/oned/ODITFWriter.cpp"
#    "src/zxing-cpp/core/src/oned/ODUPCEWriter.cpp"
#    "src/zxing-cpp/core/src/oned/ODUPCAWriter.cpp"
#    "src/zxing-cpp/core/src/oned/ODWriterHelper.cpp"
#
#    #qr-code writer
#    "src/zxing-cpp/core/src/qrcode/QREncoder.cpp"
#    "src/zxing-cpp/core/src/qrcode/QRMaskUtil.cpp"
#    "src/zxing-cpp/core/src/qrcode/QRMatrixUtil.cpp"
#    "src/zxing-cpp/core/src/qrcode/QRWriter.cpp"
#  )

target_include_directories(app PRIVATE
    ${ZXING_HEADERS_DIR}
)

zephyr_compile_definitions(Zxing_EXPORTS)

target_sources(app PRIVATE
    ${ZXING_SOURCES}
    src/main.cpp
    src/zx_scan.cpp
)
