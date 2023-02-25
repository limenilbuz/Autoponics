#include "realwaterlevel.hpp"

bool RealWaterLevel::isHigh() {
    return gpio_get_level(read_pin);
}