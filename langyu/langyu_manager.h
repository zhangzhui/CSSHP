#ifndef _CSSHP_LANGYU_MANAGER_H__
#define _CSSHP_LANGYU_MANAGER_H__
#include <QObject>
#include <stdint.h>
class LangyuManager : public QObject {
    Q_OBJECT
private:
    LangyuManager();
    ~LangyuManager();

public:
    static LangyuManager* Instance();
    void Init();

signals:
    void ReadACDCResult(int port, uint16_t result);
    void ReadDigitalResult(int port, uint16_t result);

private slots:
    void OnReadACDC(const int port); //based on 1
    void OnReadDigital(const int port); //based on 1
    void OnWriteDigital(const int port, uint8_t val); //based on 1
};
#endif
