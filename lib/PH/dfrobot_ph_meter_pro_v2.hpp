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
    int neutral_ph_mv = 1185; // THESE TWO VALUES HAVE TO BE TESTED WITH KNOWN PH SOLUTION
    int acidic_ph_mv = 2032.44; // FOR NEUTRAL, FIND THE mV AT PH 7, FOR ACIDIC, FIND THE mV AT PH 4

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
