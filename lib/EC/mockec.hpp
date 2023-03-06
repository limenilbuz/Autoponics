#pragma once
#include "ec.hpp"

/**
 * @brief Mock EC source which inherits from the EC interface.
 */
class MockEC : public EC
{
public:
    /**
     * @var Represents the value of the mock EC data.
     */
    double mock_ec_data;

    /**
     * @brief Constructs the mock EC data class.
     * @param mock_ec_data A baseline for the mock EC readings.
     */
    MockEC(double mock_ec_data) : mock_ec_data{mock_ec_data} {}

    /**
     * @brief Returns the mock EC data plus or minus 2.
     */
    double getEC() override;
};

