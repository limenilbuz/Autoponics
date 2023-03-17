#include "dfrobot_ec_meter_pro.hpp"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "pins.hpp"

DFRobotECMeterPro::DFRobotECMeterPro(esp_adc_cal_characteristics_t adc1_chars) 
: calibration_value{1.0}, temp_read_pin{TEMPERATURE_READ_PIN}, ec_read_pin{EC_READ_PIN}, adc1_chars{adc1_chars} {
    adc1_config_channel_atten(temp_read_pin, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ec_read_pin, ADC_ATTEN_DB_11);
}

double DFRobotECMeterPro::getEC() {
    auto ec_mv = getEC_mV();
    auto temp_mv = getTemp_mV();
    auto temp = voltageToTemp(temp_mv);
    auto ec = voltageToEC(ec_mv, temp);
    return ec;
}

uint32_t DFRobotECMeterPro::getTemp_mV() {
    #if MULTISAMPLING_MODE
    uint32_t temp_mV = 0;
    for (int i = 0; i < MULTISAMPLING_NUM_SAMPLES; ++i) {
        temp_mV += esp_adc_cal_raw_to_voltage(adc1_get_raw(temp_read_pin), &this->adc1_chars);
        vTaskDelay(MULTISAMPLING_WAIT_TIME); // block for 1ms after each read to not overload the adc
    }
    return temp_mV / MULTISAMPLING_NUM_SAMPLES;
    #else
    uint32_t temp_mV = esp_adc_cal_raw_to_voltage(adc1_get_raw(temp_read_pin), &this->adc1_chars);
    return temp_mV;
    #endif
}

uint32_t DFRobotECMeterPro::getEC_mV() {
    #if MULTISAMPLING_MODE
    uint32_t ec_mV = 0;
    for (int i = 0; i < MULTISAMPLING_NUM_SAMPLES; ++i) {
        ec_mV += esp_adc_cal_raw_to_voltage(adc1_get_raw(ec_read_pin), &this->adc1_chars);
        vTaskDelay(MULTISAMPLING_WAIT_TIME); // block for 1ms after each read to not overload the adc
    }
    return ec_mV / MULTISAMPLING_NUM_SAMPLES;
    #else
    uint32_t ec_mV = esp_adc_cal_raw_to_voltage(adc1_get_raw(ec_read_pin), &this->adc1_chars);
    return ec_mV;
    #endif
}

double DFRobotECMeterPro::voltageToEC(double voltage) {
    // adapted from: https://github.com/cdjq/DFRobot_ECPRO/blob/master/DFRobot_ECPRO.cpp
    double value = 100000 * voltage / RES2 / ECREF * calibration_value;
    return value;                
}

double DFRobotECMeterPro::voltageToEC(double voltage, double temperature) {
    // adapted from: https://github.com/cdjq/DFRobot_ECPRO/blob/master/DFRobot_ECPRO.cpp
    double ecvalueRaw = 100000 * voltage / RES2 / ECREF * calibration_value;
    double value = ecvalueRaw / (1.0 + 0.02 * (temperature - 25.0));
    return value;
}

double DFRobotECMeterPro::voltageToTemp(uint32_t voltage) {
    // adapted from: https://github.com/cdjq/DFRobot_ECPRO/blob/master/DFRobot_ECPRO.cpp
    double normalized_voltage = (double) voltage / 1000; 
    double Rpt1000 = (normalized_voltage/GDIFF+VR0)/I/G0;
    double temp = (Rpt1000-1000)/3.85;
    return temp;
}
