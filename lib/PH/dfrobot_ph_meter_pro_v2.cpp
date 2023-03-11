#include "dfrobot_ph_meter_pro_v2.hpp"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "pins.hpp"

DFRobotPHMeterProV2::DFRobotPHMeterProV2(esp_adc_cal_characteristics_t adc1_chars) : ph_read_pin{PH_READ_PIN}, adc1_chars{adc1_chars}
{
    adc1_config_channel_atten(ph_read_pin, ADC_ATTEN_DB_11);
}

double DFRobotPHMeterProV2::getPH()
{
    auto ph_mV = getPH_mV();
    auto ph = voltageToPH(ph_mV);
    return ph;
}

uint32_t DFRobotPHMeterProV2::getPH_mV()
{
    uint32_t ph_mV = esp_adc_cal_raw_to_voltage(adc1_get_raw(ph_read_pin), &this->adc1_chars);
    return ph_mV;
}

double DFRobotPHMeterProV2::voltageToPH(uint32_t voltage)
{
    float slope = (7.0 - 4.0) / ((1500.0 - 1500.0) / 3.0 - (2032.44 - 1500.0) / 3.0);
    float intercept = 7.0 - slope * (1500.0 - 1500.0) / 3.0;
    double value = slope * (voltage - 1500.0) / 3.0 + intercept;
    return value;
}

double voltageToPH(uint32_t voltage, double temperature) {
    // https://images.hach.com/asset-get.download.jsa?id=17525673904
    #define CELSIUS_25 25
    #define NORMALIZATION_VALUE 273.15   
    double slope_at_25C = (7.0 - 4.0) / ((1500.0 - 1500.0) / 3.0 - (2032.44 - 1500.0) / 3.0);
    double slope_normalized = slope_at_25C * ((temperature + NORMALIZATION_VALUE) / (CELSIUS_25 + NORMALIZATION_VALUE));
    double value = 7 - (voltage / slope_normalized);
    return value;
}
