#include "dp1_xy.h"
#include "Driver/commander.h"

Dp1_Xy::Dp1_Xy()
{

}

Dp1_Xy *Dp1_Xy::instance()
{
    static Dp1_Xy Dp1_Xy_ins;
    return &Dp1_Xy_ins;
}

quint16 Dp1_Xy::select_motor_reset_index(int motor_id)
{
    quint16 index;
    switch (motor_id) {
    case 0:
       index = XY_INDEX(XY_MOTOR_RESET);
       break;
    case 1:
        index = XY_INDEX(XY_MOTOR_RESET);
        break;
    case 2:
        index = DP1_INDEX(MOTOR_RESET);
        break;
    case 3:
        index = DP1_INDEX(MOTOR_RESET);
        break;
    }

    return index;
}

quint16 Dp1_Xy::select_motor_move_index(int motor_id)
{
    quint16 index;
    switch (motor_id) {
    case 0:
        index = XY_INDEX(XY_MOTOR_MOVE);
        break;
    case 1:
        index = XY_INDEX(XY_MOTOR_MOVE);
        break;
    case 2:
        index = DP1_INDEX(MOTOR_MOVE);
        break;
    case 3:
        index = DP1_INDEX(MOTOR_MOVE);
        break;
    }

    return index;
}

int Dp1_Xy::motorReset(int motorIndex)
{
    int result = -1;
    Commander::SDOitem controlItem;
    Commander::SDOIndex statusItem;

    quint16 index = select_motor_reset_index(motorIndex);

    Commander c(DevicePart::NODE_DP1_XY);
    if(motorIndex == MOTOR_X)
    {
        controlItem.index.index = index;
        controlItem.index.sub_index = 0x01;
        controlItem.data = ACTION_CONTROL;

        statusItem.index =  index;
        statusItem.sub_index = 0x02;
    }
    else if(motorIndex == MOTOR_Y)
    {
        controlItem.index.index = index;
        controlItem.index.sub_index = 0x04;
        controlItem.data = ACTION_CONTROL;

        statusItem.index =  index;
        statusItem.sub_index = 0x05;
    }
    result = c.exec(controlItem, statusItem);

    return result;
}

int Dp1_Xy::motorMove(int motorIndex, const DevicePart::MotorMotionParams &param)
{
    int result = -1;
    quint16 index = select_motor_move_index(motorIndex);

    Commander c(DevicePart::NODE_DP1_XY);

    QVector<Commander::SDOitem> items;
    int targetPosition = DOUBLE_FROM_OD_DATA(param.targetPosition);
    int speed = DOUBLE_FROM_OD_DATA(param.speed);

    Commander::SDOitem controlItem;
    Commander::SDOIndex statusIndex;

    if (motorIndex == MOTOR_X)
    {
        controlItem.index.index = index;
        controlItem.index.sub_index = 0x01;
        controlItem.data = ACTION_CONTROL;

        statusIndex.index =  index;
        statusIndex.sub_index = 0x02;

        items << Commander::SDOitem(index, 0x03, &speed) \
              << Commander::SDOitem(index, 0x04, &targetPosition);
    }
    else if (motorIndex == MOTOR_Y)
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

int Dp1_Xy::module_reset()
{
    int result = -1;
    quint16 index = MODULE_RESET_INDEX;

    Commander c(DevicePart::NODE_DP1_XY);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Dp1_Xy::move(const Dp1_Xy::RackMotionParams &param)
{
    int result = -1;
    quint16 index = XY_INDEX(XY_MOVE);
    int num = param.posNumber;
    int speed = DOUBLE_FROM_OD_DATA(param.speed);

    QVector<Commander::SDOitem> items;

    Commander::SDOitem numItem(index, 0x03, &num);
    Commander::SDOitem speedItem(index, 0x04, &speed);
    Commander::SDOitem posItem;

    items << numItem << speedItem;
    QVector<int> positions;
    positions.resize(param.posNumber * 2);

    for (quint8 i = 0x00; i < positions.size(); ++i) {
        posItem.index.index = index;
        posItem.index.sub_index = i + 0x05;
        positions[i] = DOUBLE_FROM_OD_DATA(param.positions.at(i));
        posItem.data = &positions[i];
        items.append(posItem);
    }

    Commander c(DevicePart::NODE_DP1_XY);
    result = c.writeRemoteData(items);

    if(result != 0)
    {
        return result;
    }

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}
