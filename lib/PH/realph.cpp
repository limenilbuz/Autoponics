#include "realph.hpp"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "pins.hpp"

RealPH::RealPH() : ph_read_pin{PH_READ_PIN} {
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, (adc_bits_width_t)ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
    adc1_config_width((adc_bits_width_t)ADC_WIDTH_BIT_DEFAULT);
    adc1_config_channel_atten(ph_read_pin, ADC_ATTEN_DB_11);
}

uint32_t RealPH::getPH() {
    uint32_t ph_mV = esp_adc_cal_raw_to_voltage(adc1_get_raw(ph_read_pin), &adc1_chars);
    return ph_mV;
}