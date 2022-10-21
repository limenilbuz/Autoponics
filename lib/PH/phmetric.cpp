#include "phmetric.hpp"

double PHMetric::measure() {
    double reading = ph_source.getPH();
    this->addToHistory(reading);
    return reading;
}