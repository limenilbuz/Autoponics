#ifndef MOCK_PH_HPP
#define MOCK_PH_HPP
#include "ph.hpp"


/** 
 * @brief Mock pH source which inherits from the PH interface.
*/
class MockPH : public PH {
    public:
    /**
     * @var Represents the value of the mock pH data.
    */
    double mock_ph_data;
    
    /** 
     * @brief Constructs the mock pH data class.
    */
    MockPH(double mock_ph_data) : mock_ph_data{mock_ph_data} {    }

    /**
     * @brief Returns the mock pH data.
    */
    double getPH() override {
        return mock_ph_data;
    }
};

#endif