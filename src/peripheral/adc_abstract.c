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
 * @file adc_abstract.c
 * @brief adc abstract layer function definitions
 *
 * This implementation file provides functions to read and manage adc channels 
 * based on zephyr adc functions.
 * 
 * @author Marconatale Parise
 * @date 09 June 2025
 *
 */
#include "adc_abstract.h"

int16_t buf;

Adc_t adc_a[ADC_NUM_CHANNELS] = {
  {
    .pin = DT_IO_CHANNELS_INPUT_BY_IDX(DT_PATH(zephyr_user), 0),
    .status = true,
    .fbuf = {
      .length = BUFFER_SIZE, // Set the length of the buffer
      .data_set = {0x00, 0x00, 0x00, 0x00, 0x00}, // Initialize data_set with zeros
      .data_media = 0, // Initialize data_media to zero
      .count = 0 // Initialize count to zero
    },
    .counter_spike = 0
  }, //HR_CH
  {
    .pin = DT_IO_CHANNELS_INPUT_BY_IDX(DT_PATH(zephyr_user), 1),
    .status = true,
    .fbuf = {
      .length = BUFFER_SIZE, // Set the length of the buffer
      .data_set = {0x00, 0x00, 0x00, 0x00, 0x00}, // Initialize data_set with zeros
      .data_media = 0, // Initialize data_media to zero
      .count = 0 // Initialize count to zero
    },
    .counter_spike = 0
  } //BATT_CH
};
 
struct adc_sequence sequence = {
  .buffer = &buf,
  /* buffer size in bytes, not number of samples */
  .buffer_size = sizeof(buf),
  .resolution  = 12,
};


/***********************************************************
 Function Definitions
***********************************************************/
void adc_init(){
  int err;
  /* Configure channels individually prior to sampling. */
	for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
		if (!device_is_ready(adc_channels[i].dev)) {
			LOG_ADC("ADC controller device not ready\n");
			return;
		}

		err = adc_channel_setup_dt(&adc_channels[i]);
		if (err < 0) {
			LOG_ADC("Could not setup channel #%d (%d)\n", i, err);
			return;
		}
	}
}


void Ff_buffer_add(uint8_t channel, int32_t data_read, uint8_t size){
  if(channel < size){
    if(adc_a[channel].status){
      if(adc_a[channel].fbuf.count < adc_a[channel].fbuf.length){
        adc_a[channel].fbuf.data_set[adc_a[channel].fbuf.count] = data_read; // Fill the buffer with the new data
        adc_a[channel].fbuf.count++;
      }else{
        // Shift the buffer to make space for the new data
        for(uint8_t i = 0; i < adc_a[channel].fbuf.length - 1; i++){
          adc_a[channel].fbuf.data_set[i] = adc_a[channel].fbuf.data_set[i + 1];
        }
        adc_a[channel].fbuf.data_set[adc_a[channel].fbuf.length - 1] = data_read; // Add the new data at the end
      }
    }
  }
}


bool data_is_valid(uint8_t channel, uint16_t data_read, uint8_t size){
   if(channel < size){
    if(adc_a[channel].status){
      if(adc_a[channel].fbuf.count == adc_a[channel].fbuf.length){
        uint16_t last_value = adc_a[channel].fbuf.data_set[adc_a[channel].fbuf.count - 1]; // Get the last value in the buffer
        return ((data_read <= (last_value + RANGE)) & (data_read >= (last_value - RANGE)));
      }else{
        // If the buffer is not full, consider the data valid
        return true;
      }
    }else{
      return false; // If the channel is not valid, return false
    }
    
  }else{
    return false; // If the channel is out of bounds, return false
  }
}


uint8_t spike_counter( uint8_t channel,  uint16_t data_read, uint8_t size){
  if(channel < size){
    if(adc_a[channel].status){
      if(!data_is_valid(channel, data_read, size)){
          adc_a[channel].counter_spike ++; // First spike detected
      }else{
          adc_a[channel].counter_spike = NO_ADC_SPIKE; // Reset counter if data is valid
      }
    }
  }
  return adc_a[channel].counter_spike;
}



uint16_t adc_read_ch_data (uint8_t channel, uint8_t size){
    int err;
    int32_t val_mv;
    if (channel >= ARRAY_SIZE(adc_channels)) {
        LOG_ADC("Channel %d is out of bounds for size %d\n", channel, ARRAY_SIZE(adc_channels));
        return 0; // Return 0 or handle error as needed
    }else{

      (void)adc_sequence_init_dt(&adc_channels[channel], &sequence);
      // Read the ADC value for the specified channel and store it in the buffer "buf"
      err = adc_read(adc_channels[channel].dev, &sequence);

      if (adc_a[channel].status){
        if (err >= 0) {
          val_mv = buf;
			    err = adc_raw_to_millivolts_dt(&adc_channels[channel],&val_mv);
          if (err < 0) {
            LOG_ADC(" (value in mV not available)\n");
          } else {
            LOG_ADC("Channel %"PRId32" = %"PRId32" mV\n", channel, val_mv);
          }
          if (spike_counter(channel, val_mv, size) == NO_ADC_SPIKE || spike_counter(channel, val_mv, size) >= LIMIT_ADC_SPIKE){
            Ff_buffer_add(channel, val_mv, size); // Add new data to the FIFO buffer
            adc_a[channel].counter_spike = NO_ADC_SPIKE; // Reset spike counter if data is valid
            adc_a[channel].fbuf.data_media = adc_get_media(channel, size); // Calculate media from the buffer
          }
        }else {
          LOG_ADC("ADC reading failed.\n");
          return -1;
        }
      }
    }
  return adc_a[channel].fbuf.data_media;
}

uint16_t adc_get_media (uint8_t channel, uint8_t size){
  uint16_t media = 0;
  uint16_t sat_value = 0;
  if(channel < size){
    if(adc_a[channel].status){
      if(adc_a[channel].fbuf.count > 0){
        uint16_t sum = 0;
        for(uint8_t i = 0; i < adc_a[channel].fbuf.count; i++){
          sat_value = (uint16_t)adc_a[channel].fbuf.data_set[i];
          sum += sat_value; // Sum all values in the buffer
        }
        media = sum / adc_a[channel].fbuf.count; // Calculate the average
      }else{
        media = 0; // If no data, return zero
      }
    }
  }
  return media;
}



