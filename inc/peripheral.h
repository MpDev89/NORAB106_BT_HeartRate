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
 * @file peripheral.h
 * @brief this file contain the functions prototype to link the peripher to functionalities
 *
 * The following functions will be implemented:
 * - peripheral_init() to initialize the peripherals
 * - reset_interrupt_ob1203() to reset the OB1203 interrupt status
 * - get_status_interrupt_ob1203() to get the OB1203 interrupt status
 * 
 * 
 * @author Marconatale Parise
 * @date 09 June 2025
 *
 */

#ifndef __PERIPHERAL_H__
#define __PERIPHERAL_H__

#include "common.h"
#include "gpio_abstract.h"
#include "bt_abstract.h"
#include "adc_abstract.h"

typedef struct
{
    float adc_batt_mV;
    uint8_t bt_batt_lvl;
    float adc_heart_rate_mV;
    uint8_t bt_heart_rate;
}Perip_t;

#define HR_MIN_VALUE 60.0F
#define HR_MAX_VALUE 160.0F
#define BATT_MIN_PERC_VALUE 0.0F
#define BATT_MAX_PERC_VALUE 100.0F


/**
 * @brief Initialize peripherals
 *
 * Initialize peripherals to asserve the functionalities of the system. 
 *
 * NO parameters are required for this function.
 *
 * @return void
 */
void peripheral_init();

/**
 * @brief Verify if Button 1 is pressed
 *
 * Check if Button 1 is pressed by reading the GPIO interrupt status.
 * 
 * No parameters are required for this function.
 *
 * @return bool true if Button 1 is pressed, false otherwise
 */
bool is_button1_pressed();

/**
 * @brief Verify if Button 2 is pressed
 *
 * Check if Button 2 is pressed by reading the GPIO interrupt status.
 * 
 * No parameters are required for this function.
 *
 * @return bool true if Button 2 is pressed, false otherwise
 */
bool is_button2_pressed();

void bt_bas_set(void);
void bt_hrs_set(void);
void set_heart_rate_value(void);
void set_battery_perc(void);

#endif /* __PERIPHERAL_H__ */