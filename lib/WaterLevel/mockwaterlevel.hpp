#ifndef MOCK_WATER_LEVEL_HPP
#define MOCK_WATER_LEVEL_HPP

#include "waterlevel.hpp"

/**
 * @brief Mock water level sensor which inherits from the water level interface.
*/
class MockWaterLevel : public WaterLevel{
    public:
    bool isHigh() override;
};



#endif