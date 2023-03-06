#pragma once

#include "driver/gpio.h"
#include "driver/adc.h"

#define WATER_LEVEL_READ_PIN GPIO_NUM_23
#define TEMPERATURE_READ_PIN ADC1_CHANNEL_6
#define PH_READ_PIN ADC1_CHANNEL_0
#define EC_READ_PIN ADC1_CHANNEL_3
