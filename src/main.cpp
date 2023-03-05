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
#include "realwaterlevel.hpp"
#include "realph.hpp"
#include "mockec.hpp"
#include "ecmetric.hpp"
#include "realec.hpp"

#define TASK_STACK_SIZE 4096
#define TASK_PRIORITY 1
#define LED GPIO_NUM_2
#define SIMULATION_MODE 0

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


void pHTask(void* pvParameters) {
    PHMetric* ph_metric = static_cast<PHMetric*>(pvParameters);

    while (true) {
        auto measurement = ph_metric->measure();
        auto average = ph_metric->average();
        ESP_LOGI("PH MEASUREMENT",
                 "The current measurement is: %f\nThe average is: %f",
                  measurement, average);

        vTaskDelay(1000 / portTICK_PERIOD_MS);

    }
}

void waterLevelTask(void* pvParameters) {
    RealWaterLevel wl_source{};

    while (true) {
        auto measurement = wl_source.isHigh();
        ESP_LOGI("WATER LEVEL MEASUREMENT", "WATER LEVEL MEASUREMENT %d", measurement);
        gpio_set_level(LED, measurement);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void ecTask(void* pvParameters) {
    
    static RealEC ec_source;

    while (true) {
        auto measurement = ec_source.getTempMV();
        ESP_LOGI("EC MEASUREMENT", "Temperature voltage:  %d mV | %f Celsius", measurement, ec_source.voltageToTemp(measurement));
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


static RealPH ph_source;
static RealEC ec_source;
static RealWaterLevel wl_source;

static PHMetric ph(ph_source);
ECMetric ec(ec_source);
WaterLevelMetric wl(wl_source);

/**
 * @brief The main function. Currenlty blinker example.
*/
extern "C" void app_main(void) {
    gpio_reset_pin(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    char* taskName = pcTaskGetName(NULL);

    //xTaskCreate(ecTask, "ec task", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
    //xTaskCreate(waterLevelTask, "water level task", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
    xTaskCreate(pHTask, "ph task", TASK_STACK_SIZE, &ph, TASK_PRIORITY, NULL);
}