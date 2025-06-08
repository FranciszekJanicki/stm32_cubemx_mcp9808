#include "mcp9808.h"
#include <assert.h>
#include <string.h>

static mcp9808_err_t mcp9808_bus_init(mcp9808_t const* mcp9808)
{
    return mcp9808->interface.bus_init ? mcp9808->interface.bus_init(mcp9808->interface.bus_user)
                                       : MCP9808_ERR_NULL;
}

static mcp9808_err_t mcp9808_bus_deinit(mcp9808_t const* mcp9808)
{
    return mcp9808->interface.bus_deinit
               ? mcp9808->interface.bus_deinit(mcp9808->interface.bus_user)
               : MCP9808_ERR_NULL;
}

static mcp9808_err_t mcp9808_bus_write(mcp9808_t const* mcp9808,
                                       uint8_t address,
                                       uint8_t const* data,
                                       size_t data_size)
{
    return mcp9808->interface.bus_write
               ? mcp9808->interface.bus_write(mcp9808->interface.bus_user, address, data, data_size)
               : MCP9808_ERR_NULL;
}

static mcp9808_err_t mcp9808_bus_read(mcp9808_t const* mcp9808,
                                      uint8_t address,
                                      uint8_t* data,
                                      size_t data_size)
{
    return mcp9808->interface.bus_read
               ? mcp9808->interface.bus_read(mcp9808->interface.bus_user, address, data, data_size)
               : MCP9808_ERR_NULL;
}

mcp9808_err_t mcp9808_initialize(mcp9808_t* mcp9808,
                                 mcp9808_config_t const* config,
                                 mcp9808_interface_t const* interface)
{
    assert(mcp9808 && config && interface);

    memset(mcp9808, 0, sizeof(*mcp9808));
    memcpy(&mcp9808->config, config, sizeof(*config));
    memcpy(&mcp9808->interface, interface, sizeof(*interface));

    return mcp9808_bus_init(mcp9808);
}

mcp9808_err_t mcp9808_deinitialize(mcp9808_t* mcp9808)
{
    assert(mcp9808);

    mcp9808_err_t err = mcp9808_bus_deinit(mcp9808);

    memset(mcp9808, 0, sizeof(*mcp9808));

    return err;
}

mcp9808_err_t mcp9808_get_temp_data_scaled(mcp9808_t const* mcp9808, float32_t* scaled)
{
    assert(mcp9808 && scaled);

    int16_t raw = {};

    mcp9808_err_t err = mcp9808_get_temp_data_raw(mcp9808, &raw);

    *scaled = (float32_t)raw * mcp9808->config.scale;

    return err;
}

mcp9808_err_t mcp9808_get_temp_data_raw(mcp9808_t const* mcp9808, int16_t* raw)
{
    assert(mcp9808 && raw);

    mcp9808_t_ambient_reg_t reg = {};

    mcp9808_err_t err = mcp9808_get_t_ambient_reg(mcp9808, &reg);

    *raw = reg.t_ambient;

    // sign extend to 16 bits
    if (*raw & (1 << 12)) {
        *raw |= 0xE000;
    }

    return err;
}

mcp9808_err_t mcp9808_get_config_reg(mcp9808_t const* mcp9808, mcp9808_config_reg_t* reg)
{
    assert(mcp9808 && reg);

    uint8_t data[2] = {};

    mcp9808_err_t err = mcp9808_bus_read(mcp9808, MCP9808_REG_ADDR_CONFIG, data, sizeof(data));

    reg->t_hyst = (data[0] >> 1U) & 0x03U;
    reg->shdn = data[0] & 0x01U;
    reg->crit_lock = (data[1] >> 7U) & 0x01;
    reg->win_lock = (data[1] >> 6U) & 0x01;
    reg->int_clear = (data[1] >> 5U) & 0x01;
    reg->alert_stat = (data[1] >> 4U) & 0x01;
    reg->alert_cnt = (data[1] >> 3U) & 0x01;
    reg->alert_sel = (data[1] >> 2U) & 0x01;
    reg->alert_pol = (data[1] >> 1U) & 0x01;
    reg->aler_mod = data[1] & 0x01U;

    return err;
}

mcp9808_err_t mcp9808_set_config_reg(mcp9808_t const* mcp9808, mcp9808_config_reg_t const* reg)
{
    assert(mcp9808 && reg);

    uint8_t data[2] = {};

    mcp9808_err_t err = mcp9808_bus_read(mcp9808, MCP9808_REG_ADDR_CONFIG, data, sizeof(data));

    data[0] &= ~((0x03U << 1U) | 0x01U);
    data[1] &= ~((0x01U << 7U) | (0x01U << 6U) | (0x01U << 5U) | (0x01U << 4U) | (0x01U << 3U) |
                 (0x01U << 2U) | (0x01U << 1U) | 0x01U);

    data[0] |= (reg->t_hyst & 0x03U) << 1U;
    data[0] |= reg->shdn & 0x01U;
    data[1] |= (reg->crit_lock & 0x01U) << 7U;
    data[1] |= (reg->win_lock & 0x01U) << 6U;
    data[1] |= (reg->int_clear & 0x01U) << 5U;
    data[1] |= (reg->alert_stat & 0x01U) << 4U;
    data[1] |= (reg->alert_cnt & 0x01U) << 3U;
    data[1] |= (reg->alert_sel & 0x01U) << 2U;
    data[1] |= (reg->alert_pol & 0x01U) << 1U;
    data[1] |= reg->aler_mod & 0x01U;

    err |= mcp9808_bus_write(mcp9808, MCP9808_REG_ADDR_CONFIG, data, sizeof(data));

    return err;
}

mcp9808_err_t mcp9808_get_t_upper_reg(mcp9808_t const* mcp9808, mcp9808_t_upper_reg_t* reg)
{
    assert(mcp9808 && reg);

    uint8_t data[2] = {};

    mcp9808_err_t err = mcp9808_bus_read(mcp9808, MCP9808_REG_ADDR_T_UPPER, data, sizeof(data));

    reg->t_upper = (int16_t)(((data[0] & 0x1F) << 6) | ((data[1] >> 2) & 0x3F));

    return err;
}

mcp9808_err_t mcp9808_set_t_upper_reg(mcp9808_t const* mcp9808, mcp9808_t_upper_reg_t const* reg)
{
    assert(mcp9808 && reg);

    uint8_t data[2] = {};

    mcp9808_err_t err = mcp9808_bus_read(mcp9808, MCP9808_REG_ADDR_T_UPPER, data, sizeof(data));

    data[0] |= (uint8_t)((reg->t_upper >> 6) & 0x1F);
    data[1] |= (uint8_t)((reg->t_upper & 0x3F) << 2);

    err |= mcp9808_bus_write(mcp9808, MCP9808_REG_ADDR_T_UPPER, data, sizeof(data));

    return err;
}

mcp9808_err_t mcp9808_get_t_lower_reg(mcp9808_t const* mcp9808, mcp9808_t_lower_reg_t* reg)
{
    assert(mcp9808 && reg);

    uint8_t data[2] = {};

    mcp9808_err_t err = mcp9808_bus_read(mcp9808, MCP9808_REG_ADDR_T_LOWER, data, sizeof(data));

    reg->t_lower = (int16_t)(((data[0] & 0x1F) << 6) | ((data[1] >> 2) & 0x3F));

    return err;
}

mcp9808_err_t mcp9808_set_t_lower_reg(mcp9808_t const* mcp9808, mcp9808_t_lower_reg_t const* reg)
{
    assert(mcp9808 && reg);

    uint8_t data[2] = {};

    mcp9808_err_t err = mcp9808_bus_read(mcp9808, MCP9808_REG_ADDR_T_LOWER, data, sizeof(data));

    data[0] |= (uint8_t)((reg->t_lower >> 6) & 0x1F);
    data[1] |= (uint8_t)((reg->t_lower & 0x3F) << 2);

    err |= mcp9808_bus_write(mcp9808, MCP9808_REG_ADDR_T_LOWER, data, sizeof(data));

    return err;
}

mcp9808_err_t mcp9808_get_t_crit_reg(mcp9808_t const* mcp9808, mcp9808_t_crit_reg_t* reg)
{
    assert(mcp9808 && reg);

    uint8_t data[2] = {};

    mcp9808_err_t err = mcp9808_bus_read(mcp9808, MCP9808_REG_ADDR_T_CRIT, data, sizeof(data));

    reg->t_crit = (int16_t)(((data[0] & 0x1F) << 6) | ((data[1] >> 2) & 0x3F));

    return err;
}

mcp9808_err_t mcp9808_set_t_crit_reg(mcp9808_t const* mcp9808, mcp9808_t_crit_reg_t const* reg)
{
    assert(mcp9808 && reg);

    uint8_t data[2] = {};

    mcp9808_err_t err = mcp9808_bus_read(mcp9808, MCP9808_REG_ADDR_T_CRIT, data, sizeof(data));

    data[0] |= (uint8_t)((reg->t_crit >> 6) & 0x1F);
    data[1] |= (uint8_t)((reg->t_crit & 0x3F) << 2);

    err |= mcp9808_bus_write(mcp9808, MCP9808_REG_ADDR_T_CRIT, data, sizeof(data));

    return err;
}

mcp9808_err_t mcp9808_get_t_ambient_reg(mcp9808_t const* mcp9808, mcp9808_t_ambient_reg_t* reg)
{
    assert(mcp9808 && reg);

    uint8_t data[2] = {};

    mcp9808_err_t err = mcp9808_bus_read(mcp9808, MCP9808_REG_ADDR_T_AMBIENT, data, sizeof(data));

    reg->t_crit = (data[0] >> 7U) & 0x01U;
    reg->t_upper = (data[0] >> 6U) & 0x01U;
    reg->t_lower = (data[0] >> 5U) & 0x01U;
    reg->t_ambient = (int16_t)(((data[0] & 0x1F) << 8) | (data[1] & 0xFF));

    return err;
}

mcp9808_err_t mcp9808_get_manufacturer_id_reg(mcp9808_t const* mcp9808,
                                              mcp9808_manufacturer_id_reg_t* reg)
{
    assert(mcp9808 && reg);

    uint8_t data[2] = {};

    mcp9808_err_t err =
        mcp9808_bus_read(mcp9808, MCP9808_REG_ADDR_MANUFACTURER_ID, data, sizeof(data));

    reg->manufacturer_id = (uint16_t)((data[0] & 0xFFU << 8U) | (data[1] & 0xFFU));

    return err;
}

mcp9808_err_t mcp9808_get_device_id_reg(mcp9808_t const* mcp9808, mcp9808_device_id_reg_t* reg)
{
    assert(mcp9808 && reg);

    uint8_t data[2] = {};

    mcp9808_err_t err = mcp9808_bus_read(mcp9808, MCP9808_REG_ADDR_DEVICE_ID, data, sizeof(data));

    reg->device_id = data[0] & 0xFFU;
    reg->device_revision = data[1] & 0xFFU;

    return err;
}

mcp9808_err_t mcp9808_get_resolution_reg(mcp9808_t const* mcp9808, mcp9808_resolution_reg_t* reg)
{
    assert(mcp9808 && reg);

    uint8_t data = {};

    mcp9808_err_t err = mcp9808_bus_read(mcp9808, MCP9808_REG_ADDR_RESOLUTION, &data, sizeof(data));

    reg->resolution = data & 0x03U;

    return err;
}

mcp9808_err_t mcp9808_set_resolution_reg(mcp9808_t const* mcp9808,
                                         mcp9808_resolution_reg_t const* reg)
{
    assert(mcp9808 && reg);

    uint8_t data = {};

    mcp9808_err_t err = mcp9808_bus_read(mcp9808, MCP9808_REG_ADDR_RESOLUTION, &data, sizeof(data));

    data |= reg->resolution & 0x03U;

    err |= mcp9808_bus_write(mcp9808, MCP9808_REG_ADDR_RESOLUTION, &data, sizeof(data));

    return err;
}
