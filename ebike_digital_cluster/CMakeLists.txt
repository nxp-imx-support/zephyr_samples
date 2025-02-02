# Copyright 2024, NXP
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(ebike_digital_cluster)

target_include_directories(app PRIVATE
    src
    src/generated
    src/custom
    src/generated/guider_customer_fonts
    src/generated/guider_fonts
    src/generated/images
)

FILE(GLOB gui_guider_src
    src/generated/*.c
    src/custom/*.c
    src/generated/guider_customer_fonts/*.c
    src/generated/guider_fonts/*.c
    src/generated/images/*.c
)

target_sources_ifdef(CONFIG_EDC_SHELL app PRIVATE
  src/view_shell.c
)

target_sources(app PRIVATE
  src/main.c
  src/model.c
  src/control.c
  ${gui_guider_src}
)

target_compile_definitions(app PRIVATE LV_LVGL_H_INCLUDE_SIMPLE)
