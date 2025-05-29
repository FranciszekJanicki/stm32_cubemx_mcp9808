#ifndef MCP9808_MCP9808_REGISTERS_H
#define MCP9808_MCP9808_REGISTERS_H

#include <stdint.h>

#define PACKED __attribute__((packed))

typedef struct {
    uint8_t : 4;
    uint8_t pointer : 4;
} PACKED mcp9808_register_pointer_reg_t;

typedef struct {
    uint8_t : 5;
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
} PACKED mcp9808_config_reg_t;

typedef struct {
    uint8_t : 3;
    int8_t t_upper_10to6 : 5;
    int8_t t_upper_5to0 : 6;
    uint8_t : 2;
} PACKED mcp9808_t_upper_reg_t;

typedef struct {
    uint8_t : 3;
    int8_t t_lower_10to6 : 5;
    int8_t t_lower_5to0 : 6;
    uint8_t : 2;
} PACKED mcp9808_t_lower_reg_t;

typedef struct {
    uint8_t : 3;
    int8_t t_crit_10to6 : 5;
    int8_t t_crit_5to0 : 6;
    uint8_t : 2;
} PACKED mcp9808_t_crit_reg_t;

typedef struct {
    uint8_t t_crit : 1;
    uint8_t t_upper : 1;
    uint8_t t_lower : 1;
    int8_t t_ambient_12to8 : 5;
    int8_t t_ambient_7to0 : 8;
} PACKED mcp9808_t_ambient_reg_t;

typedef struct {
    uint8_t manufacturer_id_15to8 : 8;
    uint8_t manufacturer_id_7to0 : 8;
} PACKED mcp9808_manufacturer_id_reg_t;

typedef struct {
    uint8_t device_id : 8;
    uint8_t device_revision : 8;
} PACKED mcp9808_device_id_reg_t;

typedef struct {
    uint8_t : 6;
    uint8_t resolution : 2;
} PACKED mcp9808_resolution_reg_t;

#undef PACKED

#endif // MCP9808_MCP9808_REGISTERS_H