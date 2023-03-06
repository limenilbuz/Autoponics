#include "dfrobot_ph_meter_pro_v2.hpp"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "pins.hpp"

RealPH::RealPH(esp_adc_cal_characteristics_t adc1_chars) : ph_read_pin{PH_READ_PIN}, adc1_chars{adc1_chars}
{
    adc1_config_channel_atten(ph_read_pin, ADC_ATTEN_DB_11);
}

double RealPH::getPH()
{
    auto ph_mV = getPH_mV();
    auto ph = voltageToPH(ph_mV);
    return ph;
}

uint32_t RealPH::getPH_mV()
{
    uint32_t ph_mV = esp_adc_cal_raw_to_voltage(adc1_get_raw(ph_read_pin), &this->adc1_chars);
    return ph_mV;
}

double RealPH::voltageToPH(uint32_t voltage)
{
    float slope = (7.0 - 4.0) / ((1500.0 - 1500.0) / 3.0 - (2032.44 - 1500.0) / 3.0);
    float intercept = 7.0 - slope * (1500.0 - 1500.0) / 3.0;
    double value = slope * (voltage - 1500.0) / 3.0 + intercept;
    return value;
}