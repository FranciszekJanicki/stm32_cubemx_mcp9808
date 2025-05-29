#ifndef MCP9808_MCP9808_CONFIG_H
#define MCP9808_MCP9808_CONFIG_H

#include "mcp9808_registers.h"
#include <stddef.h>
#include <stdint.h>

#define MCP9808_DEFAULT_DEVICE_ID 0x04
#define MCP9808_DEFAULT_MANUFACTURER_ID 0x0054
#define MCP9808_DEFAULT_DEVICE_REVISION 0x00

typedef float float32_t;

typedef enum {
    MCP9808_ERR_OK = 0,
    MCP9808_ERR_FAIL = 1 << 0,
    MCP9808_ERR_NULL = 1 << 1,
} mcp9808_err_t;

typedef enum {
    MCP9808_DEV_ADDR_A2L_A1L_A0L = 0b0011000,
    MCP9808_DEV_ADDR_A2L_A1L_A0H = 0b0011001,
    MCP9808_DEV_ADDR_A2L_A1H_A0H = 0b0011011,
    MCP9808_DEV_ADDR_A2H_A1H_A0H = 0b0011111,
    MCP9808_DEV_ADDR_A2H_A1H_A0L = 0b0011110,
    MCP9808_DEV_ADDR_A2H_A1L_A0L = 0b0011100,
    MCP9808_DEV_ADDR_A2L_A1H_A0L = 0b0011010,
    MCP9808_DEV_ADDR_A2H_A1L_A0H = 0b0011101,
} mcp9808_dev_addr_t;

typedef enum {
    MCP9808_REG_ADDR_REGISTER_POINTER = 0x00,
    MCP9808_REG_ADDR_CONFIG = 0x01,
    MCP9808_REG_ADDR_T_UPPER = 0x02,
    MCP9808_REG_ADDR_T_LOWER = 0x03,
    MCP9808_REG_ADDR_T_CRIT = 0x04,
    MCP9808_REG_ADDR_T_AMBIENT = 0x05,
    MCP9808_REG_ADDR_MANUFACTURER_ID = 0x06,
    MCP9808_REG_ADDR_DEVICE_ID = 0x07,
    MCP9808_REG_ADDR_RESOLUTION = 0x08,
} mcp9808_reg_addr_t;

typedef enum {
    MCP9808_LIMIT_HYST_0C0 = 0b00,
    MCP9808_LIMIT_HYST_1C5 = 0b01,
    MCP9808_LIMIT_HYST_3C0 = 0b10,
    MCP9808_LIMIT_HYST_6C0 = 0b11,
} mcp9808_limit_hyst_t;

typedef enum {
    MCP9808_MODE_CONTINUOUS = 0b00,
    MCP9808_MODE_SHUTDOWN = 0b01,
} mcp9808_mode_t;

typedef enum {
    MCP9808_CRIT_LOCK_UNLOCKED = 0b00,
    MCP9808_CRIT_LOCK_LOCKED = 0b01,
} mcp9808_crit_lock_t;

typedef enum WinLock {
    MCP9808_WIN_LOCK_UNLOCKED = 0b00,
    MCP9808_WIN_LOCK_LOCKED = 0b01,
} mcp9808_win_lock_t;

typedef enum {
    MCP9808_INT_CLEAR_NO_EFFECT = 0b00,
    MCP9808_INT_CLEAR_DATA_READY = 0b01,
} mcp9808_int_clear_t;

typedef enum {
    MCP9808_ALERT_STAT_DISABLE = 0b00,
    MCP9808_ALERT_STAT_ASSERT = 0b01,
} mcp9808_alert_stat_t;

typedef enum {
    MCP9808_ALERT_COUNT_ENABLED = 0b01,
    MCP9808_ALERT_COUNT_DISABLED = 0b00,
} mcp9808_alert_count_t;

typedef enum {
    MCP9808_ALERT_SELECT_ALL = 0b00,
    MCP9808_ALERT_SELECT_CRIT_ONLY = 0b01,
} mcp9808_alert_select_t;

typedef enum {
    MCP9808_ALERT_POLARITY_ACTIVE_LOW = 0b00,
    MCP9808_ALERT_POLARITY_ACTIVE_HIGH = 0b01,
} mcp9808_alert_polarity_t;

typedef enum {
    MCP9808_ALERT_MODE_COMPARATOR_OUTPUT = 0b00,
    MCP9808_ALERT_MODE_INTERRUPT_OUTPUT = 0b01,
} mcp9808_alert_mode_t;

typedef enum {
    MCP9808_RESOLUTION_0C5 = 0b00,
    MCP9808_RESOLUTION_0C25 = 0b01,
    MCP9808_RESOLUTION_0C125 = 0b10,
    MCP9808_RESOLUTION_0C0625 = 0b11,
} mcp9808_resolution_t;

typedef struct {
    float32_t scale;
} mcp9808_config_t;

typedef struct {
    void* bus_user;
    mcp9808_err_t (*bus_init)(void*);
    mcp9808_err_t (*bus_deinit)(void*);
    mcp9808_err_t (*bus_write)(void*, uint8_t, uint8_t const*, size_t);
    mcp9808_err_t (*bus_read)(void*, uint8_t, uint8_t*, size_t);
} mcp9808_interface_t;

inline float32_t mcp9808_resolution_to_scale(mcp9808_resolution_t resolution)
{
    switch (resolution) {
        case MCP9808_RESOLUTION_0C5:
            return 0.5F;
        case MCP9808_RESOLUTION_0C25:
            return 0.25F;
        case MCP9808_RESOLUTION_0C125:
            return 0.125F;
        case MCP9808_RESOLUTION_0C0625:
            return 0.0625F;
        default:
            return 0.0F;
    }
}

#endif // MCP9808_MCP9808_CONFIG_H