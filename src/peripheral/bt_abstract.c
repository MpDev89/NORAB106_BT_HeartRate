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
 * @file bt_abstract.c
 * @brief bt abstract layer function definitions
 *
 * This implementation file provides functions to manage bt services and characteristics 
 * based on zephyr bluetooth functions.
 * 
 * @author Marconatale Parise
 * @date 09 June 2025
 *
 */

#include "bt_abstract.h"

/***********************************************************
 Static Function Definitions
***********************************************************/
static void connected(struct bt_conn *conn, uint8_t err){
	if (err) {
		LOG("Connection failed (err 0x%02x)\n", err);
	} else {
		LOG("Device Connected\n");
	}
}

static void disconnected(struct bt_conn *conn, uint8_t reason){
	LOG("Device Disconnected (reason 0x%02x)\n", reason);
}

static void auth_cancel(struct bt_conn *conn){
	char addr[BT_ADDR_LE_STR_LEN];
	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));
	LOG("Pairing cancelled: %s\n", addr);
}

static struct bt_conn_auth_cb auth_cb_display = {
	.cancel = auth_cancel,
};

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};

/***********************************************************
 Function Definitions
***********************************************************/
void bt_ready(void){
	int err;
	LOG("Bluetooth initialized");
	err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
	if (err) {
		LOG("Advertising failed to start (err %d)\n", err);
		return;
	}
	LOG("Advertising successfully started\n");
}

void bt_conn_auth_cb_reg(){
	int err;
	err = bt_conn_auth_cb_register(&auth_cb_display);
	if (err) {
		LOG("Failed to register auth callbacks (err %d)\n", err);
	} else {
		LOG("Auth callbacks registered\n");
	}
}



