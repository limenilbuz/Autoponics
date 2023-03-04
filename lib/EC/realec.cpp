#include "realec.hpp"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "pins.hpp"

RealEC::RealEC() : calibration_value{1.0}, temp_read_pin{TEMPERATURE_READ_PIN} {
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, (adc_bits_width_t)ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
    adc1_config_width((adc_bits_width_t)ADC_WIDTH_BIT_DEFAULT);
    adc1_config_channel_atten(temp_read_pin, ADC_ATTEN_DB_11);
}

double RealEC::getEC() {
    return 1.0;
}

uint32_t RealEC::getTempMV() {
    uint32_t temp_mV = esp_adc_cal_raw_to_voltage(adc1_get_raw(temp_read_pin), &adc1_chars);
    return temp_mV;

}

double RealEC::voltageToEC(double voltage) {
    double value = 100000 * voltage / RES2 / ECREF * calibration_value;
    return value;                
}

double RealEC::voltageToEC(double voltage, double temperature) {
    double ecvalueRaw = 100000 * voltage / RES2 / ECREF * calibration_value;
    double value = ecvalueRaw / (1.0 + 0.02 * (temperature - 25.0));
    return value;
}

double RealEC::voltageToTemp(uint32_t voltage) {
    double normalized_voltage = (double) voltage / 1000; 
    double Rpt1000 = (normalized_voltage/GDIFF+VR0)/I/G0;
    double temp = (Rpt1000-1000)/3.85;
    return temp;
}