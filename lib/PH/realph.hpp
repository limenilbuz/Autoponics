#ifndef REAL_PH_HPP
#define REAL_PH_HPP

#include "ph.hpp"

/**
 * @brief pH Source which interfaces with the pH sensor.
*/
class RealPH : public PH {
    
    public:
    /**
     * @brief Constructs a RealPH object.
    */
    RealPH() = default;

    /**
     * @brief Gets the pH from the sensor.
    */
    double getPH() override;
};



#endif