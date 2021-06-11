#include "langyu_thread.h"
#include "easylogging++.h"

LangyuThread::LangyuThread() {
    LINFO << QThread::currentThreadId() << "langyu thread constructor";
}

LangyuThread::~LangyuThread() {
    LINFO << QThread::currentThreadId() << "langyu thread ~destructor";
}

void LangyuThread::run() {
    LINFO << QThread::currentThreadId() << "langyu thread start";
    QThread::exec();
}
