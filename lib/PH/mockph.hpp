#pragma once
#include "ph.hpp"

/**
 * @brief Mock pH source which inherits from the PH interface.
 */
class MockPH : public PH
{
public:
    /**
     * @var Represents the value of the mock pH data.
     */
    double mock_ph_data;

    /**
     * @brief Constructs the mock pH data class.
     * @param mock_ph_data A baseline for the mock pH readings.
     */
    MockPH(double mock_ph_data) : mock_ph_data{mock_ph_data} {}

    /**
     * @brief Returns the mock pH data plus or minus 2.
     */
    double getPH() override;
};
