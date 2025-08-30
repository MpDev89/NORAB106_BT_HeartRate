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
 * @file adc_abstract.h
 * @brief this file handles adc device tree macros and create an abstract interface based on Zephyr ADC function 
 * to manage adc channels based on data structure.
 *
 * The following functions will be implemented:
 * - adc_init() : Initialize the ADC device and configure the channels for sampling.
 * - Ff_buffer_add() : Add new data to the FIFO buffer for a specific channel in the adc abstract array.
 * - data_is_valid() : Check if the new data is valid based on ranges.
 * - spike_counter() : If data is not valid, increment the spike counter for the specific channel in the adc abstract array.
 * - adc_get_media() : Calculate the average of the data in the FIFO buffer for a specific channel in the adc abstract array.
 * - adc_read_ch_data() : Read data from the adc abstract pins and store it in the FIFO buffer for each channel.
 * 
 * 
 * @author Marconatale Parise
 * @date 09 June 2025
 *
 */

#ifndef __ADC_ABSTRACT_H__
#define __ADC_ABSTRACT_H__

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include "common.h"


#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || !DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif

#define ADC_NUM_CHANNELS	DT_PROP_LEN(DT_PATH(zephyr_user), io_channels)

#if ADC_NUM_CHANNELS > 2
#error "Currently only 1 or 2 channels supported in this sample"
#endif

#if ADC_NUM_CHANNELS == 2 && !DT_SAME_NODE( \
	DT_PHANDLE_BY_IDX(DT_PATH(zephyr_user), io_channels, 0), \
	DT_PHANDLE_BY_IDX(DT_PATH(zephyr_user), io_channels, 1))
#error "Channels have to use the same ADC."
#endif

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
	ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

/* Data of ADC io-channels specified in devicetree. */
static const struct adc_dt_spec adc_channels[] = {
	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels,
			     DT_SPEC_AND_COMMA)
};

#define NO_ADC_SPIKE  0 // no spike detected
#define LIMIT_ADC_SPIKE 3 // spike detected and data is valid
#define VDD	3300.0F            
#define RANGE   (4096*300)/VDD   //300mV range for 12bit resolution and VDD=3.3V
#define ADC_RESOLUTION 12

#define HR_CH 0
#define BATT_CH 1

#define BUFFER_SIZE 5 // number of samples to store in the buffer 

typedef struct 
{
  uint8_t    length; // index of the data in the buffer
  int32_t	 data_set[BUFFER_SIZE];
  int32_t   data_media;
  uint16_t   count; 
}Fifo_buf_t;

typedef struct 
{
	uint8_t			pin;
	bool 			  status;
	Fifo_buf_t		fbuf;
  uint8_t     counter_spike;
  
}Adc_t;



/**
 * @brief Initialize the ADC device
 *
 * Initialize the ADC device and configure the channels for sampling.
 *
 * @param no_parameter
 *
 * @return void
 */
void adc_init();


/**
 * @brief fill fifo with new data
 *
 * Add new data to the FIFO buffer for a specific channel in the adc abstract array.
 *
 * @param channel 8-bit value that indicate channel of adc abstract array 
 * @param data_read 16-bit value new data to be added to the buffer
 * @param size 8-bit value that indicate number of adc abstract array 
 *
 * @return void
 */
void Ff_buffer_add(uint8_t channel, int32_t data_read, uint8_t size);

/**
 * @brief verify data validity
 *
 * Check if the new data is valid based on ranges
 *
 * @param a 8-bit struct pointer to an n-element data array
 * @param channel 8-bit value that indicate channel of adc abstract array 
 * @param data_read 16-bit value new data to be added to the buffer
 * @param size 8-bit value that indicate number of adc abstract array 
 *
 * @return bool true if data is valid, false otherwise
 */
bool data_is_valid(uint8_t channel, uint16_t data_read, uint8_t size);

/**
 * @brief count number of spikes
 *
 * If data is not valid, increment the spike counter for the specific channel in the adc abstract array.
 *
 * @param channel 8-bit value that indicate channel of adc abstract array 
 * @param data_read 16-bit value new data to be added to the buffer
 * @param size 8-bit value that indicate number of adc abstract array 
 *
 * @return uint8_t the number of spikes detected for the specific channel
 */
uint8_t spike_counter(uint8_t channel,  uint16_t data_read, uint8_t size);

/**
 * @brief Get media from three samples
 *
 * Calculate the average of the data in the FIFO buffer for a specific channel in the adc abstract array.
 *
 * @param channel 8-bit value that indicate channel of adc abstract array 
 * @param size 8-bit value that indicate number of adc abstract array 
 *
 * @return uint16_t the calculated average value
 */
uint16_t adc_get_media (uint8_t channel, uint8_t size);

/**
 * @brief Read data from adc abstract pins
 *
 * Read data from the adc abstract pins and store it in the FIFO buffer for each channel.
 *
 * @param a 8-bit struct pointer to an n-element data array
 * @param channel 8-bit value that indicate channel of adc abstract array 
 * @param size 8-bit value that indicate number of adc abstract array 
 *
 * @return uint16_t the read data value for the specific channel
 */
uint16_t adc_read_ch_data (uint8_t channel, uint8_t size);


#endif