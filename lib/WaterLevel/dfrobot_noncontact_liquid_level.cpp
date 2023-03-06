#include "dfrobot_noncontact_liquid_level.hpp"

bool DFRobotNonContactLiquidLevel::isHigh() {
    return gpio_get_level(read_pin);
}