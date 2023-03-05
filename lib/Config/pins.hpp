#include "driver/gpio.h"
#include "driver/adc.h"

namespace config {
    #define WATER_LEVEL_READ_PIN GPIO_NUM_23
    #define TEMPERATURE_READ_PIN ADC1_CHANNEL_6
    #define PH_READ_PIN ADC1_CHANNEL_0
}
