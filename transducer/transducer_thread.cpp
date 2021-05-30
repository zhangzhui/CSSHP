#include "transducer_thread.h"
#include "transducer_manager.h"
#include "easylogging++.h"

TransducerThread::TransducerThread() {
    LINFO << QThread::currentThreadId() << "transducer thread constructor";
}

TransducerThread::~TransducerThread() {
    LINFO << QThread::currentThreadId() << "transducer thread ~destructor";
}

void TransducerThread::run() {
    LINFO << QThread::currentThreadId() << "transducer thread start";
    LINFO << QThread::currentThreadId() << "get transducer ON/OFF";
    TransducerManager::Instance()->QueryOnOff();
    QThread::exec();
}
