#ifndef CANMGR_H
#define CANMGR_H

#include <QThread>

#include "ControlCAN.h"

#define CAN_MGR (CanMgr::instance())

class CanMgr : public QThread
{
    Q_OBJECT

    typedef  struct CanRunConfig {
        DWORD devType;
        DWORD devIndex;
        DWORD devCom;

        CanRunConfig () {
            devType = -1;
            devIndex = -1;
            devCom = -1;
        }
    }RunConfig;

public:
    static CanMgr* instance();

    //bool open(int cannel, int com, int baud);
    bool open(int baud);
    bool isOpen() const;
    void close();

    void readBoardInfo();

    void clearBuffer();

    int writeSdoData(uint8_t nodeid, uint16_t index, uint8_t sub_index, QByteArray data, size_t datalen);
    int writeSdoData(uint8_t nodeid, QByteArray data);

    void readSdoData();

    void writePdoData();
    void readPdoData();

    void resetCan();

    void responSDO(const VCI_CAN_OBJ& can_obj);

protected:
    void run() override;

private:
    explicit CanMgr(QObject *parent = nullptr);
    ~CanMgr();

    QString transformData(VCI_CAN_OBJ obj, bool isRecv);

signals:
    void sigRecvData(QString);
    void sigSendData(QString);

private:
    RunConfig m_Config;

    enum{
       BAUD_1000K = 0,
       BAUD_500K,
       BAUD_100K,
    };

    bool m_IsOpen;
};

#endif // CANMGR_H
