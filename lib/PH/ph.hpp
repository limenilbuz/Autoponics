#ifndef PH_HPP
#define PH_HPP


/**
 * @brief PH source interface class.
 * Defines one method for getting the pH.
*/
class PH {

    public:
    
    /**
     * @brief Gets the pH.
    */
    virtual uint32_t getPH() = 0;

};

#endif