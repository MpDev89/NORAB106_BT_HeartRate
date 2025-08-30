/******************************************************************************
 *
 * Copyright (c) 2025 Marconatale Parise. All rights reserved.
 *
 * This file is part of proprietary software. Unauthorized copying, distribution,
 * or modification of this file, via any medium, is strictly prohibited without
 * prior written permission from the copyright holder.
 *
 *****************************************************************************/
/**
 * @file common.h
 * @brief this file contain common macros and definitions used across the project.
 *
 * 
 * @author Marconatale Parise
 * @date 09 June 2025
 *
 */
#pragma once

#include <stdint.h>
#include <math.h>
#include <stdio.h>


#ifndef __COMMON_H__
#define __COMMON_H__


#define DEBUG 1
#define DEBUG_BT 0
#define DEBUG_ADC 0

#if DEBUG
#define LOG(x,...) if(DEBUG){printf("[%u ms] " x "\n", k_uptime_get_32(), ##__VA_ARGS__);}
#define LOG_BT(x,...) if(DEBUG_BT){printf("[%u ms] " x "\n", k_uptime_get_32(), ##__VA_ARGS__);}
#define LOG_ADC(x,...) if(DEBUG_ADC){printf("[%u ms] " x "\n", k_uptime_get_32(), ##__VA_ARGS__);}
#endif

#define   ERROR_ADC_INIT    BIT(5) //error verified during void Analog_init()
#define   ERROR_GPIO_INIT   BIT(7) //error verified during const struct device *gpio_init(uint8_t index)

#define   PUSH_BTN(x)        BIT(x)

#endif