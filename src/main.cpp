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
#include "dfrobot_noncontact_liquid_level.hpp"
#include "dfrobot_ph_meter_pro_v2.hpp"
#include "mockec.hpp"
#include "ecmetric.hpp"
#include "dfrobot_ec_meter_pro.hpp"
#include "pins.hpp"
#include "driver/i2c.h"

#define TASK_STACK_SIZE 4096
#define TASK_PRIORITY 1
#define LED GPIO_NUM_2
#define SIMULATION_MODE 0

// I2C
#define I2C_SLAVE_ADDRESS 0x08 // Address for I2C connection
#define I2C_SLAVE_SDA_IO 18
#define I2C_SLAVE_SCL_IO 19
#define I2C_SLAVE_NUM 0
#define I2C_SLAVE_FREQ_HZ 400000
#define I2C_SLAVE_TIMEOUT_MS 1000
#define DATA_LENGTH 512
#define I2C_SLAVE_RX_BUF_LEN 256
#define I2C_SLAVE_TX_BUF_LEN 256

static esp_adc_cal_characteristics_t adc1_chars;

struct Threshold
{
    double PH, EC;
};

static Threshold THRESHOLD{.PH = 6.0, .EC = 2.0};

struct SystemMeasurements
{
    double PH;
    double EC;
    double Temperature;
    bool WaterLevel;
};

static SystemMeasurements system_measurements{.PH = 0.0, .EC = 0.0, .Temperature = 25.0, .WaterLevel = false};

void pHTask(void *pvParameters)
{
    static DFRobotPHMeterProV2 ph_source{adc1_chars};
    static PHMetric ph(ph_source);

    while (true)
    {
        auto ph_mV = ph_source.getPH_mV();
        auto ph = ph_source.voltageToPH(ph_mV);
        ESP_LOGI("PH MEASUREMENT",
                 "%d mV\t%.2f",
                 (int)ph_mV, ph);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void waterLevelTask(void *pvParameters)
{
    DFRobotNonContactLiquidLevel wl_source{};

    while (true)
    {
        auto measurement = wl_source.isHigh();
        ESP_LOGI("WATER LEVEL MEASUREMENT", "WATER LEVEL MEASUREMENT %d", measurement);
        gpio_set_level(LED, measurement);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void ecTask(void *pvParameters)
{

    static DFRobotECMeterPro ec_source{adc1_chars};

    while (true)
    {
        auto ec_mv = ec_source.getEC_mV();
        auto temp_mv = ec_source.getTemp_mV();
        auto temp = ec_source.voltageToTemp(temp_mv);
        auto ec = ec_source.voltageToEC(ec_mv, temp);
        ESP_LOGI("EC", "EC voltage: %d mV | %.2f us/cm\tTemperature voltage:  %d mV | %.2f Celsius",
                 ec_mv, ec, temp_mv, temp);
        system_measurements.EC = ec;
        system_measurements.Temperature = temp;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// I2C setup
static esp_err_t i2c_slave_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_SLAVE,
        .sda_io_num = I2C_SLAVE_SDA_IO, // Replace with the SDA pin number you are using
        .scl_io_num = I2C_SLAVE_SCL_IO, // Replace with the SCL pin number you are using
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
    };

    conf.slave.addr_10bit_en = 0;
    conf.slave.slave_addr = I2C_SLAVE_ADDRESS;

    i2c_param_config(I2C_SLAVE_NUM, &conf);

    return i2c_driver_install(I2C_SLAVE_NUM, conf.mode, 256, 256, 0);
}

/**
 * @brief The main function. Currenlty blinker example.
 */
extern "C" void app_main(void)
{

    ESP_ERROR_CHECK(i2c_slave_init());
    uint8_t i2c_slave_buf[DATA_LENGTH];
    ESP_LOGI("I2C Connection", "I2C initialized successfully");

    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, (adc_bits_width_t)ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
    adc1_config_width((adc_bits_width_t)ADC_WIDTH_BIT_DEFAULT);

    gpio_reset_pin(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    char *taskName = pcTaskGetName(NULL);

    // xTaskCreate(ecTask, "ec task", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
    // xTaskCreate(waterLevelTask, "water level task", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
    xTaskCreate(pHTask, "ph task", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
}