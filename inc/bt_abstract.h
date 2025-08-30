/******************************************************************************
 * Copyright (c) 2025 Marconatale Parise.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *****************************************************************************/
/**
 * @file bt_abstract.h
* @brief this file handles bluetooth device tree macros and create an abstract interface based on Zephyr bluetooth function 
 * to manage bluetooth services and characteristics.
 *
 * The following functions will be implemented:
 * - i2c_init() to initialize the i2c peripheral
 * - i2c_scan() to scan for i2c devices on the bus
 * 
 * @author Marconatale Parise
 * @date 09 June 2025
 *
 */

#ifndef __BT_ABSTRACT_H__
#define __BT_ABSTRACT_H__



#include "common.h"
#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/services/bas.h>
#include <zephyr/bluetooth/services/hrs.h>

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL,
		      BT_UUID_16_ENCODE(BT_UUID_HRS_VAL),
		      BT_UUID_16_ENCODE(BT_UUID_BAS_VAL),
		      BT_UUID_16_ENCODE(BT_UUID_DIS_VAL))
};


/**
 * @brief Bluetooth ready function
 *
 * Initialize bluetooth and start advertising with the specified parameters.
 *
 * @param no parameters
 *
 * @return void
 */
void bt_ready(void);

/**
 * @brief Manage Bluetooth connection callbacks
 *
 * The struct bt_conn_auth_cb defines function pointers for various pairing/auth events:
 * struct bt_conn_auth_cb {
 * void (*passkey_display)(struct bt_conn *conn, unsigned int passkey);
 * void (*passkey_entry)(struct bt_conn *conn);
 * void (*cancel)(struct bt_conn *conn);
 * void (*confirm)(struct bt_conn *conn);
 * void (*pairing_confirm)(struct bt_conn *conn);
 * };
 * 
 * Meaning:
 * passkey_display → Called when the device must show a 6-digit passkey to the user.
 * passkey_entry → Called when the device must ask the user to type a passkey.
 * cancel → Called when pairing/authentication is canceled.
 * confirm → Called when the device must confirm "yes/no" pairing.
 * pairing_confirm → For numeric comparison pairing, to confirm the displayed numbers match.
 *
 * no @param 
 *
 * @return void
 */
void bt_conn_auth_cb_reg(void);

#endif