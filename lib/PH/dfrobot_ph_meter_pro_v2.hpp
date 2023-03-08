#pragma once

#include "ph.hpp"
#include "driver/adc.h"
#include "esp_adc_cal.h"

/**
 * @brief Represent an interface to the DFRobot PH meter pro v2 sensor.
 * https://www.dfrobot.com/product-2069.html
 */
class DFRobotPHMeterProV2 : public PH
{
private:
    adc1_channel_t ph_read_pin;
    esp_adc_cal_characteristics_t adc1_chars;

public:
    /**
     * @brief Constructs the sensor object.
     */
    explicit DFRobotPHMeterProV2(esp_adc_cal_characteristics_t adc1_chars);

    /**
     * @brief Gets the pH from the sensor.
     */
    double getPH() override;

    /**
     * @brief Converts a voltage to the pH.
     */
    uint32_t getPH_mV();

    /**
     * @brief Returns the real pH data.
     */
    double voltageToPH(uint32_t voltage);
};
