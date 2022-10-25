#include "ecmetric.hpp"

double ECMetric::measure() {
    double reading = ec_source.getEC();
    this->addToHistory(reading);
    return reading;
}