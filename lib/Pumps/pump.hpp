#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
using milliliter = int;


/**
 * @brief Pump class. Simple interface for working with the DC motor peristaltic pump.
*/
class Pump {
    private:
    /**
     * @var Represents the time in milliseconds it takes to pump 1mL. Acts as the time calibration point for all activations.
     * Must be calculated for each individual pump prior to use to ensure correct dosage during activation.
    */
    int multiplier;

    /**
     * @var Corresponds to the pin on the ESP32 board that sends the logic signals to the corresponding relay.
     * Our relays should be "normally open mode". This means that our relays will not allow current to flow unless they receive
     * a logic '1' from our microcontroller.
    */
    gpio_num_t relay_control_pin;
    
    public:
    /**
     * @brief Constructs the pump object.
    */
    Pump(int flow_rate_multiplier, gpio_num_t relay_pin);

    /**
     * @brief Turns on the pump. The time that the pump runs depends on the volume passed in as a parameter.
     * 
     * Time based approach works since we are not using any PWM modules for the pump, 
     * and are relying on a constant voltage to power the DC motor.
     * 
     * There are several pros and cons to this approach.
     * 
     * Pros:
     * 
     * Time-based approach is simple to understand.
     * 
     * The logic is very easy to implement.
     * 
     * 
     * Cons:
     * 
     * Unidirectional pump flow.
     * 
     * Fixed voltage = fixed flow rate.
     * 
     * Calibration needs to be very good to ensure accurate dosing.
     * 
     * Relay has some activation time, might cause inaccurate dosage.
    */
    void activate(milliliter volume);

    /**
     * @brief Turns on the pump for exactly 5 seconds. Use this method with a volume measuring tool to determine the multiplier.
     * The formula for the multipler should be (volume of mL) / 5000.
    */
    void calibrate();
};