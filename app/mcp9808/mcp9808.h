#ifndef MCP9808_MCP9808_H
#define MCP9808_MCP9808_H

#include "mcp9808_config.h"
#include "mcp9808_registers.h"

typedef struct {
    mcp9808_config_t config;
    mcp9808_interface_t interface;
} mcp9808_t;

mcp9808_err_t
mcp9808_initialize(mcp9808_t* mcp9808, mcp9808_config_t const* config, mcp9808_interface_t const* interface);
mcp9808_err_t mcp9808_deinitialize(mcp9808_t* mcp9808);

mcp9808_err_t mcp9808_get_temp_data_scaled(mcp9808_t const* mcp9808, float32_t* scaled);

mcp9808_err_t mcp9808_get_temp_data_raw(mcp9808_t const* mcp9808, int16_t* raw);

mcp9808_err_t mcp9808_get_config_reg(mcp9808_t const* mcp9808, mcp9808_config_reg_t* reg);
mcp9808_err_t mcp9808_set_config_reg(mcp9808_t const* mcp9808, mcp9808_config_reg_t const* reg);

mcp9808_err_t mcp9808_get_t_upper_reg(mcp9808_t const* mcp9808, mcp9808_t_upper_reg_t* reg);
mcp9808_err_t mcp9808_set_t_upper_reg(mcp9808_t const* mcp9808, mcp9808_t_upper_reg_t const* reg);

mcp9808_err_t mcp9808_get_t_lower_reg(mcp9808_t const* mcp9808, mcp9808_t_lower_reg_t* reg);
mcp9808_err_t mcp9808_set_t_lower_reg(mcp9808_t const* mcp9808, mcp9808_t_lower_reg_t const* reg);

mcp9808_err_t mcp9808_get_t_crit_reg(mcp9808_t const* mcp9808, mcp9808_t_crit_reg_t* reg);
mcp9808_err_t mcp9808_set_t_crit_reg(mcp9808_t const* mcp9808, mcp9808_t_crit_reg_t const* reg);

mcp9808_err_t mcp9808_get_t_ambient_reg(mcp9808_t const* mcp9808, mcp9808_t_ambient_reg_t* reg);

mcp9808_err_t mcp9808_get_manufacturer_id_reg(mcp9808_t const* mcp9808, mcp9808_manufacturer_id_reg_t* reg);

mcp9808_err_t mcp9808_get_device_id_reg(mcp9808_t const* mcp9808, mcp9808_device_id_reg_t* reg);

mcp9808_err_t mcp9808_get_resolution_reg(mcp9808_t const* mcp9808, mcp9808_resolution_reg_t* reg);
mcp9808_err_t mcp9808_set_resolution_reg(mcp9808_t const* mcp9808, mcp9808_resolution_reg_t const* reg);

#endif // MCP9808_MCP9808_H