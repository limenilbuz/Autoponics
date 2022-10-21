#include "mockph.hpp"
#include <stdlib.h>

double MockPH::getPH() {
    return mock_ph_data + ((4 * (rand() / (float) RAND_MAX)) - 2);
}
