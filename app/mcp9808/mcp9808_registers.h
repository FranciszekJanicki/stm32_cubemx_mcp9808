#ifndef MCP9808_MCP9808_REGISTERS_H
#define MCP9808_MCP9808_REGISTERS_H

#include <stdint.h>

typedef struct {
    uint8_t t_hyst : 2;
    uint8_t shdn : 1;
    uint8_t crit_lock : 1;
    uint8_t win_lock : 1;
    uint8_t int_clear : 1;
    uint8_t alert_stat : 1;
    uint8_t alert_cnt : 1;
    uint8_t alert_sel : 1;
    uint8_t alert_pol : 1;
    uint8_t aler_mod : 1;
} mcp9808_config_reg_t;

typedef struct {
    int16_t t_upper : 11;
} mcp9808_t_upper_reg_t;

typedef struct {
    int16_t t_lower : 11;
} mcp9808_t_lower_reg_t;

typedef struct {
    int16_t t_crit : 11;
} mcp9808_t_crit_reg_t;

typedef struct {
    uint8_t t_crit : 1;
    uint8_t t_upper : 1;
    uint8_t t_lower : 1;
    int16_t t_ambient : 13;
} mcp9808_t_ambient_reg_t;

typedef struct {
    uint16_t manufacturer_id : 16;
} mcp9808_manufacturer_id_reg_t;

typedef struct {
    uint8_t device_id : 8;
    uint8_t device_revision : 8;
} mcp9808_device_id_reg_t;

typedef struct {
    uint8_t resolution : 2;
} mcp9808_resolution_reg_t;

#endif // MCP9808_MCP9808_REGISTERS_H