#include "ecmetric.hpp"

double ECMetric::measure() {
    double reading = ec_source.getEC();
    this->addToHistory(reading);
    return reading;
}

int ECMetric::toPPM(double ec_measurement, PPMConversion conversion_factor) {
    switch (conversion_factor) {
        case PPMConversion::Hanna:
            return 50 * ec_measurement;
        case PPMConversion::Eutech:
            return 64 * ec_measurement;
        case PPMConversion::Truncheon:
            return 70 * ec_measurement;
    } 
}