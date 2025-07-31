#include "switch_signal.h"
#include "Driver/commander.h"

Switch_Signal::Switch_Signal()
{

}

Switch_Signal *Switch_Signal::instance()
{
    static Switch_Signal switch_signal_ins;
    return &switch_signal_ins;
}

int Switch_Signal::moduleReset()
{
    int result = -1;
    quint16 index = MODULE_RESET_INDEX;

    Commander c(DevicePart::NODE_SWITCH_SIGNAL);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Switch_Signal::motorReset(int motorIndex)
{
    int result = -1;
    Commander::SDOitem controlItem;
    Commander::SDOIndex statusItem;

    quint16 index = SWITCH_SIGNAL_INDEX(MOTOR_RESET);

    Commander c(DevicePart::NODE_SWITCH_SIGNAL);

    controlItem.index.index = index;
    controlItem.index.sub_index = 0x01;
    controlItem.data = ACTION_CONTROL;

    statusItem.index =  index;
    statusItem.sub_index = 0x02;

    result = c.exec(controlItem, statusItem);

    return result;
}

int Switch_Signal::motorMove(int motorIndex, const DevicePart::MotorMotionParams &param)
{
    int result = -1;
    quint16 index = SWITCH_SIGNAL_INDEX(MOTOR_MOVE);

    Commander c(DevicePart::NODE_SWITCH_SIGNAL);

    QVector<Commander::SDOitem> items;
    int targetPosition = DOUBLE_FROM_OD_DATA(param.targetPosition);
    int speed = DOUBLE_FROM_OD_DATA(param.speed);

    Commander::SDOitem controlItem;
    Commander::SDOIndex statusIndex;

    controlItem.index.index = index;
    controlItem.index.sub_index = 0x01;
    controlItem.data = ACTION_CONTROL;

    statusIndex.index =  index;
    statusIndex.sub_index = 0x02;

    items << Commander::SDOitem(index, 0x03, &speed) \
          << Commander::SDOitem(index, 0x04, &targetPosition);

    c.writeRemoteData(items);

    result = c.exec(controlItem, statusIndex);

    return result;
}

//打开废料门
int Switch_Signal::openCover()
{
    int result = -1;
    quint16 index = SWITCH_SIGNAL_INDEX(COVER_OPEN);

    Commander c(DevicePart::NODE_SWITCH_SIGNAL);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

//关闭废料门
int Switch_Signal::closeCover()
{
    int result = -1;
    quint16 index = SWITCH_SIGNAL_INDEX(COVER_CLOSE);

    Commander c(DevicePart::NODE_SWITCH_SIGNAL);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Switch_Signal::waste_pump_open()
{
    int result = -1;
    quint16 index = SWITCH_SIGNAL_INDEX(WASTE_PUMP_OPEN);

    Commander c(DevicePart::NODE_SWITCH_SIGNAL);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Switch_Signal::waste_pump_close()
{
    int result = -1;
    quint16 index = SWITCH_SIGNAL_INDEX(WASTE_PUMP_CLOSE);

    Commander c(DevicePart::NODE_SWITCH_SIGNAL);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Switch_Signal::tray_light_open()
{
    int result = -1;
    quint16 index = SWITCH_SIGNAL_INDEX(TRAY_LIGHT_OPEN);

    Commander c(DevicePart::NODE_SWITCH_SIGNAL);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Switch_Signal::tray_light_close()
{
    int result = -1;
    quint16 index = SWITCH_SIGNAL_INDEX(TRAY_LIGHT_CLOSE);

    Commander c(DevicePart::NODE_SWITCH_SIGNAL);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Switch_Signal::getParameter(Switch_Signal::Switch_SignalParams &params)
{
    int result = -1;

    quint16 index = SWITCH_SIGNAL_INDEX(PARAMETER);

    Commander c(DevicePart::NODE_SWITCH_SIGNAL);
    QVector<Commander::SDOitem> items;
    int cover_openPosition;
    int cover_closePosition;
    int cover_speed;

    items << Commander::SDOitem(index, 0x03, &cover_openPosition) \
          << Commander::SDOitem(index, 0x04, &cover_closePosition) \
          << Commander::SDOitem(index, 0x05, &cover_speed);

    result = c.readRemoteData(items);

    if (!result) {
        params.cover_openPosition = DOUBLE_TO_OD_DATA(cover_openPosition);
        params.cover_closePosition = DOUBLE_TO_OD_DATA(cover_closePosition);
        params.cover_speed = DOUBLE_TO_OD_DATA(cover_speed);
    }

    return result;
}

int Switch_Signal::setParameter(const Switch_Signal::Switch_SignalParams &params)
{
    int result = -1;
    quint16 index = SWITCH_SIGNAL_INDEX(PARAMETER);

    Commander c(DevicePart::NODE_SWITCH_SIGNAL);
    QVector<Commander::SDOitem> items;

    int cover_openPosition = DOUBLE_FROM_OD_DATA(params.cover_openPosition);
    int cover_closePosition = DOUBLE_FROM_OD_DATA(params.cover_closePosition);
    int cover_speed = DOUBLE_FROM_OD_DATA(params.cover_speed);

    items << Commander::SDOitem(index, 0x03, &cover_openPosition) \
          << Commander::SDOitem(index, 0x04, &cover_closePosition) \
          << Commander::SDOitem(index, 0x05, &cover_speed);

    result = c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

//查询门状态
int Switch_Signal::checkDoorStatus(int doorIndex)
{
    int result = -1;
    int node_id = -1;
    quint8 subIndex = 0x01;

    quint16 index = SWITCH_SIGNAL_INDEX(DOOR_STATUS);

    switch (doorIndex)
    {
    case Left_Door:
        node_id = 0x09;
        subIndex = 0x01;
        break;
    case Middle_Carrier_Door:
        node_id = 0x0A;
        subIndex = 0x01;
        break;
    case Light_Door:
        node_id = 0x0E;
        subIndex = 0x01;
        break;
    case Left_Carrier_Door:
        node_id = 0x09;
        subIndex = 0x02;
        break;
    case Right_Carrier_Door:
        node_id = 0x0A;
        subIndex = 0x02;
        break;
    case Scraps_Door:
        node_id = 0x0E;
        subIndex = 0x02;
        break;
    case Middle_Door:
        node_id = 0x0A;
        subIndex = 0x03;
        break;
    default:
        break;
    }

    Commander c(node_id);
    QVector<Commander::SDOitem> items;

    items << Commander::SDOitem(index, subIndex, &result);

    c.readRemoteData(items);

    return result;

}
