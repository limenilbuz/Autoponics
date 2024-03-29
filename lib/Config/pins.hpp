#pragma once

#include "driver/gpio.h"
#include "driver/adc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define WATER_LEVEL_READ_PIN GPIO_NUM_23
#define PH_READ_PIN ADC1_CHANNEL_0
#define EC_READ_PIN ADC1_CHANNEL_3
#define TEMPERATURE_READ_PIN ADC1_CHANNEL_6

#define PH_UP_PUMP_RELAY_PIN GPIO_NUM_32
#define PH_DOWN_PUMP_RELAY_PIN GPIO_NUM_33
#define EC_PUMP_RELAY_PIN GPIO_NUM_25

#define MULTISAMPLING_MODE 1
#define MULTISAMPLING_NUM_SAMPLES 64
#define MULTISAMPLING_WAIT_TIME (1 / portTICK_PERIOD_MS)