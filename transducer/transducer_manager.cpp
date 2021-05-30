#include "transducer_manager.h"
#include "common/modbus_utils.h"
#include "easylogging++.h"

const int slave = 2;
const char *device = "/dev/ttyO2";
const int baud = 9600;
const char parity = 'N';
const int dataBit = 8;
const int stopBit = 1;
int maxFreq = 10000; // 100%
int freq = 3000; // 50Hz * 0.3 = 15Hz 3000 = 0.3 * 10000
int stride = 200; // 1Hz

TransducerManager::TransducerManager() {
    LINFO << "transducer constructor";
}

TransducerManager::~TransducerManager() {
    LINFO << "transducer ~destructor";
}

TransducerManager* TransducerManager::Instance() {
    static TransducerManager manager;
    return &manager;
}

void TransducerManager::Init() {
    LINFO << "transducer init";
    LINFO << "transducer init end";
}

void TransducerManager::QueryOnOff() {
    LINFO << "query transducer state.";
    bool off = false;
    modbus_t *ctx = modbus_create(device, baud, parity, dataBit, stopBit, slave);
    if (ctx != NULL) {
        int addr = 0x30 << 8 | 0x00;
        uint16_t value = 0;
        modbus_read_registers(ctx, addr, 1, &value);
        LINFO << "transducer state is: " << value;
        off = value == 0x03;
    }
    modbus_destroy(ctx);
    LINFO << "query transducer state end.";
    emit TransducerOnOff(off);
}

void TransducerManager::OnStartTransducer() {
    LINFO << "start transducer.";
    modbus_t *ctx = modbus_create(device, baud, parity, dataBit, stopBit, slave);
    if (ctx != NULL) {
        {
            LINFO << "set transducer freq to 50Hz";
            int addr = 0x10 << 8 | 0x00;
            uint16_t value = maxFreq;
            modbus_write_register(ctx, addr, value);
        }
        {
            LINFO << "start transducer";
            int addr = 0x20 << 8 | 0x00;
            uint16_t value = 0x00 << 8 | 0x01;
            modbus_write_register(ctx, addr, value);
        }
        {
            LINFO << "set var(freq) = 3000";
            freq = 3000;
            LINFO << "set transducer freq to 15Hz";
            int addr = 0x10 << 8 | 0x00;
            uint16_t value = freq;
            modbus_write_register(ctx, addr, value);
        }
    }
    modbus_destroy(ctx);
    LINFO << "start transducer end.";
}

void TransducerManager::OnStopTransducer() {
    LINFO << "stop transducer.";
    modbus_t *ctx = modbus_create(device, baud, parity, dataBit, stopBit, slave);
    if (ctx != NULL) {
        {
            LINFO << "stop transducer";
            int addr = 0x20 << 8 | 0x00;
            uint16_t value = 0x00 << 8 | 0x06;
            modbus_write_register(ctx, addr, value);
        }
        {
            LINFO << "set transducer freq to 50Hz";
            int addr = 0x10 << 8 | 0x00;
            uint16_t value = maxFreq;
            modbus_write_register(ctx, addr, value);
            LINFO << "set var(freq) = 3000";
            freq = 3000;
        }
    }
    modbus_destroy(ctx);
    LINFO << "stop transducer end.";
}

void TransducerManager::OnIncreaseFreq() {
    if (freq >= 10000 || freq < 0) {
        LINFO << "invalid freq: " << freq;
        return;
    }

    int from = freq;
    freq += stride;
    int to = freq;
    LINFO << "increase freq, from " << from << " to " << to;
    modbus_t *ctx = modbus_create(device, baud, parity, dataBit, stopBit, slave);
    if (ctx != NULL) {
        {
            int addr = 0x10 << 8 | 0x00;
            uint16_t value = to;
            modbus_write_register(ctx, addr, value);
        }
    }
    modbus_destroy(ctx);
    LINFO << "increase freq end";
}

void TransducerManager::OnDecreaseFreq() {
    if (freq >= 10000 || freq < 0) {
        LINFO << "invalid freq: " << freq;
        return;
    }

    int from = freq;
    freq -= stride;
    int to = freq;
    LINFO << "decrease freq, from " << from << " to " << to;
    modbus_t *ctx = modbus_create(device, baud, parity, dataBit, stopBit, slave);
    if (ctx != NULL) {
        {
            int addr = 0x10 << 8 | 0x00;
            uint16_t value = to;
            modbus_write_register(ctx, addr, value);
        }
    }
    modbus_destroy(ctx);
    LINFO << "decrease freq end";
}
