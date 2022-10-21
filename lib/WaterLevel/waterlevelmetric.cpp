#include "waterlevelmetric.hpp"

bool WaterLevelMetric::measure() {
    bool reading = water_level_source.isHigh();
    addToHistory(reading);
    return reading;
}