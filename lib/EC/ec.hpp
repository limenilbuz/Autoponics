#pragma once

/**
 * @brief EC source interface class.
 * Defines one method for getting the EC.
 */
class EC
{

public:
    /**
     * @brief Gets the EC.
     */
    virtual double getEC() = 0;
};
