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
    mcp9808_config_t mcp9808_config = {.scale = 0.5F};
    mcp9808_interface_t mcp9808_interface = {
        .bus_user = &mcp9808_user,
        .bus_initialize = mcp9808_bus_initialize,
        .bus_read_data = mcp9808_bus_read_data,
        .bus_write_data = mcp9808_bus_write_data};

    if (mcp9808_initialize(&mcp9808, &mcp9808_config, &mcp9808_interface) !=
        MCP9808_ERR_OK) {
        Error_Handler();
    }

    float scaled;
    while (1) {
        mcp9808_get_temp_data_scaled(&mcp9808, &scaled);
    }
}