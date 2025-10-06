#include "main.h"
#include "gpio.h"
#include "i2c.h"
#include "mcp9808.h"
#include "stm32l476xx.h"
#include "stm32l4xx_hal.h"
#include "usart.h"
#include <stdio.h>

void SystemClock_Config(void);

typedef struct {
    I2C_HandleTypeDef* i2c_bus;
    uint16_t i2c_address;
} mcp9808_bus_user_t;

mcp9808_err_t mcp9808_bus_initialize(void* user)
{
    mcp9808_bus_user_t* bus_user = user;

    return HAL_I2C_IsDeviceReady(bus_user->i2c_bus,
                                 bus_user->i2c_address,
                                 10,
                                 100) == HAL_OK
               ? MCP9808_ERR_OK
               : MCP9808_ERR_FAIL;
}

mcp9808_err_t mcp9808_bus_write_data(void* user,
                                     uint8_t address,
                                     uint8_t const* data,
                                     size_t data_size)
{
    mcp9808_bus_user_t* bus_user = user;

    HAL_I2C_Mem_Write(bus_user->i2c_bus,
                      bus_user->i2c_address,
                      address,
                      I2C_MEMADD_SIZE_8BIT,
                      data,
                      data_size,
                      100);

    return MCP9808_ERR_OK;
}

mcp9808_err_t mcp9808_bus_read_data(void* user,
                                    uint8_t address,
                                    uint8_t* data,
                                    size_t data_size)
{
    mcp9808_bus_user_t* bus_user = user;

    HAL_I2C_Mem_Read(bus_user->i2c_bus,
                     bus_user->i2c_address,
                     address,
                     I2C_MEMADD_SIZE_8BIT,
                     data,
                     data_size,
                     100);

    return MCP9808_ERR_OK;
}

static mcp9808_err_t mcp9808_initialize_chip(mcp9808_t const* mcp9808)
{
    mcp9808_err_t err = MCP9808_ERR_OK;

    mcp9808_manufacturer_id_reg_t man_id = {0};
    mcp9808_device_id_reg_t dev_id = {0};

    err |= mcp9808_get_manufacturer_id_reg(mcp9808, &man_id);
    err |= mcp9808_get_device_id_reg(mcp9808, &dev_id);

    if (err != MCP9808_ERR_OK)
        return err;

    if (man_id.manufacturer_id != 0x0054 || (dev_id.device_id & 0xFF) != 0x04) {
        return MCP9808_ERR_FAIL;
    }

    mcp9808_config_reg_t cfg = {0};
    err |= mcp9808_get_config_reg(mcp9808, &cfg);

    cfg.t_hyst = 0;
    cfg.shdn = 0;
    cfg.crit_lock = 0;
    cfg.win_lock = 0;
    cfg.int_clear = 0;
    cfg.alert_stat = 0;
    cfg.alert_cnt = 0;
    cfg.alert_sel = 0;
    cfg.alert_pol = 0;
    cfg.aler_mod = 0;

    err |= mcp9808_set_config_reg(mcp9808, &cfg);

    mcp9808_resolution_reg_t res = {.resolution = 0x03};
    err |= mcp9808_set_resolution_reg(mcp9808, &res);

    mcp9808_t_upper_reg_t upper = {.t_upper = 30 << 4};
    mcp9808_t_lower_reg_t lower = {.t_lower = 20 << 4};
    mcp9808_t_crit_reg_t crit = {.t_crit = 40 << 4};

    err |= mcp9808_set_t_upper_reg(mcp9808, &upper);
    err |= mcp9808_set_t_lower_reg(mcp9808, &lower);
    err |= mcp9808_set_t_crit_reg(mcp9808, &crit);

    return err;
}

float32_t mcp9808_resolution_to_scale(mcp9808_resolution_t);

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_USART2_UART_Init();
    MX_I2C1_Init();

    mcp9808_bus_user_t mcp9808_user = {
        .i2c_bus = &hi2c1,
        .i2c_address = MCP9808_SLAVE_ADDRESS_A2L_A1L_A0L << 1};

    mcp9808_t mcp9808;
    mcp9808_config_t mcp9808_config = {.scale =
                                           mcp9808_resolution_to_scale(0x03)};
    mcp9808_interface_t mcp9808_interface = {
        .bus_user = &mcp9808_user,
        .bus_initialize = mcp9808_bus_initialize,
        .bus_read_data = mcp9808_bus_read_data,
        .bus_write_data = mcp9808_bus_write_data};

    if (mcp9808_initialize(&mcp9808, &mcp9808_config, &mcp9808_interface) !=
        MCP9808_ERR_OK) {
        printf("Error!\n\r");
    }

    if (mcp9808_initialize_chip(&mcp9808) != MCP9808_ERR_OK) {
        printf("Error!\n\r");
    }

    float scaled;
    while (1) {
        mcp9808_get_temp_data_scaled(&mcp9808, &scaled);
        printf("scaled: %f\n\r", scaled);
    }
}