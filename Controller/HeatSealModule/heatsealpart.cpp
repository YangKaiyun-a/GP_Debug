#include "heatsealpart.h"

#include "Driver/commander.h"
#include <QThread>
#include <QDebug>

HeatSealPart::HeatSealPart(QObject *parent) : AbstractPart(parent)
{

}

HeatSealPart *HeatSealPart::instance()
{
    static HeatSealPart heatseal;
    return &heatseal;
}

void HeatSealPart::init()
{

}

int HeatSealPart::moduleReset()
{
    int result = -1;
    quint16 index = HEATSEAL_INDEX(MODULE_RESET);

    Commander c(DevicePart::NODE_HEATSEAL);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int HeatSealPart::startHeat()
{
    int result = -1;
    quint16 index = HEATSEAL_INDEX(HEAT_START);

    Commander c(DevicePart::NODE_HEATSEAL);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int HeatSealPart::stopHeat()
{
    int result = -1;
    quint16 index = HEATSEAL_INDEX(HEAT_STOP);

    Commander c(DevicePart::NODE_HEATSEAL);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int HeatSealPart::sealing()
{
    int result = -1;
    quint16 index = HEATSEAL_INDEX(SEALING);

    Commander c(DevicePart::NODE_HEATSEAL);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

void HeatSealPart::readTemp()
{
    if(m_bReadTemp)
    {
        return;
    }

    m_bReadTemp = true;

    quint16 index = HEATSEAL_INDEX(TEMP_CURRENT);
    Commander c(DevicePart::NODE_HEATSEAL);

    int temp1 = 0, temp2 = 0, temp3 = 0;
    QList<float> temps;
    QVector<Commander::SDOitem> items;
    items << Commander::SDOitem(index, 0x01, &temp1) \
          << Commander::SDOitem(index, 0x02, &temp2) \
          << Commander::SDOitem(index, 0x03, &temp3);

    while(m_bReadTemp)
    {
        QThread::msleep(300);
        temps.clear();
        c.readRemoteData(items);
        temps << DOUBLE_TO_OD_DATA(temp1) << DOUBLE_TO_OD_DATA(temp2)<<DOUBLE_TO_OD_DATA(temp3);
        Q_EMIT sigCurrentTemp(temps);

    }
}

int HeatSealPart::trayPullOut()
{
    int result = -1;
    quint16 index = HEATSEAL_INDEX(TRAY_OUT);

    Commander c(DevicePart::NODE_HEATSEAL);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int HeatSealPart::trayPushIn()
{
    int result = -1;
    quint16 index = HEATSEAL_INDEX(TRAY_IN);

    Commander c(DevicePart::NODE_HEATSEAL);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int HeatSealPart::tempUpload(int status, int frequency)
{
    int result = -1;
    quint16 index = HEATSEAL_INDEX(TEMP_UDLOAD);

    Commander c(DevicePart::NODE_HEATSEAL);
    QVector<Commander::SDOitem> items;

    items << Commander::SDOitem(index, 0x03, &status) \
          << Commander::SDOitem(index, 0x04, &frequency);

    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

void HeatSealPart::test(int count, bool is_pause)
{
    m_test = true;

    moduleReset();

    if(is_pause == 0)
    {
        test_count = 0;
    }

    while(m_test&&(test_count<count))
    {
        qDebug()<<123;
        Q_EMIT sigTestCurrentCount(++test_count);

        startHeat();
        trayPushIn();

        for(int i = 0; i < time1; i++)
        {
            QThread::sleep(1);
            if(m_test == false)
            {
                break;
            }
        }

        sealing();
        trayPullOut();
        stopHeat();

        for(int j = 0; j < time2; j++)
        {
            QThread::sleep(1);
            if(m_test == false)
            {
                break;
            }
        }
    }

    m_test = false;

}

int HeatSealPart::stop_test()
{
    m_test = false;
    stopHeat();
}

int HeatSealPart::getParameter(HeatSealPart::HeatSealParams &params)
{
    int result = -1;
    quint16 index = HEATSEAL_INDEX(PARAMETER);

    Commander c(DevicePart::NODE_HEATSEAL);
    QVector<Commander::SDOitem> items;

    int targetTemp = -1;
    int sealTime = -1;
    int zDownPosition = -1;
    int trayOutPosition = -1;
    int traySealPosition = -1;
    int zDownSpeed = -1;
    int trayOutSpeed = -1;
    int zResetSpeed = -1;
    int trayResetSpeed = -1;
    int tempCompensationOut = -1;
    int tempCompensationMid = -1;
    int tempCompensationIn = -1;

    items << Commander::SDOitem(index, 0x03, &targetTemp) \
          << Commander::SDOitem(index, 0x04, &sealTime) \
          << Commander::SDOitem(index, 0x05, &zDownPosition) \
          << Commander::SDOitem(index, 0x06, &trayOutPosition) \
          << Commander::SDOitem(index, 0x07, &traySealPosition) \
          << Commander::SDOitem(index, 0x08, &zDownSpeed) \
          << Commander::SDOitem(index, 0x09, &trayOutSpeed) \
          << Commander::SDOitem(index, 0x0A, &zResetSpeed) \
          << Commander::SDOitem(index, 0x0B, &trayResetSpeed) \
          << Commander::SDOitem(index, 0x0C, &tempCompensationOut) \
          << Commander::SDOitem(index, 0x0E, &tempCompensationMid) \
          << Commander::SDOitem(index, 0x0D, &tempCompensationIn);

    result = c.readRemoteData(items);

    if (!result) {
        params.targetTemp = DOUBLE_TO_OD_DATA(targetTemp);
        params.sealTime = DOUBLE_TO_OD_DATA(sealTime);
        params.zDownPosition = DOUBLE_TO_OD_DATA(zDownPosition);
        params.trayOutPosition = DOUBLE_TO_OD_DATA(trayOutPosition);
        params.traySealPosition = DOUBLE_TO_OD_DATA(traySealPosition);
        params.zDownSpeed = INT_TO_OD_DATA(zDownSpeed);
        params.trayOutSpeed = INT_TO_OD_DATA(trayOutSpeed);
        params.zResetSpeed = INT_TO_OD_DATA(zResetSpeed);
        params.trayResetSpeed = INT_TO_OD_DATA(trayResetSpeed);
        params.tempCompensationOut = DOUBLE_TO_OD_DATA(tempCompensationOut);
        params.tempCompensationMid = DOUBLE_TO_OD_DATA(tempCompensationMid);
        params.tempCompensationIn = DOUBLE_TO_OD_DATA(tempCompensationIn);
    }

    return result;
}

//修改全部参数
int HeatSealPart::setParameter(const HeatSealPart::HeatSealParams &params)
{
    int result = -1;
    quint16 index = HEATSEAL_INDEX(PARAMETER);

    Commander c(DevicePart::NODE_HEATSEAL);
    QVector<Commander::SDOitem> items;

    int targetTemp = DOUBLE_FROM_OD_DATA(params.targetTemp);
    int zDownPosition = DOUBLE_FROM_OD_DATA(params.zDownPosition);
    int trayOutPosition = DOUBLE_FROM_OD_DATA(params.trayOutPosition);
    int tempCompensationOut = DOUBLE_FROM_OD_DATA(params.tempCompensationOut);
    int tempCompensationIn = DOUBLE_FROM_OD_DATA(params.tempCompensationIn);

    items << Commander::SDOitem(index, 0x03, &targetTemp) \
          << Commander::SDOitem(index, 0x05, &zDownPosition) \
          << Commander::SDOitem(index, 0x06, &trayOutPosition) \
          << Commander::SDOitem(index, 0x0C, &tempCompensationOut) \
          << Commander::SDOitem(index, 0x0D, &tempCompensationIn);

    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int HeatSealPart::motorMove(int motorIndex, const DevicePart::MotorMotionParams &param)
{
    int result = -1;
    quint16 index = HEATSEAL_INDEX(MOTOR_MOVE);

    Commander c(DevicePart::NODE_HEATSEAL);

    QVector<Commander::SDOitem> items;
    int targetPosition = DOUBLE_FROM_OD_DATA(param.targetPosition);
    int speed = DOUBLE_FROM_OD_DATA(param.speed);

    Commander::SDOitem controlItem;
    Commander::SDOIndex statusIndex;

    if (MOTOR_Z == motorIndex) {
        controlItem.index.index = index;
        controlItem.index.sub_index = 0x01;
        controlItem.data = ACTION_CONTROL;

        statusIndex.index =  index;
        statusIndex.sub_index = 0x02;

        items << Commander::SDOitem(index, 0x03, &speed) \
              << Commander::SDOitem(index, 0x04, &targetPosition);
    } else {
        controlItem.index.index = index;
        controlItem.index.sub_index = 0x06;
        controlItem.data = ACTION_CONTROL;

        statusIndex.index = index;
        statusIndex.sub_index = 0x07;

        items << Commander::SDOitem(index, 0x08, &speed) \
              << Commander::SDOitem(index, 0x09, &targetPosition);
    }

    c.writeRemoteData(items);

    result = c.exec(controlItem, statusIndex);

    return result;
}

//在位检测
int HeatSealPart::checkBoardStatus()
{
    int boardStatus = -1;

    quint16 index = HEATSEAL_INDEX(BOARD_STATUS);

    Commander c(DevicePart::NODE_HEATSEAL);
    QVector<Commander::SDOitem> items;

    items << Commander::SDOitem(index, 0x01, &boardStatus);

    c.readRemoteData(items);

    return boardStatus;
}

int HeatSealPart::motorReset(int motorIndex)
{
    int result = -1;
    quint16 index = HEATSEAL_INDEX(MOTOR_RESET);

    Commander c(DevicePart::NODE_HEATSEAL);

    Commander::SDOitem controlItem;
    Commander::SDOIndex statusIndex;

    if (MOTOR_Z == motorIndex)
    {
        controlItem.index.index = index;
        controlItem.index.sub_index = 0x01;
        controlItem.data = ACTION_CONTROL;

        statusIndex.index = index;
        statusIndex.sub_index = 0x02;

    }
    else
    {
        controlItem.index.index = index;
        controlItem.index.sub_index = 0x04;
        controlItem.data = ACTION_CONTROL;

        statusIndex.index = index;
        statusIndex.sub_index = 0x05;

    }

    result = c.exec(controlItem, statusIndex);

    return result;
}
