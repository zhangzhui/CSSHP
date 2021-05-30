#ifndef _CSSHP_TRANSDUCER_THREAD_H__
#define _CSSHP_TRANSDUCER_THREAD_H__
#include <QThread>
class TransducerThread : public QThread {
public:
    TransducerThread();
    ~TransducerThread();
protected:
    virtual void run();
};
#endif
