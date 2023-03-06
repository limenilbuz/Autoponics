#pragma once


/**
 * @brief PH source interface class.
 * Defines one method for getting the pH.
*/
class PH {

    public:
    
    /**
     * @brief Gets the pH.
    */
    virtual double getPH() = 0;

};
