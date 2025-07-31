#include "canmgr.h"

#include <QDebug>
#include <QMessageBox>

#ifdef __cplusplus
extern "C" {
#endif

#include "canfestival.h"

#ifdef __cplusplus
};
#endif

#include "canopenexecutor.h"

#define SDO_CMD(ccs, t, n, e, s) \
    ((ccs << 5) | (t << 4) |  (n << 2) | (e << 1) | s)
#define SDO_INITIATE_CMD(ccs, n) \
    SDO_CMD(ccs, 0, n, 1, 1)

CanMgr::CanMgr(QObject *parent) : QThread(parent)
{
    m_Config.devType = VCI_USBCAN2;
    m_Config.devIndex = 0; //设备索引
    m_Config.devCom = 0; //通道索引

    m_IsOpen = false;
}

CanMgr::~CanMgr()
{
    close();
    terminate();
    wait();
    deleteLater();
}

QString CanMgr::transformData(VCI_CAN_OBJ obj, bool isRecv)
{
    QString data;

    data.clear();
    if (!isRecv) {
        data += "Received Node: " + QString::number(obj.ID, 16) + " Data: ";
    } else {
        data += "Send Node: " + QString::number(obj.ID, 16) + " Data: ";
    }

    for (int i = 0; i < 8; i++) {
        if (0x10 > obj.Data[i])
            data += QString("0x0%1 ").arg(obj.Data[i], 0, 16);
        else {
            data += QString("0x%1 ").arg(obj.Data[i], 0, 16);
        }
    }

    return data;
}

CanMgr *CanMgr::instance()
{
    static CanMgr can;

    return &can;
}

bool CanMgr::open(int baud)
{
    if (m_IsOpen)
        return m_IsOpen;

    //打开设备,只需一次
    if (STATUS_OK != VCI_OpenDevice(m_Config.devType, m_Config.devIndex, 0)) {
        qDebug() << "VCI_OpenDevice ERR!";
        return m_IsOpen;
    }

    VCI_INIT_CONFIG initConfig;
    initConfig.AccMask = 0xFFFFFFFF;
    initConfig.Mode = 0;
    initConfig.Timing0 = baud;  //500kbps
    initConfig.Timing1 = 0x14;
    //初始化设备
    if (STATUS_OK != VCI_InitCAN(m_Config.devType, m_Config.devIndex, m_Config.devCom, &initConfig)) {
        qDebug() << "VCI_InitCAN ERR!";
        return m_IsOpen;
    }

    //开始采集
    if (STATUS_OK != VCI_StartCAN(m_Config.devType, m_Config.devIndex, m_Config.devCom)) {
        qDebug() << "VCI_StartCAN ERR!";
        return m_IsOpen;
    }

    QMessageBox::information(nullptr, "成功", "打开Can成功", QMessageBox::Ok);

    m_IsOpen = true;
    qDebug() << "Open CAN device success!";

    return m_IsOpen;
}

bool CanMgr::isOpen() const
{
    return m_IsOpen;
}

void CanMgr::close()
{
    if (STATUS_ERR != VCI_CloseDevice(m_Config.devType, m_Config.devCom)) {
        m_IsOpen = false;
        qDebug() << "Close CAN device success!";
    } else {
        qDebug() << "Close CAN device failed!";
    }
}

void CanMgr::resetCan()
{
    if (STATUS_OK != VCI_ResetCAN(m_Config.devType, m_Config.devIndex, m_Config.devCom)) {
        qDebug() << "resetCan Failed!";
    }
}

void CanMgr::responSDO(const VCI_CAN_OBJ &can_obj)
{
    Message co_msg;
    co_msg.cob_id = can_obj.ID;
    co_msg.len = can_obj.DataLen;
    co_msg.rtr = can_obj.RemoteFlag;
    memcpy(co_msg.data, can_obj.Data, can_obj.DataLen);
    EnterMutex();
    canDispatch(CanopenExecutor::coData(), &co_msg);
    LeaveMutex();
}

void CanMgr::readBoardInfo()
{
    VCI_BOARD_INFO info;

    if (STATUS_OK != VCI_ReadBoardInfo(m_Config.devType, m_Config.devIndex, &info)) {
        qDebug() << "ReadBoardInfo Failed!";
    }
}

void CanMgr::clearBuffer()
{
    if (STATUS_OK != VCI_ClearBuffer(m_Config.devType, m_Config.devIndex, m_Config.devCom)) {
        qDebug() << "clearBuffer Failed!";
    }
}

int CanMgr::writeSdoData(uint8_t nodeid, uint16_t index, uint8_t sub_index, QByteArray data, size_t datalen)
{
    VCI_CAN_OBJ can_obj;

    if (datalen > 4)
        return -1;

    can_obj.ID = 0x600 + nodeid;
    //发送类型
    can_obj.SendType = 0;
    //数据类型
    can_obj.RemoteFlag = 0;
    //是否扩展帧
    can_obj.ExternFlag = 0;
    //数据长度
    can_obj.DataLen = 0x8;

    //数据内容
    can_obj.Data[0] = SDO_INITIATE_CMD(1, (4 - datalen));
    can_obj.Data[1] = index & 0xff;
    can_obj.Data[2] = index >> 8;
    can_obj.Data[3] = sub_index;

    for (uint8_t i = 0; i < 4; i++) {
        uint8_t d;
        if (i < datalen) {
            d = data[i];
        } else {
            d = 0;
        }
        can_obj.Data[i + 4] = d;
    }

    return VCI_Transmit(m_Config.devType, m_Config.devIndex, m_Config.devCom, &can_obj, 1);
}

int CanMgr::writeSdoData(uint8_t nodeid, QByteArray data)
{
    VCI_CAN_OBJ can_obj;

    if (data.size() !=  8)
        return -1;

    can_obj.ID = 0x600 + nodeid;
    //发送类型
    can_obj.SendType = 0;
    //数据类型
    can_obj.RemoteFlag = 0;
    //是否扩展帧
    can_obj.ExternFlag = 0;
    //数据长度
    can_obj.DataLen = 0x8;

    for (int i = 0; i < data.size(); i++) {
        can_obj.Data[i] = data[i];
    }

    Q_EMIT sigRecvData(transformData(can_obj, true));

    return VCI_Transmit(m_Config.devType, m_Config.devIndex, m_Config.devCom, &can_obj, 1);
}

void CanMgr::run()
{
    clearBuffer();
    while (m_IsOpen) {
        if (VCI_GetReceiveNum(m_Config.devType, m_Config.devIndex, m_Config.devCom) > 0) {
            VCI_CAN_OBJ canObj[100];
            ULONG recvCount = VCI_Receive(m_Config.devType, m_Config.devIndex, m_Config.devCom, canObj, 100, 20);
            //读取失败
            if (0xFFFFFFFF == recvCount) {
                VCI_ERR_INFO errInfo;
                VCI_ReadErrInfo(m_Config.devType, m_Config.devIndex, m_Config.devCom, &errInfo);
            } else {
                for (ULONG var = 0; var < recvCount; ++var) {
                    responSDO(canObj[var]);
                    qDebug() <<"\033[0;32;32m"<< transformData(canObj[var], false).toStdString().data();
                    //Q_EMIT sigRecvData(transformData(canObj[var], false));
                }
            }
        } else {
            msleep(1);
        }
    }
}
