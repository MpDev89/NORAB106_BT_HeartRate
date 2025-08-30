# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(NORAB106_BT_HeartRate)

FILE(GLOB app_sources src/*.c)
zephyr_library_include_directories(${ZEPHYR_BASE}/samples/bluetooth)
zephyr_include_directories(inc)        #Add this line

target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/peripheral/gpio_abstract.c)  #Add this line
target_sources(app PRIVATE src/peripheral/peripheral.c)  #Add this line
target_sources(app PRIVATE src/peripheral/bt_abstract.c)  #Add this line
target_sources(app PRIVATE src/peripheral/adc_abstract.c)  #Add this line
