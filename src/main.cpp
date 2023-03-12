/**
 * @file main.c
 * @brief Contains app_main.
 */

#include "all_includes.hpp"

#define TASK_STACK_SIZE 4096
#define TASK_PRIORITY 1

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
        ESP_LOGI("PH",
                 "pH voltage: %d mV -> %.2fpH",
                 (int)ph_mV, ph);
        system_measurements.PH = ph;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void waterLevelTask(void *pvParameters)
{
    DFRobotNonContactLiquidLevel wl_source{};

    while (true)
    {
        auto measurement = wl_source.isHigh();
        ESP_LOGI("WL",
                 "Water level is: %s",
                 (measurement ? "OK" : "LOW"));
        system_measurements.WaterLevel = measurement;
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
        auto k = RES2 * ECREF * 1278 / 100000.0 / ec_mv;
        ESP_LOGI("EC", "EC voltage: %d mV -> %.2f μS/cm\t"
                       "Temp voltage:  %d mV -> %.2f° Celsius\t"
                       "%.2f Kvalue",
                 int(ec_mv), ec, int(temp_mv), temp, k);
        system_measurements.EC = ec / 1000;
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
            vTaskDelay(10000 / portTICK_PERIOD_MS); // only run every 10 seconds
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

        vTaskDelay(100 / portTICK_PERIOD_MS); // only run every 10 seconds
    }
}

// Reading Threshold from the screen through I2C
void I2CReadTask(void *pvParameters)
{
    while (true)
    {
        i2c_slave_read_buffer(I2C_SLAVE_NUM, (uint8_t *)&THRESHOLD, I2C_SLAVE_RX_BUF_LEN, 100 / portTICK_PERIOD_MS);
        i2c_reset_rx_fifo(I2C_SLAVE_NUM);

        ESP_LOGI("Threshold values", "THRESHOLD PH: %.2fpH\n"
                                     "THRESHOLD EC: %.2fμS\n"
                                     "THRESHOLD PUMP: %s",
                 THRESHOLD.PH, THRESHOLD.EC, (THRESHOLD.PUMP ? "ON" : "OFF"));
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// Writing system seasurements to the screen through I2C
void I2CWriteTask(void *pvParameters)
{
    while (true)
    {
        i2c_slave_write_buffer(I2C_SLAVE_NUM, (uint8_t *)&system_measurements, sizeof(system_measurements), 100 / portTICK_PERIOD_MS);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

extern "C" void app_main(void)
{
    ESP_ERROR_CHECK(i2c_slave_init()); // lib/Config/i2c_slave_config.hpp
    ESP_LOGI("I2C Connection", "I2C initialized successfully");

    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, (adc_bits_width_t)ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
    adc1_config_width((adc_bits_width_t)ADC_WIDTH_BIT_DEFAULT);

    xTaskCreate(ecTask,         "ecTask",         TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
    xTaskCreate(waterLevelTask, "waterLevelTask", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
    xTaskCreate(pHTask,         "pHTask",         TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
    xTaskCreate(I2CReadTask,    "I2CReadTask",    TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
    xTaskCreate(I2CWriteTask,   "I2CWriteTask",   TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
    xTaskCreate(pumpTask,       "pumpTask",       TASK_STACK_SIZE, NULL, TASK_PRIORITY - 1, NULL);
}