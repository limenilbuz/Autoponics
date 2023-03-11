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
#include "pump.hpp"

#define TASK_STACK_SIZE 4096
#define TASK_PRIORITY 1
#define LED GPIO_NUM_2
#define SIMULATION_MODE 0

// I2C
#define I2C_SLAVE_ADDRESS 0x08 // Address for I2C connection
#define I2C_SLAVE_SDA_IO 18
#define I2C_SLAVE_SCL_IO 19
#define I2C_SLAVE_NUM 0
#define I2C_SLAVE_FREQ_HZ 1000000
#define I2C_SLAVE_TIMEOUT_MS 1000
#define I2C_SLAVE_RX_BUF_LEN 256
#define I2C_SLAVE_TX_BUF_LEN 256

static esp_adc_cal_characteristics_t adc1_chars;

struct Threshold
{
    double PH, EC;
    bool PUMP;
};

static Threshold THRESHOLD{.PH = 6.0, .EC = 2.0, .PUMP = false};

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
        auto k = RES2*ECREF*1278/100000.0/ec_mv;
        ESP_LOGI("EC", "EC voltage: %d mV | %.2f us/cm\tTemperature voltage:  %d mV | %.2f Celsius | %.2f Kvalue",
                 int(ec_mv), ec, int(temp_mv), temp, k);
        system_measurements.EC = ec;
        system_measurements.Temperature = temp;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void pumpTask(void *pvParameters)
{

    /*
    Jeon calibration of pumps states 0.31 mL/s
    .31mL per second
    1ml = 3.22 second
    1ml = 3220 millsecond
    */

    int milliseconds_to_one_ml = 3220;

    Pump ph_down_pump{milliseconds_to_one_ml, PH_DOWN_PUMP_RELAY_PIN};
    Pump ph_up_pump{milliseconds_to_one_ml, PH_UP_PUMP_RELAY_PIN};
    Pump ec_pump{milliseconds_to_one_ml, EC_PUMP_RELAY_PIN};

    while (true)
    {
        double delta = 0.5;

        if (!THRESHOLD.PUMP)
        {
            vTaskDelay(10000 / portTICK_PERIOD_MS); // only run every 100 seconds
            continue;
        }

        if (system_measurements.PH > THRESHOLD.PH + delta)
        {
            ph_down_pump.activate(1);
        }
        else if (system_measurements.PH < THRESHOLD.PH - delta)
        {
            ph_up_pump.activate(1);
        }

        if (system_measurements.EC < THRESHOLD.EC - delta)
        {
            ec_pump.activate(1);
        }

        vTaskDelay(10000 / portTICK_PERIOD_MS); // only run every 10 seconds
    }
}

// I2C setup
static esp_err_t i2c_slave_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_SLAVE,
        .sda_io_num = I2C_SLAVE_SDA_IO,
        .scl_io_num = I2C_SLAVE_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
    };

    conf.slave.addr_10bit_en = 0;
    conf.slave.slave_addr = I2C_SLAVE_ADDRESS;

    i2c_param_config(I2C_SLAVE_NUM, &conf);

    return i2c_driver_install(I2C_SLAVE_NUM, conf.mode, I2C_SLAVE_RX_BUF_LEN, I2C_SLAVE_TX_BUF_LEN, 0);
}

// Reading Threshold from the screen through I2C
void I2CReadTask(void *pvParameters)
{
    while (true)
    {
        i2c_slave_read_buffer(I2C_SLAVE_NUM, (uint8_t *)&THRESHOLD, I2C_SLAVE_RX_BUF_LEN, 100 / portTICK_PERIOD_MS);
        i2c_reset_rx_fifo(I2C_SLAVE_NUM);

        ESP_LOGI("PH", "THRESHOLD PH: %.2f", THRESHOLD.PH);
        ESP_LOGI("EC", "THRESHOLD EC: %.2f", THRESHOLD.EC);
        ESP_LOGI("PUMP", "THRESHOLD PUMP: %d", THRESHOLD.PUMP);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

// Writing system seasurements to the screen through I2C
void I2CWriteTask(void *pvParameters)
{
    while (true)
    {
        /* Random value test
        system_measurements.PH = system_measurements.PH + ((double)(rand() % 2) / 100) - ((double)(rand() % 2) / 100);
        if (system_measurements.PH > THRESHOLD.PH + 0.05 && THRESHOLD.PUMP){
            system_measurements.PH = system_measurements.PH - ((double)(rand() % 15) / 100);
        }
        else if (system_measurements.PH < THRESHOLD.PH - 0.05 && THRESHOLD.PUMP){
            system_measurements.PH = system_measurements.PH + ((double)(rand() % 15) / 100);
        }

        system_measurements.EC = system_measurements.EC - ((double)(rand() % 2) / 100);
        if (system_measurements.EC < THRESHOLD.EC && THRESHOLD.PUMP){
            system_measurements.EC = system_measurements.EC + ((double)(rand() % 20) / 100);
        }
        if (system_measurements.EC < 0){
            system_measurements.EC = 0;
        }

        system_measurements.Temperature = 60 + ((double)(rand() % 500) / 100);

        double wl_threshold = rand() % 100;
        system_measurements.WaterLevel = false;
        if (wl_threshold > 10){
            system_measurements.WaterLevel = true;

        }
        */
        i2c_slave_write_buffer(I2C_SLAVE_NUM, (uint8_t *)&system_measurements, sizeof(system_measurements), 100 / portTICK_PERIOD_MS);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void relayTest(void *pvParameters)
{
    gpio_reset_pin(PH_UP_PUMP_RELAY_PIN);
    gpio_set_direction(PH_UP_PUMP_RELAY_PIN, GPIO_MODE_OUTPUT);
    Pump ph_up_pump{3220, PH_UP_PUMP_RELAY_PIN};
    bool once = false;
    while (1)
    {
        if (!once) {
            ph_up_pump.activate(10);
            once = true;
        }

        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief The main function. Currenlty blinker example.
 */
extern "C" void app_main(void)
{

    ESP_ERROR_CHECK(i2c_slave_init());
    ESP_LOGI("I2C Connection", "I2C initialized successfully");

    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, (adc_bits_width_t)ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
    adc1_config_width((adc_bits_width_t)ADC_WIDTH_BIT_DEFAULT);

    gpio_reset_pin(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    char *taskName = pcTaskGetName(NULL);

    xTaskCreate(ecTask, "ec task", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
    xTaskCreate(waterLevelTask, "water level task", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
    xTaskCreate(pHTask, "ph task", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
    xTaskCreate(I2CReadTask, "I2C Read", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
    xTaskCreate(I2CWriteTask, "I2C Write", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
    //xTaskCreate(pumpTask, "Pump array task", TASK_STACK_SIZE, NULL, TASK_PRIORITY - 1, NULL);
    //xTaskCreate(relayTest, "asdf", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
    xTaskCreate(ecTask, "ec task", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
    // xTaskCreate(waterLevelTask, "water level task", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
    //xTaskCreate(pHTask, "ph task", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
    //xTaskCreate(I2CReadTask, "I2C Read", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
    //xTaskCreate(I2CWriteTask, "I2C Write", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
}