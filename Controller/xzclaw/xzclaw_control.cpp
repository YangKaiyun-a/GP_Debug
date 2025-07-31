#include "xzclaw_control.h"
#include "Driver/commander.h"

xzclaw_control::xzclaw_control()
{

}

xzclaw_control *xzclaw_control::instance()
{
    static xzclaw_control xzclaw_control_ins;
    return &xzclaw_control_ins;
}

quint16 xzclaw_control::select_motor_reset_index(int motor_id)
{
    quint16 index = 0;

    switch (motor_id) {
    case 0:
       index = XY_INDEX(X_RESET);
       break;
    case 1:
        index = CLAW_INDEX(CLAW_Z_RESET);
        break;
    case 2:
       index = XZ_CLAW_INDEX(M1_RESET);
       break;
    case 3:
        index = XZ_CLAW_INDEX(M2_RESET);
        break;
    case 4:
       index = XZ_CLAW_INDEX(JACK_RESET);
       break;
    default:
        break;
    }

    return index;
}

quint16 xzclaw_control::select_motor_move_index(int motor_id)
{
    quint16 index = 0;

    switch (motor_id) {
    case 0:
       index = XY_INDEX(X_MOVE);
       break;
    case 1:
        index = CLAW_INDEX(CLAW_Z_MOVE);
        break;
    case 2:
       index = XZ_CLAW_INDEX(M1_MOVE);
       break;
    case 3:
        index = XZ_CLAW_INDEX(M2_MOVE);
        break;
    case 4:
       index = XZ_CLAW_INDEX(JACK_MOVE);
       break;
    default:
        break;
    }

    return index;
}

int xzclaw_control::claw_reset()
{
    int result = -1;
    quint16 index = CLAW_INDEX(CLAW_RESET);

    Commander c(DevicePart::NODE_XZ_CLAW);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}

int xzclaw_control::claw_glab(float glabWidth)
{
    int result = -1;
    quint16 index = CLAW_INDEX(CLAW_GLAB);

    Commander c(DevicePart::NODE_XZ_CLAW);

    QVector<Commander::SDOitem> items;
    int width = DOUBLE_FROM_OD_DATA(glabWidth);
    items << Commander::SDOitem(index, 0x03, &width);

    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}

int xzclaw_control::claw_release(float releaseWidth)
{
    int result = -1;
    quint16 index = CLAW_INDEX(CLAW_RELEASE);

    Commander c(DevicePart::NODE_XZ_CLAW);

    QVector<Commander::SDOitem> items;
    int width = DOUBLE_FROM_OD_DATA(releaseWidth);
    items << Commander::SDOitem(index, 0x03, &width);

    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}

int xzclaw_control::motorReset(int motorIndex)
{
    int result = -1;

    quint16 index = select_motor_reset_index(motorIndex);

    Commander c(DevicePart::NODE_XZ_CLAW);

    Commander::SDOitem controlItem;
    Commander::SDOIndex statusIndex;
    controlItem.index.index = index;
    statusIndex.index = index;

    if(motorIndex != Z_comboBox)
    {
        controlItem.index.sub_index = 0x01;
        controlItem.data = ACTION_CONTROL;
        statusIndex.sub_index = 0x02;
    }
    else
    {
        controlItem.index.sub_index = 0x03;
        controlItem.data = ACTION_CONTROL;
        statusIndex.sub_index = 0x04;
    }

    result = c.exec(controlItem, statusIndex);

    return result;
}

int xzclaw_control::motorMove(int motorIndex, const DevicePart::MotorMotionParams &param)
{
    int result = -1;

    quint16 index = select_motor_move_index(motorIndex);
    Commander c(DevicePart::NODE_XZ_CLAW);

    QVector<Commander::SDOitem> items;

    int speed = DOUBLE_FROM_OD_DATA(param.speed);
    int targetPosition = DOUBLE_FROM_OD_DATA(param.targetPosition);
    items << Commander::SDOitem(index, 0x03, &speed)
          << Commander::SDOitem(index, 0x04, &targetPosition);

    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}

int xzclaw_control::getParameter(xzclaw_control::xzclaw_Params &params)
{
    int result = -1;
    quint16 index = XZ_CLAW_INDEX(CLAW_PARAM);

    Commander c(DevicePart::NODE_XZ_CLAW);
    QVector<Commander::SDOitem> items;

    int z_fly_position = 0;
//    int z_down_position = 0;
    int z_speed = 0;
//    int z_zero_offset = 0;
    int M1_Close_Position = 0;
    int M1_Close_Speed = 0;
    int M2_Close_Position = 0;
    int M2_Close_Speed = 0;
    int Jack_Transporter_Position = 0;
    int Jack_wait_Position = 0;
    int Jack_Speed = 0;
    int z_door_Position = 0;

    items << Commander::SDOitem(index, 0x03, &z_fly_position) \
          << Commander::SDOitem(index, 0x05, &z_speed) \
          << Commander::SDOitem(index, 0x07, &M1_Close_Position) \
          << Commander::SDOitem(index, 0x08, &M1_Close_Speed) \
          << Commander::SDOitem(index, 0x09, &M2_Close_Position) \
          << Commander::SDOitem(index, 0x0A, &M2_Close_Speed) \
          << Commander::SDOitem(index, 0x0B, &Jack_Transporter_Position) \
          << Commander::SDOitem(index, 0x0C, &Jack_wait_Position) \
          << Commander::SDOitem(index, 0x0D, &Jack_Speed)\
            << Commander::SDOitem(index, 0x0E, &z_door_Position);

    result = c.readRemoteData(items);

    if (!result) {
        params.z_fly_position = DOUBLE_TO_OD_DATA(z_fly_position);
        params.z_speed = DOUBLE_TO_OD_DATA(z_speed);
        params.M1_Close_Position = DOUBLE_TO_OD_DATA(M1_Close_Position);
        params.M1_Close_Speed = DOUBLE_TO_OD_DATA(M1_Close_Speed);
        params.M2_Close_Position = DOUBLE_TO_OD_DATA(M2_Close_Position);
        params.M2_Close_Speed = DOUBLE_TO_OD_DATA(M2_Close_Speed);

        params.Jack_Transporter_Position = DOUBLE_TO_OD_DATA(Jack_Transporter_Position);
        params.Jack_wait_Position = DOUBLE_TO_OD_DATA(Jack_wait_Position);
        params.Jack_Speed = DOUBLE_TO_OD_DATA(Jack_Speed);
        params.z_door_position = DOUBLE_TO_OD_DATA(z_door_Position);
    }

    return result;
}

int xzclaw_control::setParameter(const xzclaw_control::xzclaw_Params &params)
{
    int result = -1;
    quint16 index = XZ_CLAW_INDEX(CLAW_PARAM);

    Commander c(DevicePart::NODE_XZ_CLAW);
    QVector<Commander::SDOitem> items;

    int z_fly_position = DOUBLE_FROM_OD_DATA(params.z_fly_position);
    int z_speed = DOUBLE_FROM_OD_DATA(params.z_speed);
    int M1_Close_Position = DOUBLE_FROM_OD_DATA(params.M1_Close_Position);
    int M1_Close_Speed = DOUBLE_FROM_OD_DATA(params.M1_Close_Speed);
    int M2_Close_Position = DOUBLE_FROM_OD_DATA(params.M2_Close_Position);
    int M2_Close_Speed = DOUBLE_FROM_OD_DATA(params.M2_Close_Speed);
    int Jack_Transporter_Position = DOUBLE_FROM_OD_DATA(params.Jack_Transporter_Position);
    int Jack_wait_Position = DOUBLE_FROM_OD_DATA(params.Jack_wait_Position);
    int Jack_Speed = DOUBLE_FROM_OD_DATA(params.Jack_Speed);
    int z_door_position = DOUBLE_FROM_OD_DATA(params.z_door_position);

    items << Commander::SDOitem(index, 0x03, &z_fly_position) \
          << Commander::SDOitem(index, 0x05, &z_speed) \
          << Commander::SDOitem(index, 0x07, &M1_Close_Position) \
          << Commander::SDOitem(index, 0x08, &M1_Close_Speed) \
          << Commander::SDOitem(index, 0x09, &M2_Close_Position) \
          << Commander::SDOitem(index, 0x0A, &M2_Close_Speed) \
          << Commander::SDOitem(index, 0x0B, &Jack_Transporter_Position) \
          << Commander::SDOitem(index, 0x0C, &Jack_wait_Position) \
          << Commander::SDOitem(index, 0x0D, &Jack_Speed)\
            << Commander::SDOitem(index, 0x0E, &z_door_position);

    result = c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    c.exec(controlItem, statusIndex);

    return result;
}

int xzclaw_control::module_reset()
{
    int result = -1;
    quint16 index = MODULE_RESET_INDEX;

    Commander c(DevicePart::NODE_XZ_CLAW);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int xzclaw_control::M1_Open_entry()
{
    int result = -1;
    quint16 index = XZ_CLAW_INDEX(DOOR_OPEN);

    Commander c(DevicePart::NODE_XZ_CLAW);
    QVector<Commander::SDOitem> items;

    int num = 1;
    items << Commander::SDOitem(index, 0x03, &num);

    result = c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int xzclaw_control::M1_Close_entry()
{
    int result = -1;
    quint16 index = XZ_CLAW_INDEX(DOOR_CLOSE);

    Commander c(DevicePart::NODE_XZ_CLAW);
    QVector<Commander::SDOitem> items;

    int num = 1;
    items << Commander::SDOitem(index, 0x03, &num);

    result = c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int xzclaw_control::M2_Open_entry()
{
    int result = -1;
    quint16 index = XZ_CLAW_INDEX(DOOR_OPEN);

    Commander c(DevicePart::NODE_XZ_CLAW);
    QVector<Commander::SDOitem> items;

    int num = 2;
    items << Commander::SDOitem(index, 0x03, &num);

    result = c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int xzclaw_control::M2_Close_entry()
{
    int result = -1;
    quint16 index = XZ_CLAW_INDEX(DOOR_CLOSE);

    Commander c(DevicePart::NODE_XZ_CLAW);
    QVector<Commander::SDOitem> items;

    int num = 2;
    items << Commander::SDOitem(index, 0x03, &num);

    result = c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int xzclaw_control::JACK_Transporter_Position_entry()
{
    int result = -1;
    quint16 index = XZ_CLAW_INDEX(JACK_TRANSPORT);

    Commander c(DevicePart::NODE_XZ_CLAW);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int xzclaw_control::JACK_Wait_Position_entry()
{
    int result = -1;
    quint16 index = XZ_CLAW_INDEX(JACK_WAIT);

    Commander c(DevicePart::NODE_XZ_CLAW);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int xzclaw_control::uvlight_1_open_entry()
{
    int result = -1;
    quint16 index = UVLIGHT_INDEX(UVLIGHT_1_OPEN);

    Commander c(DevicePart::NODE_XZ_CLAW);

//    QVector<Commander::SDOitem> items;
//    int num = 2;
//    items << Commander::SDOitem(index, 0x03, &num);
//    result = c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int xzclaw_control::uvlight_1_close_entry()
{
    int result = -1;
    quint16 index = UVLIGHT_INDEX(UVLIGHT_1_CLOSE);

    Commander c(DevicePart::NODE_XZ_CLAW);

//    QVector<Commander::SDOitem> items;
//    int num = 2;
//    items << Commander::SDOitem(index, 0x03, &num);
//    result = c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int xzclaw_control::toZGlab(float zPosition, float openWidth, float z_fly_Position)
{
    int result = -1;
    quint16 index = CLAW_INDEX(CLAW_TO_Z_GLAB);

    QVector<Commander::SDOitem> items;
    int zPos = DOUBLE_FROM_OD_DATA(zPosition);
    int widthOpen = DOUBLE_FROM_OD_DATA(openWidth);
    int Zfly_Position = DOUBLE_FROM_OD_DATA(z_fly_Position);


    items << Commander::SDOitem(index, 0x03, &zPos) \
          << Commander::SDOitem(index, 0x04, &widthOpen)\
            << Commander::SDOitem(index, 0x06, &Zfly_Position);

    Commander c(DevicePart::NODE_XZ_CLAW);
    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}

int xzclaw_control::toZRelease(float zPosition, float releaseWidth, float z_fly_Position)
{
    int result = -1;
    quint16 index = CLAW_INDEX(CLAW_TO_Z_RELEASE);

    QVector<Commander::SDOitem> items;
    int zPos = DOUBLE_FROM_OD_DATA(zPosition);
    int width = DOUBLE_FROM_OD_DATA(releaseWidth);
    int Zfly_Position = DOUBLE_FROM_OD_DATA(z_fly_Position);

    items << Commander::SDOitem(index, 0x03, &zPos)\
          << Commander::SDOitem(index, 0x04, &width)\
          << Commander::SDOitem(index, 0x05, &Zfly_Position);

    Commander c(DevicePart::NODE_XZ_CLAW);
    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}
