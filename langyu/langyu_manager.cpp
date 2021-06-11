#include "langyu_manager.h"
#include "common/modbus_utils.h"
#include "easylogging++.h"
#include <QThread>
#include <errno.h>

namespace langyu {
const int slave = 1;
const char *device = "/dev/ttyO5";
const int baud = 115200;
const char parity = 'N';
const int dataBit = 8;
const int stopBit = 1;
}

using namespace langyu;

LangyuManager::LangyuManager() {
    LINFO << QThread::currentThreadId() << "langyu manager constructor";
}

LangyuManager::~LangyuManager() {
    LINFO << QThread::currentThreadId() << "langyu manager ~destructor";
}

LangyuManager* LangyuManager::Instance() {
    static LangyuManager manager;
    return &manager;
}

void LangyuManager::Init() {
    LINFO << QThread::currentThreadId() << "langyu manager init";
    LINFO << QThread::currentThreadId() << "langyu manager init end";
}

void LangyuManager::OnReadACDC(const int port) {
    //based on 1
    if (port < 1 || port > 8) {
        LINFO << QThread::currentThreadId() << "[AC/DC]invalid port number: " << port;
        return;
    }

    int actualPort = port + 49;
    LINFO << QThread::currentThreadId() << "[AC/DC]actual port number: " << actualPort;
    modbus_t *ctx = modbus_create(device, baud, parity, dataBit, stopBit, slave);
    if (ctx != NULL) {
        uint16_t result;
        int rc = modbus_read_registers(ctx, actualPort, 1, &result);
        if (rc != -1) {
            emit ReadACDCResult(port, result);
        } else {
            LINFO << QThread::currentThreadId() << "[AC/DC]modbus_read_registers failed " << modbus_strerror(errno);
        }
    }
    modbus_destroy(ctx);
    LINFO << '[AC/DC]read completed';
}

void LangyuManager::OnReadDigital(const int port) {
    //based on 1
    if (port < 1 || port > 8) {
        LINFO << QThread::currentThreadId() << "[Digital Read]invalid port number: " << port;
        return;
    }

    int actualPort = port - 1;
    LINFO << QThread::currentThreadId() << "[Digital Read]actual port number: " << actualPort;
    modbus_t *ctx = modbus_create(device, baud, parity, dataBit, stopBit, slave);
    if (ctx != NULL) {
        uint8_t result;
        int rc = modbus_read_input_bits(ctx, actualPort, 1, &result);
        if (rc != -1) {
            emit ReadDigitalResult(port, result);
        } else {
            LINFO << QThread::currentThreadId() << "[Digital Read]modbus_read_input_bits failed " << modbus_strerror(errno);
        }
    }
    modbus_destroy(ctx);
    LINFO << '[Digital Read]read completed';
}

void LangyuManager::OnWriteDigital(const int port, uint8_t val) {
    //based on 1
    if (port < 1 || port > 8) {
        LINFO << QThread::currentThreadId() << "[Digital Write]invalid port number: " << port;
        return;
    }

    int actualPort = port - 1;
    LINFO << QThread::currentThreadId() << "[Digital Write]actual port number: " << actualPort;
    modbus_t *ctx = modbus_create(device, baud, parity, dataBit, stopBit, slave);
    if (ctx != NULL) {
        int rc = modbus_write_bits(ctx, actualPort, 1, &val);
        if (rc == -1) {
            LINFO << QThread::currentThreadId() << "[Digital Write]modbus_write_bits failed " << modbus_strerror(errno);
        }
    }
    modbus_destroy(ctx);
    LINFO << '[Digital Write]read completed';
}
