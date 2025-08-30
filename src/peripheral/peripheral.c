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
 * @file peripheral.c
 * @brief peripheral function declaration linked to functionalities
 *
 * This implementation file provides function to set up the peripheral pins based on the 
 * functionalities required by the system.
 * 
 * @author Marconatale Parise
 * @date 09 June 2025
 *
 */
#include "peripheral.h"


extern Gpio_t gpio_a[NUM_GPIO_PERIP]; // array of gpio peripheral
extern Adc_t adc_a[ADC_NUM_CHANNELS]; // array of gpio peripheral
Perip_t perip = {.adc_batt_mV = 0.0F, .bt_batt_lvl = 0U, .adc_heart_rate_mV = 0.0F,  .bt_heart_rate = 0U};


/***********************************************************
 Function Definitions
***********************************************************/
void peripheral_init() {
  int err;

  //Button 1 to start reading measurements
  gpio_enable(gpio_a, BTN1_ch, true);
  gpio_enable_interrupt(gpio_a, BTN1_ch, true);
  gpio_init(gpio_a, BTN1_ch, NUM_GPIO_PERIP);
  gpio_configure(gpio_a, BTN1_ch, NUM_GPIO_PERIP);
  gpio_configure_interrupt(gpio_a, BTN1_ch, NUM_GPIO_PERIP); 

  //Button 2 to stop reading measurements
  gpio_enable(gpio_a, BTN2_ch, true);
  gpio_enable_interrupt(gpio_a, BTN2_ch, true);
  gpio_init(gpio_a, BTN2_ch, NUM_GPIO_PERIP);
  gpio_configure(gpio_a, BTN2_ch, NUM_GPIO_PERIP);
  gpio_configure_interrupt(gpio_a, BTN2_ch, NUM_GPIO_PERIP); 

  adc_init();  

	err = bt_enable(NULL);
	if (err) {
		LOG("Bluetooth init failed (err %d)\n", err);
		return;
	}

	bt_ready();
	bt_conn_auth_cb_reg();

  LOG("Peripherals initialized successfully.\n");
}


bool is_button1_pressed(){
  bool status = get_gpio_interrupt_status(gpio_a, BTN1_ch);
  if (status){
    reset_gpio_interrupt(gpio_a, BTN1_ch);
  }
	return status; 
}

bool is_button2_pressed(){
  bool status = get_gpio_interrupt_status(gpio_a, BTN2_ch);
  if (status){
    reset_gpio_interrupt(gpio_a, BTN2_ch);
  }
	return status; 
}

void bt_bas_set(void){
  LOG("Battery adc voltage: %.1f mV.", perip.adc_batt_mV);
	bt_bas_set_battery_level(perip.bt_batt_lvl);
  LOG("Battery level: %d %%.", perip.bt_batt_lvl);
}

void bt_hrs_set(void){
    LOG("Heartrate adc voltage: %.1f mV.",perip.adc_heart_rate_mV);
  	bt_hrs_notify(perip.bt_heart_rate);
    LOG("Heartrate: %d bpm.",perip.bt_heart_rate);
}


void set_heart_rate_value(void){
  uint16_t hr_voltage_mv = adc_read_ch_data(HR_CH, ADC_NUM_CHANNELS);
  perip.adc_heart_rate_mV = (float)hr_voltage_mv;
  perip.bt_heart_rate = (uint8_t)(perip.adc_heart_rate_mV * (HR_MAX_VALUE - HR_MIN_VALUE) / VDD  + HR_MIN_VALUE);
  
}

void set_battery_perc(void){
  uint16_t batt_voltage_mv = adc_read_ch_data(BATT_CH, ADC_NUM_CHANNELS);
  perip.adc_batt_mV = (float)batt_voltage_mv;
  perip.bt_batt_lvl = (uint8_t)(perip.adc_batt_mV * (BATT_MAX_PERC_VALUE - BATT_MIN_PERC_VALUE) / VDD  + BATT_MIN_PERC_VALUE);
}


