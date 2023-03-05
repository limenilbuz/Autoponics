#ifndef REAL_PH_HPP
#define REAL_PH_HPP

#include "ph.hpp"
#include "driver/adc.h"
#include "esp_adc_cal.h"

/**
 * @brief pH Source which interfaces with the pH sensor.
*/
class RealPH : public PH {
    private:
    adc1_channel_t ph_read_pin;
    esp_adc_cal_characteristics_t adc1_chars;

    public:
    /**
     * @brief Constructs a RealPH object.
    */
    explicit RealPH(esp_adc_cal_characteristics_t adc1_chars);

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



#endif