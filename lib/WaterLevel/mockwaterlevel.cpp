#include "mockwaterlevel.hpp"

#include <stdlib.h>

bool MockWaterLevel::isHigh() {
    return rand() % 2;
}