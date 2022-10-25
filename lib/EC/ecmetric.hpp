#ifndef EC_METRIC_HPP
#define EC_METRIC_HPP


#include "ec.hpp"
#include "metric.hpp"

/**
 * @brief EC Metric class that returns the EC reading from a EC source class object.
*/
class ECMetric : public Metric<double> {  
    private:
    /**
     * @var EC source object, defined as a reference to a EC class for dynamic dispatch of EC interface sub-classes.
    */
    EC& ec_source;
    
    public:
    /**
    * @brief PPM conversion enum type.
    * Names the common conversions from EC to PPM.
    */
    enum PPMConversion {Hanna, Eutech, Truncheon};

    /**
     * @brief Constructs a ECMetric class object.
     * @param source Represents the source of the EC measurement (mock or real).
    */
    ECMetric(EC& source) : ec_source{source} {}

    /**
     * @brief Returns the measured EC from the source.
    */
    double measure();
    
    /**
     * @brief Converts an EC measurement to PPM using one of the standard conversion factors.
     * See https://reefertilizer.com/blog/ec-to-ppm-conversion-calculator/ for more info.
     * @param ec_measurement The EC measurement, in units of us/cm.
     * @param conversion_factor The named conversion factor, enum type.
    */
    int toPPM(double ec_measurement, PPMConversion conversion_factor = PPMConversion::Hanna);

};

#endif