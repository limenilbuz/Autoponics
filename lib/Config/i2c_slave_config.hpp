#pragma once

// I2C
#define I2C_SLAVE_ADDRESS 0x08 // Address for I2C connection
#define I2C_SLAVE_SDA_IO 18
#define I2C_SLAVE_SCL_IO 19
#define I2C_SLAVE_NUM 0
#define I2C_SLAVE_FREQ_HZ 400000
#define I2C_SLAVE_TIMEOUT_MS 1000
#define I2C_SLAVE_RX_BUF_LEN 256
#define I2C_SLAVE_TX_BUF_LEN 256

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

