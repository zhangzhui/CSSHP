#ifndef _CSSHP_LANGYU_THREAD_H__
#define _CSSHP_LANGYU_THREAD_H__
#include <QThread>
class LangyuThread: public QThread {
public:
    LangyuThread();
    ~LangyuThread();
protected:
    virtual void run();
};
#endif
