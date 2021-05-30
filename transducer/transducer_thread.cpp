#include "transducer_thread.h"
#include "transducer_manager.h"
#include "easylogging++.h"

TransducerThread::TransducerThread() {
    LINFO << "transducer thread constructor";
}

TransducerThread::~TransducerThread() {
    LINFO << "transducer thread ~destructor";
}

void TransducerThread::run() {
    LINFO << "transducer thread start";
    LINFO << "get transducer ON/OFF";
    TransducerManager::Instance()->QueryOnOff();
    QThread::exec();
}
