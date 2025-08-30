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
 * @file main.c
 * @brief main function to initialize peripherals and provide threads for bluetooth 
 * and peripheral handling
 *
 * This file contains the main function that initializes the peripherals and
 * provides heartbeat and battery service over Bluetooth each 5 seconds or using dedicated 
 * buttons.
 * 
 * @author Marconatale Parise
 * @date 09 June 2025
 *
 */

#include "peripheral.h"
#include "common.h"


/* size of stack area used by each thread */
#define STACKSIZE 1024
/* scheduling priority used by each thread */
#define PRIORITY 7
#define HIGH_PRIORITY 5

void main(void){
	peripheral_init();
}

void bt_thread(void){
	while(1){
		bt_hrs_set();
		bt_bas_set();
		k_sleep(K_MSEC(5000));
  }
}

void bt_event_thread(void){
	while(1){
		 // Check if Button 1 is pressed
		if(is_button1_pressed()){
			bt_hrs_set();
		}
		// Check if Button 2 is pressed
		if(is_button2_pressed()){
			bt_bas_set();
		}
		k_sleep(K_MSEC(50));
		
  }
}

void perip_thread(void){
	while(1){
		set_heart_rate_value();
		set_battery_perc();
		k_sleep(K_MSEC(100));
  }
}

K_THREAD_DEFINE(bt_thread_id, STACKSIZE, bt_thread, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(bt_event_thread_id, STACKSIZE, bt_event_thread, NULL, NULL, NULL, HIGH_PRIORITY, 0, 0);
K_THREAD_DEFINE(perip_thread_id, STACKSIZE, perip_thread, NULL, NULL, NULL,	PRIORITY, 0, 0);