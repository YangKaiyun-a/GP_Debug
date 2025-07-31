#include "coolstorepart.h"

#include "Driver/commander.h"
#include "stdio.h"

#include <QThread>
#include <QDebug>

CoolStorePart::CoolStorePart(QObject *parent) : AbstractPart(parent)
{
}

CoolStorePart *CoolStorePart::instance()
{
    static CoolStorePart coolStore;
    return &coolStore;
}

void CoolStorePart::init()
{

}

void CoolStorePart::readTemp()
{
    if(m_bReadTemp)
    {
        return;
    }

    m_bReadTemp = true;

    quint16 index = COLDSTORE_INDEX(CURRENT_TEMP);
    Commander c(DevicePart::NODE_EXTRACT_COOLSTORE);

    int temp1 = 0, temp2 = 0, temp3 = 0;
    QList<float> temps;
    QVector<Commander::SDOitem> items;
    items << Commander::SDOitem(index, 0x01, &temp1) \
          << Commander::SDOitem(index, 0x02, &temp2) \
          << Commander::SDOitem(index, 0x03, &temp3);

    while(m_bReadTemp)
    {
        QThread::msleep(2000);
        temps.clear();
        c.readRemoteData(items);
        temps << DOUBLE_TO_OD_DATA(temp1) << DOUBLE_TO_OD_DATA(temp2) << DOUBLE_TO_OD_DATA(temp3);
        Q_EMIT sigCurrentTemp(temps);
    }
}

int CoolStorePart::moduleReset()
{
    int result = -1;
    quint16 index = MODULE_RESET_INDEX;

    Commander c(DevicePart::NODE_EXTRACT_COOLSTORE);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int CoolStorePart::startLeftCool()
{
    int result = -1;
    quint16 index = COLDSTORE_INDEX(COOL_LEFT_START);

    Commander c(DevicePart::NODE_EXTRACT_COOLSTORE);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int CoolStorePart::startRightCool()
{
    int result = -1;
    quint16 index = COLDSTORE_INDEX(COOL_RIGHT_START);

    Commander c(DevicePart::NODE_EXTRACT_COOLSTORE);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int CoolStorePart::startSysbuildCool()
{
    int result = -1;
    quint16 index = COLDSTORE_INDEX(COOL_SYSBUILD_START);

    Commander c(DevicePart::NODE_EXTRACT_COOLSTORE);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int CoolStorePart::stopLeftCool()
{
    int result = -1;
    quint16 index = COLDSTORE_INDEX(COOL_LEFT_STOP);

    Commander c(DevicePart::NODE_EXTRACT_COOLSTORE);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int CoolStorePart::stopRightCool()
{
    int result = -1;
    quint16 index = COLDSTORE_INDEX(COOL_RIGHT_STOP);

    Commander c(DevicePart::NODE_EXTRACT_COOLSTORE);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int CoolStorePart::stopSysbuildCool()
{
    int result = -1;
    quint16 index = COLDSTORE_INDEX(COOL_SYSBUILD_STOP);

    Commander c(DevicePart::NODE_EXTRACT_COOLSTORE);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}


int CoolStorePart::openLeftCover()
{
    int result = -1;
    quint16 index = COLDSTORE_INDEX(COVER_LEFT_OPEN);

    Commander c(DevicePart::NODE_EXTRACT_COOLSTORE);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int CoolStorePart::closeLeftCover()
{
    int result = -1;
    quint16 index = COLDSTORE_INDEX(COVER_LEFT_CLOSE);

    Commander c(DevicePart::NODE_EXTRACT_COOLSTORE);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int CoolStorePart::openRightCover()
{
    int result = -1;
    quint16 index = COLDSTORE_INDEX(COVER_RIGHT_OPEN);

    Commander c(DevicePart::NODE_EXTRACT_COOLSTORE);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int CoolStorePart::closeRightCover()
{
    int result = -1;
    quint16 index = COLDSTORE_INDEX(COVER_RIGHT_CLOSE);

    Commander c(DevicePart::NODE_EXTRACT_COOLSTORE);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

void CoolStorePart::test(CoolStorePart::testparam param, int is_pause)
{
    if (m_test)
        return;

    m_test = true;

    if(is_pause == 0)
    {
        current_loop_num = 0;
    }

    while(m_test&&(current_loop_num < param.loop_num))
    {
        Q_EMIT testCount(++current_loop_num);
        openLeftCover();
        openRightCover();
        closeLeftCover();
        closeRightCover();
    }

    m_test = false;
}

void CoolStorePart::stoptest()
{
    m_test = false;
}


int CoolStorePart::motorMove(int motorIndex, const DevicePart::MotorMotionParams&param)
{

    int result = -1;
    quint16 index = COLDSTORE_INDEX(MOTOR_MOVE);

    Commander c(DevicePart::NODE_EXTRACT_COOLSTORE);

    QVector<Commander::SDOitem> items;
    int targetPosition = DOUBLE_FROM_OD_DATA(param.targetPosition);
    int speed = DOUBLE_FROM_OD_DATA(param.speed);

    Commander::SDOitem controlItem;
    Commander::SDOIndex statusIndex;
    Commander::SDOIndex speedItem;
    Commander::SDOIndex curPositionItem;
    Commander::SDOIndex targetPositionItem;

    if (MOTOR_LEFT == motorIndex)
    {
        controlItem.index.index = index;
        controlItem.index.sub_index = 0x01;
        controlItem.data = ACTION_CONTROL;

        statusIndex.index =  index;
        statusIndex.sub_index = 0x02;

        items << Commander::SDOitem(index, 0x03, &speed) \
              << Commander::SDOitem(index, 0x04, &targetPosition);
    }
    else if (MOTOR_RIGHT == motorIndex)
    {
        controlItem.index.index = index;
        controlItem.index.sub_index = 0x06;
        controlItem.data = ACTION_CONTROL;

        statusIndex.index =  index;
        statusIndex.sub_index = 0x07;

        items << Commander::SDOitem(index, 0x08, &speed) \
              << Commander::SDOitem(index, 0x09, &targetPosition);

    }

    c.writeRemoteData(items);
    result = c.exec(controlItem, statusIndex);

    return result;
}

int CoolStorePart::motorReset(int motorIndex)
{
    int result = -1;
    Commander::SDOitem controlItem;
    Commander::SDOIndex statusItem;

    quint16 index = COLDSTORE_INDEX(MOTOR_RESET);

    Commander c(DevicePart::NODE_EXTRACT_COOLSTORE);

    if(motorIndex == MOTOR_LEFT)
    {
        controlItem.index.index = index;
        controlItem.index.sub_index = 0x01;
        controlItem.data = ACTION_CONTROL;

        statusItem.index =  index;
        statusItem.sub_index = 0x02;
    }
    else if(motorIndex == MOTOR_RIGHT)
    {
        controlItem.index.index = index;
        controlItem.index.sub_index = 0x03;
        controlItem.data = ACTION_CONTROL;

        statusItem.index =  index;
        statusItem.sub_index = 0x04;
    }

    result = c.exec(controlItem, statusItem);

    return result;
}

int CoolStorePart::setParameter(const CoolStorePart::CoolStoreParams &params)
{
    int result = -1;
    quint16 index = COLDSTORE_INDEX(PARAMETER);

    Commander c(DevicePart::NODE_EXTRACT_COOLSTORE);
    QVector<Commander::SDOitem> items;

    int left_k = DOUBLE_FROM_OD_DATA(params.left_k);
    int left_b = DOUBLE_FROM_OD_DATA(params.left_b);
    int right_k = DOUBLE_FROM_OD_DATA(params.right_k);
    int right_b = DOUBLE_FROM_OD_DATA(params.right_b);
    int sysbuild_k = DOUBLE_FROM_OD_DATA(params.sysbuild_k);
    int sysbuild_b = DOUBLE_FROM_OD_DATA(params.sysbuild_b);
    int left_cover_openPosition = INT_FROM_OD_DATA(params.left_cover_openPosition);
    int left_cover_closePosition = INT_FROM_OD_DATA(params.left_cover_closePosition);
    int right_cover_openPosition = DOUBLE_FROM_OD_DATA(params.right_cover_openPosition);
    int right_cover_closePosition = DOUBLE_FROM_OD_DATA(params.right_cover_closePosition);
    int left_cover_speed = DOUBLE_FROM_OD_DATA(params.left_cover_speed);
    int right_cover_speed = DOUBLE_FROM_OD_DATA(params.right_cover_speed);
    int left_target_temperature = DOUBLE_FROM_OD_DATA(params.left_target_temperature);
    int right_target_temperature = DOUBLE_FROM_OD_DATA(params.right_target_temperature);
    int sysbuild_target_temperature = DOUBLE_FROM_OD_DATA(params.sysbuilding_target_temperature);

    items << Commander::SDOitem(index, 0x03, &left_k) \
          << Commander::SDOitem(index, 0x04, &left_b) \
          << Commander::SDOitem(index, 0x05, &right_k) \
          << Commander::SDOitem(index, 0x06, &right_b) \
          << Commander::SDOitem(index, 0x07, &sysbuild_k) \
          << Commander::SDOitem(index, 0x08, &sysbuild_b) \
          << Commander::SDOitem(index, 0x09, &left_cover_openPosition) \
          << Commander::SDOitem(index, 0x0A, &left_cover_closePosition) \
          << Commander::SDOitem(index, 0x0B, &right_cover_openPosition) \
          << Commander::SDOitem(index, 0x0C, &right_cover_closePosition) \
          << Commander::SDOitem(index, 0x0D, &left_cover_speed) \
          << Commander::SDOitem(index, 0x0E, &right_cover_speed) \
          << Commander::SDOitem(index, 0x0F, &left_target_temperature) \
          << Commander::SDOitem(index, 0x10, &right_target_temperature) \
          << Commander::SDOitem(index, 0x11, &sysbuild_target_temperature);

    result = c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    c.exec(controlItem, statusIndex);

    return result;
}

int CoolStorePart::getParameter(CoolStorePart::CoolStoreParams &params)
{
    int result = -1;
    quint16 index = COLDSTORE_INDEX(PARAMETER);

    Commander c(DevicePart::NODE_EXTRACT_COOLSTORE);
    QVector<Commander::SDOitem> items;

    int left_k;
    int left_b;
    int right_k;
    int right_b;
    int sysbuild_k;
    int sysbuild_b;
    int left_cover_openPosition;
    int left_cover_closePosition;
    int right_cover_openPosition;
    int right_cover_closePosition;
    int left_cover_speed;
    int right_cover_speed;
    int left_target_temperature;
    int right_target_temperature;
    int sysbuild_target_temperature;

    items << Commander::SDOitem(index, 0x03, &left_k) \
          << Commander::SDOitem(index, 0x04, &left_b) \
          << Commander::SDOitem(index, 0x05, &right_k) \
          << Commander::SDOitem(index, 0x06, &right_b) \
          << Commander::SDOitem(index, 0x07, &sysbuild_k) \
          << Commander::SDOitem(index, 0x08, &sysbuild_b) \
          << Commander::SDOitem(index, 0x09, &left_cover_openPosition) \
          << Commander::SDOitem(index, 0x0A, &left_cover_closePosition) \
          << Commander::SDOitem(index, 0x0B, &right_cover_openPosition) \
          << Commander::SDOitem(index, 0x0C, &right_cover_closePosition) \
          << Commander::SDOitem(index, 0x0D, &left_cover_speed) \
          << Commander::SDOitem(index, 0x0E, &right_cover_speed) \
          << Commander::SDOitem(index, 0x0F, &left_target_temperature) \
          << Commander::SDOitem(index, 0x10, &right_target_temperature)\
          << Commander::SDOitem(index, 0x11, &sysbuild_target_temperature);
    result = c.readRemoteData(items);

    if (!result) {
        params.left_k = DOUBLE_TO_OD_DATA(left_k);
        params.left_b = DOUBLE_TO_OD_DATA(left_b);
        params.right_k = DOUBLE_TO_OD_DATA(right_k);
        params.right_b = DOUBLE_TO_OD_DATA(right_b);
        params.sysbuild_k = DOUBLE_TO_OD_DATA(sysbuild_k);
        params.sysbuild_b = DOUBLE_TO_OD_DATA(sysbuild_b);
        params.left_cover_openPosition = DOUBLE_TO_OD_DATA(left_cover_openPosition);
        params.left_cover_closePosition = DOUBLE_TO_OD_DATA(left_cover_closePosition);
        params.right_cover_openPosition = DOUBLE_TO_OD_DATA(right_cover_openPosition);
        params.right_cover_closePosition = DOUBLE_TO_OD_DATA(right_cover_closePosition);
        params.left_cover_speed = DOUBLE_TO_OD_DATA(left_cover_speed);
        params.right_cover_speed = DOUBLE_TO_OD_DATA(right_cover_speed);
        params.left_target_temperature = DOUBLE_TO_OD_DATA(left_target_temperature);
        params.right_target_temperature = DOUBLE_TO_OD_DATA(right_target_temperature);
        params.sysbuilding_target_temperature = DOUBLE_TO_OD_DATA(sysbuild_target_temperature);
    }

    return result;
}

//保存核酸暂存位参数-左参数
int CoolStorePart::setLeftParameter(const CoolStoreParams &params)
{
    int result = -1;
    quint16 index = COLDSTORE_INDEX(PARAMETER);

    Commander c(DevicePart::NODE_EXTRACT_COOLSTORE);
    QVector<Commander::SDOitem> items;

    int left_k = DOUBLE_FROM_OD_DATA(params.left_k);
    int left_b = DOUBLE_FROM_OD_DATA(params.left_b);
    int left_cover_closePosition = INT_FROM_OD_DATA(params.left_cover_closePosition);
    int left_target_temperature = DOUBLE_FROM_OD_DATA(params.left_target_temperature);

    items << Commander::SDOitem(index, 0x03, &left_k) \
          << Commander::SDOitem(index, 0x04, &left_b) \
          << Commander::SDOitem(index, 0x0A, &left_cover_closePosition) \
          << Commander::SDOitem(index, 0x0F, &left_target_temperature);

    result = c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    c.exec(controlItem, statusIndex);

    return result;
}

//保存核酸暂存位-右参数
int CoolStorePart::setRightParameter(const CoolStoreParams &params)
{
    int result = -1;
    quint16 index = COLDSTORE_INDEX(PARAMETER);

    Commander c(DevicePart::NODE_EXTRACT_COOLSTORE);
    QVector<Commander::SDOitem> items;

    int right_k = DOUBLE_FROM_OD_DATA(params.right_k);
    int right_b = DOUBLE_FROM_OD_DATA(params.right_b);
    int right_cover_closePosition = INT_FROM_OD_DATA(params.right_cover_closePosition);
    int right_target_temperature = DOUBLE_FROM_OD_DATA(params.right_target_temperature);

    items << Commander::SDOitem(index, 0x05, &right_k) \
          << Commander::SDOitem(index, 0x06, &right_b) \
          << Commander::SDOitem(index, 0x0C, &right_cover_closePosition) \
          << Commander::SDOitem(index, 0x10, &right_target_temperature);

    result = c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    c.exec(controlItem, statusIndex);

    return result;
}

//体系构建位-保存参数
int CoolStorePart::setSysbuildParameter(const CoolStoreParams &params)
{
    int result = -1;
    quint16 index = COLDSTORE_INDEX(PARAMETER);

    Commander c(DevicePart::NODE_EXTRACT_COOLSTORE);
    QVector<Commander::SDOitem> items;

    int sysbuild_k = DOUBLE_FROM_OD_DATA(params.sysbuild_k);
    int sysbuild_b = DOUBLE_FROM_OD_DATA(params.sysbuild_b);
    int sysbuilding_target_temperature = INT_FROM_OD_DATA(params.sysbuilding_target_temperature);

    items << Commander::SDOitem(index, 0x07, &sysbuild_k) \
          << Commander::SDOitem(index, 0x08, &sysbuild_b) \
          << Commander::SDOitem(index, 0x11, &sysbuilding_target_temperature);

    result = c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    c.exec(controlItem, statusIndex);

    return result;
}
