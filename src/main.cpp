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
#include "realph.hpp"
#include "mockec.hpp"
#include "ecmetric.hpp"

#define TASK_STACK_SIZE 4096
#define TASK_PRIORITY 1
#define LED GPIO_NUM_2
#define SIMULATION_MODE 1

struct Threshold {
    double PH, EC;
};

Threshold THRESHOLD {.PH = 6.0, .EC = 2.0};

struct SystemMeasurements {
    double PH;
    double EC;
    bool WaterLevel;
};

SystemMeasurements system_measurements {.PH = 0.0, .EC = 0.0, .WaterLevel = false};


void blinkTask(void* pvParameters) {
    while (true) {
        gpio_set_level(LED, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(LED, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

#if SIMULATION_MODE
void pHTask(void* pvParameters) {
    PHMetric* ph_metric = static_cast<PHMetric*>(pvParameters);

    while (true) {
        auto measurement = ph_metric->measure();
        auto average = ph_metric->average();
        ESP_LOGI("PH MEASUREMENT",
                 "The current measurement is: %f\nThe average is: %f",
                  measurement, average);

        if (measurement > THRESHOLD.PH+0.5) 
        {
            ESP_LOGI("PH MEASUREMENT", "Too high!");
            gpio_set_level(LED, 1);
        }
        else if (measurement < THRESHOLD.PH-0.5)
        {
            ESP_LOGI("PH MEASUREMENT", "Too low!");
            gpio_set_level(LED, 1);
        }
        else
        {
            ESP_LOGI("PH MEASUREMENT", "Just right :)");
            gpio_set_level(LED, 0);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);

    }
}
#else

#endif

#if SIMULATION_MODE
MockPH ph_source{6.5};
MockEC ec_source{2.0};
MockWaterLevel wl_source;
#else
RealPH ph_source;
#endif
PHMetric ph(ph_source);
ECMetric ec(ec_source);
WaterLevelMetric wl(wl_source);

/**
 * @brief The main function. Currenlty blinker example.
*/
extern "C" void app_main(void) {
    gpio_reset_pin(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);

    char* taskName = pcTaskGetName(NULL);

    
    
    //xTaskCreate(blinkTask, "blink task", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
    xTaskCreate(pHTask, "ph task", TASK_STACK_SIZE, &ph, TASK_PRIORITY, NULL);
}