#ifndef REAL_WATER_LEVEL_HPP
#define REAL_WATER_LEVEL_HPP

#include "waterlevel.hpp"
#include "driver/gpio.h"
#include "pins.hpp"

/**
 * @brief Represents an interface to the DFRobot Non-contact Digital Liquid Level Sensor. 
 * https://www.dfrobot.com/product-1493.html
*/
class DFRobotNonContactLiquidLevel : public WaterLevel {
    private:
    gpio_num_t read_pin;

    public:
    DFRobotNonContactLiquidLevel() : read_pin{WATER_LEVEL_READ_PIN} {
        gpio_reset_pin(read_pin);
        gpio_set_direction(read_pin, GPIO_MODE_INPUT);
        gpio_set_pull_mode(read_pin, GPIO_PULLDOWN_ONLY);
    }
    bool isHigh() override;
};



#endif