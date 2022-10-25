#include "mockec.hpp"
#include <stdlib.h>

double MockEC::getEC() {
    return mock_ec_data + ((2 * (rand() / (float) RAND_MAX)) - 1);
}
