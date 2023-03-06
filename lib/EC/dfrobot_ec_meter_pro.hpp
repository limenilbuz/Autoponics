#pragma once

#include "ec.hpp"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define RES2 820.0
#define ECREF 200.0
#define GDIFF (30 / 1.8)
#define VR0 0.223
#define G0 2
#define I (1.24 / 10000)

/**
 * @brief Represent an interface to the DFRobot EC Meter Pro sensor.
 * https://www.dfrobot.com/product-2565.html
 */
class DFRobotECMeterPro : public EC
{
private:
    double calibration_value = 1.0;
    adc1_channel_t temp_read_pin;
    adc1_channel_t ec_read_pin;
    esp_adc_cal_characteristics_t adc1_chars;

public:
    /**
     * @brief Constructs the sensor object.
     */
    explicit DFRobotECMeterPro(esp_adc_cal_characteristics_t adc1_chars);

    /**
     * @brief Returns the EC value from the sensor.
     */
    double getEC() override;

    /**
     * @brief Converts a voltage to the EC in us/cm.
     */
    double voltageToEC(double voltage);

    /**
     * @brief Converts a voltage to EC in us/cm depending on the temperature.
     */
    double voltageToEC(double voltage, double temperature);

    /**
     * @brief Converts a voltage to temperature in Celsius.
     */
    double voltageToTemp(uint32_t voltage);

    /**
     * @brief Gets the millivolt value of the temperature pin.
     */
    uint32_t getTemp_mV();

    /**
     * @brief Gets the millivolt value of the EC pin.
     */
    uint32_t getEC_mV();
};
