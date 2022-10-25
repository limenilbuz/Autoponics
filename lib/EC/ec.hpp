#ifndef EC_HPP
#define EC_HPP


/**
 * @brief EC source interface class.
 * Defines one method for getting the EC.
*/
class EC {

    public:
    
    /**
     * @brief Gets the EC.
    */
    virtual double getEC() = 0;

};

/**
 * @brief PPM conversion enum type.
 * Names the common conversions from EC to PPM.
*/
enum PPMConversion {Hanna, Eutech, Truncheon};


#endif