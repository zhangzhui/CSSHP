#include "transducer_manager.h"
#include "common/modbus_utils.h"
#include "easylogging++.h"
#include <QThread>

namespace transducer {
const int slave = 2;
const char *device = "/dev/ttyO2";
const int baud = 9600;
const char parity = 'N';
const int dataBit = 8;
const int stopBit = 1;
const int maxFreq = 10000; // 100%
int freq = 3000; // 50Hz * 0.3 = 15Hz 3000 = 0.3 * 10000
int stride = 200; // 1Hz
}

using namespace transducer;

TransducerManager::TransducerManager() {
    LINFO << QThread::currentThreadId() << "transducer constructor";
}

TransducerManager::~TransducerManager() {
    LINFO << QThread::currentThreadId() << "transducer ~destructor";
}

TransducerManager* TransducerManager::Instance() {
    static TransducerManager manager;
    return &manager;
}

void TransducerManager::Init() {
    LINFO << QThread::currentThreadId() << "transducer init";
    LINFO << QThread::currentThreadId() << "transducer init end";
}

void TransducerManager::QueryOnOff() {
    LINFO << QThread::currentThreadId() << "query transducer state.";
    bool off = false;
    modbus_t *ctx = modbus_create(device, baud, parity, dataBit, stopBit, slave);
    if (ctx != NULL) {
        int addr = 0x30 << 8 | 0x00;
        uint16_t value = 0;
        modbus_read_registers(ctx, addr, 1, &value);
        LINFO << QThread::currentThreadId() << "transducer state is: " << value;
        off = value == 0x03;
    }
    modbus_destroy(ctx);
    LINFO << QThread::currentThreadId() << "query transducer state end.";
    emit TransducerOnOff(off);
}

void TransducerManager::OnStartTransducer() {
    LINFO << QThread::currentThreadId() << "start transducer.";
    modbus_t *ctx = modbus_create(device, baud, parity, dataBit, stopBit, slave);
    if (ctx != NULL) {
        {
            LINFO << QThread::currentThreadId() << "set transducer freq to 50Hz";
            int addr = 0x10 << 8 | 0x00;
            uint16_t value = maxFreq;
            modbus_write_register(ctx, addr, value);
        }
        {
            LINFO << QThread::currentThreadId() << "start transducer";
            int addr = 0x20 << 8 | 0x00;
            uint16_t value = 0x00 << 8 | 0x01;
            modbus_write_register(ctx, addr, value);
        }
        {
            LINFO << QThread::currentThreadId() << "set var(freq) = 3000";
            freq = 3000;
            LINFO << QThread::currentThreadId() << "set transducer freq to 15Hz";
            int addr = 0x10 << 8 | 0x00;
            uint16_t value = freq;
            modbus_write_register(ctx, addr, value);
        }
    }
    modbus_destroy(ctx);
    LINFO << QThread::currentThreadId() << "start transducer end.";
}

void TransducerManager::OnStopTransducer() {
    LINFO << QThread::currentThreadId() << "stop transducer.";
    modbus_t *ctx = modbus_create(device, baud, parity, dataBit, stopBit, slave);
    if (ctx != NULL) {
        {
            LINFO << QThread::currentThreadId() << "stop transducer";
            int addr = 0x20 << 8 | 0x00;
            uint16_t value = 0x00 << 8 | 0x06;
            modbus_write_register(ctx, addr, value);
        }
        {
            LINFO << QThread::currentThreadId() << "set transducer freq to 50Hz";
            int addr = 0x10 << 8 | 0x00;
            uint16_t value = maxFreq;
            modbus_write_register(ctx, addr, value);
            LINFO << QThread::currentThreadId() << "set var(freq) = 3000";
            freq = 3000;
        }
    }
    modbus_destroy(ctx);
    LINFO << QThread::currentThreadId() << "stop transducer end.";
}

void TransducerManager::OnIncreaseFreq() {
    if (freq >= 10000) {
        LINFO << QThread::currentThreadId() << "invalid freq: " << freq;
        LINFO << QThread::currentThreadId() << "set freq to 10000.";
        freq = maxFreq;
        return;
    }

    int from = freq;
    freq += stride;
    int to = freq;
    if (to > maxFreq) {
        to = maxFreq;
    }
    LINFO << QThread::currentThreadId() << "increase freq, from " << from << " to " << to;
    modbus_t *ctx = modbus_create(device, baud, parity, dataBit, stopBit, slave);
    if (ctx != NULL) {
        {
            int addr = 0x10 << 8 | 0x00;
            uint16_t value = to;
            modbus_write_register(ctx, addr, value);
        }
    }
    modbus_destroy(ctx);
    LINFO << QThread::currentThreadId() << "increase freq end";
}

void TransducerManager::OnDecreaseFreq() {
    if (freq <= 0) {
        LINFO << QThread::currentThreadId() << "invalid freq: " << freq;
        LINFO << QThread::currentThreadId() << "clear freq to 0.";
        freq = 0;
        return;
    }

    int from = freq;
    freq -= stride;
    int to = freq;
    if (to < 0) {
        to = 0;
    }
    LINFO << QThread::currentThreadId() << "decrease freq, from " << from << " to " << to;
    modbus_t *ctx = modbus_create(device, baud, parity, dataBit, stopBit, slave);
    if (ctx != NULL) {
        {
            int addr = 0x10 << 8 | 0x00;
            uint16_t value = to;
            modbus_write_register(ctx, addr, value);
        }
    }
    modbus_destroy(ctx);
    LINFO << QThread::currentThreadId() << "decrease freq end";
}
