#include <QObject>
#include "modbus_utils.h"
#include "easylogging++.h"
#include <errno.h>

modbus_t* modbus_create(const char *device, int baud, char parity, int data_bit, int stop_bit, int slave) {
    modbus_t *ctx = NULL;
    do {
        ctx = modbus_new_rtu(device, baud, parity, data_bit, stop_bit);
        if (ctx == NULL) {
            LINFO << "modbus_new_rtu failed: " << modbus_strerror(errno);
            break;
        }

        if (modbus_set_slave(ctx, slave) == -1) {
            LINFO << "modbus_set_slave failed: " << modbus_strerror(errno);
            modbus_destroy(ctx);
            break;
        }

        if (modbus_connect(ctx) == -1) {
            LINFO << "modbus_connect failed: " << modbus_strerror(errno);
            modbus_destroy(ctx);
            break;
        }
    } while (false);
    return ctx;
}

void modbus_destroy(modbus_t* &ctx) {
    if (ctx != NULL) {
        modbus_close(ctx);
        modbus_free(ctx);
        ctx = NULL;
    }
}
