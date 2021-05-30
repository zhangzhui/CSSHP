#ifndef _CSSHP_TRANSDUCER_MANAGER_H__
#define _CSSHP_TRANSDUCER_MANAGER_H__
#include <QObject>
class TransducerManager : public QObject {
    Q_OBJECT
private:
    TransducerManager();
    ~TransducerManager();

public:
    static TransducerManager* Instance();
    void Init();

public:
    void QueryOnOff();

signals:
    void TransducerOnOff(bool off);

private slots:
    void OnStartTransducer();
    void OnStopTransducer();
    void OnIncreaseFreq();
    void OnDecreaseFreq();
};
#endif
