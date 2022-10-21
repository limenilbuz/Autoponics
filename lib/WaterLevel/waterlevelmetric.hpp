#ifndef WATER_LEVEL_METRIC_HPP
#define WATER_LEVEL_METRIC_HPP

#include "waterlevel.hpp"
#include "metric.hpp"

/** 
 * @brief Water level metric class that returns the water level reading from the water level source.
*/
class WaterLevelMetric : public Metric<bool> {
    public:
    /**
     * @brief Constructs a water level metric object.
     * @param source The specified source of water level readings (mock or real)
    */
    WaterLevelMetric(WaterLevel& source) : water_level_source{source} {}

    /**
     * @brief Returns the water level from the water level source
    */
    bool measure();

    private:
    /**
     * @var Water level source object, defined as a reference to a WaterLevel class for dynamic dispatch of WaterLevel interface sub-classes.
    */

    WaterLevel& water_level_source;
};


#endif