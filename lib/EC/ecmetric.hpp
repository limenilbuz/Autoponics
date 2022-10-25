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
     * @brief Constructs a ECMetric class object.
     * @param source Represents the source of the EC measurement (mock or real).
    */
    ECMetric(EC& source) : ec_source{source} {}

    /**
     * @brief Returns the measured EC from the source.
    */
    double measure();

};

#endif