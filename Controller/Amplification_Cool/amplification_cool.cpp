#include "amplification_cool.h"
#include "Driver/commander.h"
#include <QThread>

Amplification_Cool::Amplification_Cool()
{

}

Amplification_Cool *Amplification_Cool::instance()
{
    static Amplification_Cool amplification_cool_ins;

    return &amplification_cool_ins;
}

int Amplification_Cool::moduleReset()
{
    int result = -1;
    quint16 index = MODULE_RESET_INDEX;

    Commander c(DevicePart::NODE_AMPLIFICATION_COOLSTORE);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Amplification_Cool::startCool()
{
    int result = -1;
    quint16 index = COLDSTORE_INDEX(COOL_LEFT_START);

    Commander c(DevicePart::NODE_AMPLIFICATION_COOLSTORE);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Amplification_Cool::stopCool()
{
    int result = -1;
    quint16 index = COLDSTORE_INDEX(COOL_LEFT_STOP);

    Commander c(DevicePart::NODE_AMPLIFICATION_COOLSTORE);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Amplification_Cool::tray_light_open()
{
    int result = -1;
    quint16 index = SWITCH_SIGNAL_INDEX(TRAY_LIGHT_OPEN);

    Commander c(DevicePart::NODE_AMPLIFICATION_COOLSTORE);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Amplification_Cool::tray_light_close()
{
    int result = -1;
    quint16 index = SWITCH_SIGNAL_INDEX(TRAY_LIGHT_CLOSE);

    Commander c(DevicePart::NODE_AMPLIFICATION_COOLSTORE);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Amplification_Cool::setParameter(const Amplification_Cool::CoolStoreParams &params)
{
    int result = -1;
    quint16 index = COLDSTORE_INDEX(PARAMETER);

    Commander c(DevicePart::NODE_AMPLIFICATION_COOLSTORE);
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

int Amplification_Cool::getParameter(Amplification_Cool::CoolStoreParams &params)
{
    int result = -1;
    quint16 index = COLDSTORE_INDEX(PARAMETER);

    Commander c(DevicePart::NODE_AMPLIFICATION_COOLSTORE);
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

void Amplification_Cool::readTemp()
{
    m_bReadTemp = true;

    quint16 index = COLDSTORE_INDEX(CURRENT_TEMP);
    Commander c(DevicePart::NODE_AMPLIFICATION_COOLSTORE);

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
