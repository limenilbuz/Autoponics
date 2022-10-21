/**
 * @file main.c
 * @brief Contains app_main.
*/

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "phmetric.hpp"
#include "mockph.hpp"
#include "waterlevelmetric.hpp"
#include "mockwaterlevel.hpp"

#define LED GPIO_NUM_2

/**
 * @brief The main function. Currenlty blinker example.
*/
extern "C" void app_main(void) {
    gpio_reset_pin(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);

    char* taskName = pcTaskGetName(NULL);

    MockPH ph_source{6.5};
    PHMetric ph(static_cast<PH&>(ph_source));

    MockWaterLevel wl_source;
    WaterLevelMetric wl(static_cast<WaterLevel&>(wl_source));
    while (1) {
        ESP_LOGI(taskName, "Hello world!\n");
        ESP_LOGI(taskName, "%f", ph.measure());
        gpio_set_level(LED, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(LED, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}