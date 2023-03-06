#pragma once

#include "waterlevel.hpp"

/**
 * @brief Mock water level sensor which inherits from the water level interface.
 */
class MockWaterLevel : public WaterLevel
{
public:
    bool isHigh() override;
};
