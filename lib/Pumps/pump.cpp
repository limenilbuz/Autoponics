#include "pump.hpp"

Pump::Pump(int flow_rate_multiplier, gpio_num_t relay_pin) 
 : multiplier{flow_rate_multiplier}, relay_control_pin{relay_pin} {
    gpio_reset_pin(relay_control_pin);
    gpio_set_direction(relay_control_pin, GPIO_MODE_OUTPUT);
 };

void Pump::activate(milliliter volume) {
    // represents how many milliseconds it takes for the pump to dose out the volume specified by the parameter
    int time_interval = volume * multiplier; 
    
    gpio_set_level(relay_control_pin, 1);
    vTaskDelay(time_interval / portTICK_PERIOD_MS); // WARNING: might need to offset the interval to account for relay activation time!
    gpio_set_level(relay_control_pin, 0);
}

void Pump::calibrate() {
    gpio_set_level(relay_control_pin, 1);
    vTaskDelay(5000 / portTICK_PERIOD_MS); // WARNING: might need to offset the interval to account for relay activation time!
    gpio_set_level(relay_control_pin, 0);
}