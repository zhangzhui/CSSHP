#ifndef _CSSHP_COMMON_MODBUS_UTILS_H__
#define _CSSHP_COMMON_MODBUS_UTILS_H__
#include "modbus/modbus.h"
modbus_t* modbus_create(const char *device, int baud, char parity, int data_bit, int stop_bit, int slave);
void modbus_destroy(modbus_t* &ctx);
#endif
