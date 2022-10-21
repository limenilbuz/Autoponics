#ifndef WATER_LEVEL_HPP
#define WATER_LEVEL_HPP

/**
 * @brief Water level interface class.
 * Defines one method for checking if the water level is high enough.
*/
class WaterLevel {

    public:
    /**
     * @brief Returns a boolean value whether the water level is high enough or not.
     * Mock functionality comes from a coin flip probability.
    */
    virtual bool isHigh() = 0;
};


#endif