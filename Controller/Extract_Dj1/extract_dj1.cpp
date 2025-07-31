#include "extract_dj1.h"
#include "extract_dj1.h"
#include "Driver/commander.h"

Extract_DJ1::Extract_DJ1()
{

}

Extract_DJ1 *Extract_DJ1::instance()
{
    static Extract_DJ1 extract_dj1_ins;
    return &extract_dj1_ins;
}

quint16 Extract_DJ1::select_motor_reset_index(int motor_id)
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
        index = DJ1_INDEX(MOTOR_RESET);
        break;
    case 3:
        index = DJ1_INDEX(MOTOR_RESET);
        break;
    case 4:
        index = DJ1_INDEX(MOTOR_RESET);
        break;
    }

    return index;
}

quint16 Extract_DJ1::select_motor_move_index(int motor_id)
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
        index = DJ1_INDEX(MOTOR_MOVE);
        break;
    case 3:
        index = DJ1_INDEX(MOTOR_MOVE);
        break;
    case 4:
        index = DJ1_INDEX(MOTOR_MOVE);
        break;
    }

    return index;
}

int Extract_DJ1::motorReset(int motorIndex)
{
    int result = -1;
    Commander::SDOitem controlItem;
    Commander::SDOIndex statusItem;

    quint16 index = select_motor_reset_index(motorIndex);

    Commander c(DevicePart::NODE_EXTRACT_DJ1);
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
    else if(motorIndex == MOTOR_Z)
    {
        controlItem.index.index = index;
        controlItem.index.sub_index = 0x01;
        controlItem.data = ACTION_CONTROL;

        statusItem.index =  index;
        statusItem.sub_index = 0x02;
    }
    else if(motorIndex == MOTOR_BIG_PIPET)
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

int Extract_DJ1::motorMove(int motorIndex, const DevicePart::MotorMotionParams &param)
{
    int result = -1;
    quint16 index = select_motor_move_index(motorIndex);

    Commander c(DevicePart::NODE_EXTRACT_DJ1);

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
    else if (motorIndex == MOTOR_Z)
    {
        controlItem.index.index = index;
        controlItem.index.sub_index = 0x01;
        controlItem.data = ACTION_CONTROL;

        statusIndex.index =  index;
        statusIndex.sub_index = 0x02;

        items << Commander::SDOitem(index, 0x03, &speed) \
             << Commander::SDOitem(index, 0x04, &targetPosition);
    }
    else if (motorIndex == MOTOR_BIG_PIPET)
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

int Extract_DJ1::module_reset()
{
    int result = -1;
    quint16 index = MODULE_RESET_INDEX;

    Commander c(DevicePart::NODE_EXTRACT_DJ1);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Extract_DJ1::aspirate(const Extract_DJ1::AspiraterParams &param)
{
    int result = -1;
    quint16 index = DJ1_INDEX(ASPIRATE);

    QVector<Commander::SDOitem> items;
    int aspirateStrategy = param.aspirateStrategy;
    int preAspirateVolume = INT_FROM_OD_DATA(param.preAspirateVolume);
    int preSquirtVolume = INT_FROM_OD_DATA(param.preSquirtVolume);
    int preAspirateSpeed = INT_FROM_OD_DATA(param.preAspirateSpeed);
    int preAspirateAccDecNum = param.preAspirateAccDecNum;
    int aspirateVolume = INT_FROM_OD_DATA(param.aspirateVolume);
    int aspirateCompensateVolume = INT_FROM_OD_DATA(param.aspirateCompensateVolume);
    int aspirateSpeed = INT_FROM_OD_DATA(param.aspirateSpeed);
    int aspirateAccDecNum = param.aspirateAccDecNum;
    int squitTrimVolume = INT_FROM_OD_DATA(param.squitTrimVolume);
    int squitTrimSpeed = DOUBLE_FROM_OD_DATA(param.squitTrimSpeed);
    int squirtAccDecNum = param.squirtAccDecNum;
    int leaveLiquidZPosition = DOUBLE_FROM_OD_DATA(param.leaveLiquidZPosition);
    int leaveLiquidZSpeed = INT_FROM_OD_DATA(param.leaveLiquidZSpeed);

    int postAspirateVolume = INT_FROM_OD_DATA(param.postAspirateVolume);
    int postAspirateSpeed = INT_FROM_OD_DATA(param.postAspirateSpeed);
    int postAspirateAccDecNum = param.postAspirateAccDecNum;
    int aspirateZPreparePosition = INT_FROM_OD_DATA(param.aspirateZPreparePosition);
    int aspirateZPrepareSpeed = INT_FROM_OD_DATA(param.aspirateZPrepareSpeed);

    int aspirateZPositionStart = DOUBLE_FROM_OD_DATA(param.aspirateZPositionStart);
    int aspirateZPositionMiddle = DOUBLE_FROM_OD_DATA(param.aspirateZPositionMiddle);
    int aspirateZPositionEnd = DOUBLE_FROM_OD_DATA(param.aspirateZPositionEnd);
    int aspirateZSpeed = DOUBLE_FROM_OD_DATA(param.aspirateZSpeed);
    int sectionalArea = DOUBLE_FROM_OD_DATA(param.sectionalArea);
    int adjustment_k = DOUBLE_FROM_OD_DATA(param.adjustment_k);
    int adjustment_b = DOUBLE_FROM_OD_DATA(param.adjustment_b);
    int check_threshold = DOUBLE_FROM_OD_DATA(param.check_threshold);
    int finish_z_position = DOUBLE_FROM_OD_DATA(param.finish_z_position);
    int finish_z_speed = DOUBLE_FROM_OD_DATA(param.finish_z_speed);

    items << Commander::SDOitem(index, 0x03, &aspirateStrategy) \
          << Commander::SDOitem(index, 0x04, &preAspirateVolume) \
          << Commander::SDOitem(index, 0x05, &preSquirtVolume) \
          << Commander::SDOitem(index, 0x06, &preAspirateSpeed) \
          << Commander::SDOitem(index, 0x07, &preAspirateAccDecNum) \
          << Commander::SDOitem(index, 0x08, &aspirateVolume) \
          << Commander::SDOitem(index, 0x09, &aspirateCompensateVolume) \
          << Commander::SDOitem(index, 0x0A, &aspirateSpeed) \
          << Commander::SDOitem(index, 0x0B, &aspirateAccDecNum) \
          << Commander::SDOitem(index, 0x0C, &squitTrimVolume) \
          << Commander::SDOitem(index, 0x0D, &squitTrimSpeed) \
          << Commander::SDOitem(index, 0x0E, &squirtAccDecNum) \
          << Commander::SDOitem(index, 0x0F, &leaveLiquidZPosition) \
          << Commander::SDOitem(index, 0x10, &leaveLiquidZSpeed) \
          << Commander::SDOitem(index, 0x11, &postAspirateVolume) \
          << Commander::SDOitem(index, 0x12, &postAspirateSpeed) \
          << Commander::SDOitem(index, 0x13, &postAspirateAccDecNum) \
          << Commander::SDOitem(index, 0x14, &aspirateZPreparePosition) \
          << Commander::SDOitem(index, 0x15, &aspirateZPrepareSpeed) \
          << Commander::SDOitem(index, 0x16, &aspirateZPositionStart) \
          << Commander::SDOitem(index, 0x17, &aspirateZPositionMiddle) \
          << Commander::SDOitem(index, 0x18, &aspirateZPositionEnd) \
          << Commander::SDOitem(index, 0x19, &aspirateZSpeed) \
          << Commander::SDOitem(index, 0x1A, &sectionalArea) \
          << Commander::SDOitem(index, 0x1B, &adjustment_k) \
          << Commander::SDOitem(index, 0x1C, &adjustment_b) \
          << Commander::SDOitem(index, 0x1D, &check_threshold) \
          << Commander::SDOitem(index, 0x1E, &finish_z_position) \
          << Commander::SDOitem(index, 0x1F, &finish_z_speed);

    Commander c(DevicePart::NODE_EXTRACT_DJ1);
    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;

}

int Extract_DJ1::squirt(const Extract_DJ1::SquirtParams &param)
{
    int result = -1;
    quint16 index = DJ1_INDEX(SQUIRT);

    QVector<Commander::SDOitem> items;
    int squirtStrategy = param.squirtStrategy;
    int squirtVolume = INT_FROM_OD_DATA(param.squirtVolume);
    int squitCompensateVolume = INT_FROM_OD_DATA(param.squitCompensateVolume);
    int squirtSpeed = INT_FROM_OD_DATA(param.squirtSpeed);
    int squirtAccDecNum = param.squirtAccDecNum;

    int leaveLiquidZPosition = DOUBLE_FROM_OD_DATA(param.leaveLiquidZPosition);
    int leaveLiquidZSpeed = INT_FROM_OD_DATA(param.leaveLiquidZSpeed);
    int postAspirateVolume = INT_FROM_OD_DATA(param.postAspirateVolume);
    int postAspirateSpeed = INT_FROM_OD_DATA(param.postAspirateSpeed);
    int postAspirateAccDecNum = param.postAspirateAccDecNum;
    int squirtZPreparePosition = INT_FROM_OD_DATA(param.squirtZPreparePosition);
    int squirtZPrepareSpeed = INT_FROM_OD_DATA(param.squirtZPrepareSpeed);

    int squirtZPositionStart = DOUBLE_FROM_OD_DATA(param.squirtZPositionStart);
    int squirtZPositionMid = DOUBLE_FROM_OD_DATA(param.squirtZPositionMid);
    int squirtZPositionEnd = DOUBLE_FROM_OD_DATA(param.squirtZPositionEnd);
    int squirtZSpeed = DOUBLE_FROM_OD_DATA(param.squirtZSpeed);

    int sectionalArea = DOUBLE_FROM_OD_DATA(param.sectionalArea);
    int adjustment_k = DOUBLE_FROM_OD_DATA(param.adjustment_k);
    int adjustment_b = DOUBLE_FROM_OD_DATA(param.adjustment_b);
    int check_threshold = DOUBLE_FROM_OD_DATA(param.check_threshold);
    int finish_z_position = DOUBLE_FROM_OD_DATA(param.finish_z_position);
    int finish_z_speed = DOUBLE_FROM_OD_DATA(param.finish_z_speed);

    items << Commander::SDOitem(index, 0x03, &squirtStrategy) \
          << Commander::SDOitem(index, 0x04, &squirtVolume) \
          << Commander::SDOitem(index, 0x05, &squitCompensateVolume) \
          << Commander::SDOitem(index, 0x06, &squirtSpeed) \
          << Commander::SDOitem(index, 0x07, &squirtAccDecNum) \
          << Commander::SDOitem(index, 0x08, &leaveLiquidZPosition) \
          << Commander::SDOitem(index, 0x09, &leaveLiquidZSpeed) \
          << Commander::SDOitem(index, 0x0A, &postAspirateVolume) \
          << Commander::SDOitem(index, 0x0B, &postAspirateSpeed) \
          << Commander::SDOitem(index, 0x0C, &postAspirateAccDecNum) \
          << Commander::SDOitem(index, 0x0D, &squirtZPreparePosition) \
          << Commander::SDOitem(index, 0x0E, &squirtZPrepareSpeed) \
          << Commander::SDOitem(index, 0x0F, &squirtZPositionStart) \
          << Commander::SDOitem(index, 0x10, &squirtZPositionMid) \
          << Commander::SDOitem(index, 0x11, &squirtZPositionEnd) \
          << Commander::SDOitem(index, 0x12, &squirtZSpeed) \
          << Commander::SDOitem(index, 0x13, &sectionalArea) \
          << Commander::SDOitem(index, 0x14, &adjustment_k) \
          << Commander::SDOitem(index, 0x15, &adjustment_b) \
          << Commander::SDOitem(index, 0x16, &check_threshold) \
          << Commander::SDOitem(index, 0x17, &finish_z_position) \
          << Commander::SDOitem(index, 0x18, &finish_z_speed);

    Commander c(DevicePart::NODE_EXTRACT_DJ1);
    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Extract_DJ1::liquidMix(const Extract_DJ1::LiquidMixParams &param)
{
    int result = -1;
    quint16 index = DJ1_INDEX(LIQUID_MIX);

    QVector<Commander::SDOitem> items;
    int mixId = param.mixStrategy;
    int preAspirateVolume = INT_FROM_OD_DATA(param.preAspirateVolume);
    int preSquirtVolume = INT_FROM_OD_DATA(param.preSquirtVolume);
    int preAspirateSpeed = INT_FROM_OD_DATA(param.preAspirateSpeed);
    int preAspirateAccDecNum = param.preAspirateAccDecNum;
    int aspirateSpeed = INT_FROM_OD_DATA(param.aspirateSpeed);
    int squirtSpeed = INT_FROM_OD_DATA(param.squirtSpeed);
    int pipetAccDecNum = param.pipetAccDecNum;
    int firstAspirateVolume = INT_FROM_OD_DATA(param.firstAspirateVolume);
    int firstAspirateCompensateVolume = INT_FROM_OD_DATA(param.firstAspirateCompensateVolume);
    int squirtZPosition = INT_FROM_OD_DATA(param.squirtZPosition);
    int squirtVolume = INT_FROM_OD_DATA(param.squirtVolume);
    int aspirateZPosition = INT_FROM_OD_DATA(param.aspirateZPosition);
    int aspirateVolume = INT_FROM_OD_DATA(param.aspirateVolume);
    int mixTimes = DOUBLE_FROM_OD_DATA(param.mixTimes);
    int lastSquirtVolume = DOUBLE_FROM_OD_DATA(param.lastSquirtVolume);
    int lastSquitCompensateVolume = INT_FROM_OD_DATA(param.lastSquitCompensateVolume);
    int leaveLiquidZPosition = DOUBLE_FROM_OD_DATA(param.leaveLiquidZPosition);
    int leaveLiquidZSpeed = INT_FROM_OD_DATA(param.leaveLiquidZSpeed);
    int postAspirateVolume = INT_FROM_OD_DATA(param.postAspirateVolume);
    int postAspirateSpeed = INT_FROM_OD_DATA(param.postAspirateSpeed);
    int postAspirateAccDecNum = param.postAspirateAccDecNum;
    int mixZPosition_sta = DOUBLE_FROM_OD_DATA(param.mixZPosition_sta);
    int mixZPosition_mid = DOUBLE_FROM_OD_DATA(param.mixZPosition_mid);
    int mixZPosition_end = DOUBLE_FROM_OD_DATA(param.mixZPosition_end);
    int mixZSpeed = DOUBLE_FROM_OD_DATA(param.mixZSpeed);
    int sectionalArea = DOUBLE_FROM_OD_DATA(param.sectionalArea);
    int adjustment_k = DOUBLE_FROM_OD_DATA(param.adjustment_k);
    int adjustment_b = DOUBLE_FROM_OD_DATA(param.adjustment_b);
    int check_threshold = DOUBLE_FROM_OD_DATA(param.check_threshold);
    int finish_z_position = DOUBLE_FROM_OD_DATA(param.finish_z_position);
    int finish_z_speed = DOUBLE_FROM_OD_DATA(param.finish_z_speed);

    items << Commander::SDOitem(index, 0x03, &mixId) \
          << Commander::SDOitem(index, 0x04, &preAspirateVolume) \
          << Commander::SDOitem(index, 0x05, &preSquirtVolume) \
          << Commander::SDOitem(index, 0x06, &preAspirateSpeed) \
          << Commander::SDOitem(index, 0x07, &preAspirateAccDecNum) \
          << Commander::SDOitem(index, 0x08, &aspirateSpeed) \
          << Commander::SDOitem(index, 0x09, &squirtSpeed) \
          << Commander::SDOitem(index, 0x0A, &pipetAccDecNum) \
          << Commander::SDOitem(index, 0x0B, &firstAspirateVolume) \
          << Commander::SDOitem(index, 0x0C, &firstAspirateCompensateVolume) \
          << Commander::SDOitem(index, 0x0D, &squirtZPosition) \
          << Commander::SDOitem(index, 0x0E, &squirtVolume)   \
          << Commander::SDOitem(index, 0x0F, &aspirateZPosition)   \
          << Commander::SDOitem(index, 0x10, &aspirateVolume) \
          << Commander::SDOitem(index, 0x11, &mixTimes) \
          << Commander::SDOitem(index, 0x12, &lastSquirtVolume) \
          << Commander::SDOitem(index, 0x13, &lastSquitCompensateVolume) \
          << Commander::SDOitem(index, 0x14, &leaveLiquidZPosition) \
          << Commander::SDOitem(index, 0x15, &leaveLiquidZSpeed) \
          << Commander::SDOitem(index, 0x16, &postAspirateVolume) \
          << Commander::SDOitem(index, 0x17, &postAspirateSpeed) \
          << Commander::SDOitem(index, 0x18, &postAspirateAccDecNum) \
          << Commander::SDOitem(index, 0x19, &mixZPosition_sta) \
          << Commander::SDOitem(index, 0x1A, &mixZPosition_mid) \
          << Commander::SDOitem(index, 0x1B, &mixZPosition_end) \
          << Commander::SDOitem(index, 0x1C, &mixZSpeed) \
          << Commander::SDOitem(index, 0x1D, &sectionalArea) \
          << Commander::SDOitem(index, 0x1E, &adjustment_k) \
          << Commander::SDOitem(index, 0x1F, &adjustment_b) \
          << Commander::SDOitem(index, 0x20, &check_threshold) \
          << Commander::SDOitem(index, 0x21, &finish_z_position) \
          << Commander::SDOitem(index, 0x22, &finish_z_speed);

    Commander c(DevicePart::NODE_EXTRACT_DJ1);
    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Extract_DJ1::pause()
{

}

int Extract_DJ1::resume()
{

}

int Extract_DJ1::move(Extract_DJ1::RackMotionParams param)
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

    Commander c(DevicePart::NODE_EXTRACT_DJ1);
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

int Extract_DJ1::init_clean_start(float z_position)
{
    int result = -1;
    quint16 index = DJ1_INDEX(INIT_CLEAN);

    Commander c(DevicePart::NODE_EXTRACT_DJ1);

    QVector<Commander::SDOitem> items;
    items << Commander::SDOitem(index, 0x03, &z_position);

    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Extract_DJ1::remainder_check_clean_start(float z_position)
{
    int result = -1;
    quint16 index = DJ1_INDEX(REMAINDER_CHECK_CLEAN);
    Commander c(DevicePart::NODE_EXTRACT_DJ1);

    QVector<Commander::SDOitem> items;
    items << Commander::SDOitem(index, 0x03, &z_position);

    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Extract_DJ1::routine_clean_strart(int time, float z_position)
{

    int result = -1;
    quint16 index = DJ1_INDEX(ROUTINE_CLEAN);

    QVector<Commander::SDOitem> items;
    items << Commander::SDOitem(index, 0x03, &time)\
          << Commander::SDOitem(index, 0x04, &z_position) ;
    Commander c(DevicePart::NODE_EXTRACT_DJ1);

    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Extract_DJ1::cleaning_liqud_clean_start(int cleaning_liquid_clean_time, int pure_water_clean_time, float z_position)
{
    int result = -1;
    quint16 index = DJ1_INDEX(CLEANING_LIQUID_CLEAN);

    QVector<Commander::SDOitem> items;
    items << Commander::SDOitem(index, 0x03, &cleaning_liquid_clean_time) \
          << Commander::SDOitem(index, 0x04, &pure_water_clean_time) \
          << Commander::SDOitem(index, 0x05, &z_position);

    Commander c(DevicePart::NODE_EXTRACT_DJ1);

    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Extract_DJ1::open_little_inside_valve()
{
    int result = -1;
    quint16 index = DJ1_INDEX(OPEN_LITTLE_INSIDE_VALVE);

    Commander c(DevicePart::NODE_EXTRACT_DJ1);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Extract_DJ1::close_little_inside_valve()
{
    int result = -1;
    quint16 index = DJ1_INDEX(CLOSE_LITTLE_INSIDE_VALVE);

    Commander c(DevicePart::NODE_EXTRACT_DJ1);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Extract_DJ1::open_big_inside_valve()
{
    int result = -1;
    quint16 index = DJ1_INDEX(OPEN_BIG_INSIDE_VALVE);

    Commander c(DevicePart::NODE_EXTRACT_DJ1);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Extract_DJ1::close_big_inside_valve()
{
    int result = -1;
    quint16 index = DJ1_INDEX(CLOSE_BIG_INSIDE_VALVE);

    Commander c(DevicePart::NODE_EXTRACT_DJ1);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Extract_DJ1::open_outside_valve()
{
    int result = -1;
    quint16 index = DJ1_INDEX(OPEN_OUTSIDE_VALVE);

    Commander c(DevicePart::NODE_EXTRACT_DJ1);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Extract_DJ1::close_outside_valve()
{
    int result = -1;
    quint16 index = DJ1_INDEX(CLOSE_OUTSIDE_VALVE);

    Commander c(DevicePart::NODE_EXTRACT_DJ1);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Extract_DJ1::open_inside_pump()
{
    int result = -1;
    quint16 index = DJ1_INDEX(OPEN_INSIDE_PUMP);

    Commander c(DevicePart::NODE_EXTRACT_DJ1);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Extract_DJ1::close_inside_pump()
{
    int result = -1;
    quint16 index = DJ1_INDEX(CLOSE_INSIDE_PUMP);

    Commander c(DevicePart::NODE_EXTRACT_DJ1);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Extract_DJ1::open_outside_pump()
{
    int result = -1;
    quint16 index = DJ1_INDEX(OPEN_OUTSIDE_PUMP);

    Commander c(DevicePart::NODE_EXTRACT_DJ1);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Extract_DJ1::close_outside_pump()
{
    int result = -1;
    quint16 index = DJ1_INDEX(CLOSE_OUTSIDE_PUMP);

    Commander c(DevicePart::NODE_EXTRACT_DJ1);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Extract_DJ1::open_waste_pump()
{
    int result = -1;
    quint16 index = DJ1_INDEX(OPEN_WASTE_PUMP);

    Commander c(DevicePart::NODE_EXTRACT_DJ1);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Extract_DJ1::close_waste_pump()
{
    int result = -1;
    quint16 index = DJ1_INDEX(CLOSE_WASTE_PUMP);

    Commander c(DevicePart::NODE_EXTRACT_DJ1);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}
int Extract_DJ1::open_clean_pump()
{
    int result = -1;
    quint16 index = DJ1_INDEX(OPEN_CLEAN_PUMP);

    Commander c(DevicePart::NODE_EXTRACT_DJ1);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Extract_DJ1::close_clean_pump()
{
    int result = -1;
    quint16 index = DJ1_INDEX(CLOSE_CLEAN_PUMP);

    Commander c(DevicePart::NODE_EXTRACT_DJ1);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}
int Extract_DJ1::open_clean_valve()
{
    int result = -1;
    quint16 index = DJ1_INDEX(OPEN_CLEAN_VALVE);

    Commander c(DevicePart::NODE_EXTRACT_DJ1);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}
int Extract_DJ1::close_clean_valve()
{
    int result = -1;
    quint16 index = DJ1_INDEX(CLOSE_CLEAN_VALVE);

    Commander c(DevicePart::NODE_EXTRACT_DJ1);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}
int Extract_DJ1::relative_aspirate(float aspirate_speed, int acc_num, float aspirate_volume)
{
    int result = -1;
    quint16 index = DJ1_INDEX(RELATIVE_ASPIRATE);

    QVector<Commander::SDOitem> items;
    int asp_speed = DOUBLE_FROM_OD_DATA(aspirate_speed);
    int asp_volume = DOUBLE_FROM_OD_DATA(aspirate_volume);

    items   << Commander::SDOitem(index, 0x03, &asp_speed) \
            << Commander::SDOitem(index, 0x04, &acc_num)\
            << Commander::SDOitem(index, 0x05, &asp_volume);

    Commander c(DevicePart::NODE_EXTRACT_DJ1);
    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}

int Extract_DJ1::getParameter(Extract_DJ1::ExtractDj1Params &params)
{
    int result = -1;
    quint16 index = DJ1_INDEX(PARAMETER);

    Commander c(DevicePart::NODE_EXTRACT_DJ1);
    QVector<Commander::SDOitem> items;
    int little_pump_start_speed_low ;
    int little_pump_low_acc_low ;
    int little_pump_threshold_speed_low ;
    int little_pump_high_acc_low ;
    int little_pump_high_dec_low;
    int little_pump_low_dec_low;
    int little_pump_stop_speed_low;
    int little_pump_start_speed_mid;
    int little_pump_low_acc_mid;
    int little_pump_threshold_speed_mid;
    int little_pump_high_acc_mid;
    int little_pump_high_dec_mid;
    int little_pump_low_dec_mid;
    int little_pump_stop_speed_mid;
    int little_pump_start_speed_high;
    int little_pump_low_acc_high;
    int little_pump_threshold_speed_high;
    int little_pump_high_acc_high;
    int little_pump_high_dec_high;
    int little_pump_low_dec_high;
    int little_pump_stop_speed_high;
    int dj1_z_fly_position;
    int dj1_z_speed;
    int routine_clean_z_offset;
    int cleaning_liquid_z_offset;
    int pure_water_z_offset;
    int remainder_check_clean_z_offset;
    int init_clean_z_offset;
    int pump_return_difference_volume;
    int claw_z_speed;
    int routine_clean_aspirate_v1_volume;
    int routine_clean_aspirate_v2_volume;
    int routine_clean_aspirate_speed;
    int routine_clean_z_leave_speed;
    int routine_clean_aspirate_air_volume;
    int cleaning_clean_aspirate_v1_volume;
    int cleaning_clean_aspirate_v2_volume;
    int cleaning_clean_aspirate_speed;
    int cleaning_clean_z_leave_speed;
    int cleaning_clean_aspirate_air_volume;

    items << Commander::SDOitem(index, 0x03, &little_pump_start_speed_low) \
          << Commander::SDOitem(index, 0x04, &little_pump_low_acc_low) \
          << Commander::SDOitem(index, 0x05, &little_pump_threshold_speed_low) \
          << Commander::SDOitem(index, 0x06, &little_pump_high_acc_low) \
          << Commander::SDOitem(index, 0x07, &little_pump_high_dec_low) \
          << Commander::SDOitem(index, 0x08, &little_pump_low_dec_low) \
          << Commander::SDOitem(index, 0x09, &little_pump_stop_speed_low) \
          << Commander::SDOitem(index, 0x0A, &little_pump_start_speed_mid) \
          << Commander::SDOitem(index, 0x0B, &little_pump_low_acc_mid) \
          << Commander::SDOitem(index, 0x0C, &little_pump_threshold_speed_mid) \
          << Commander::SDOitem(index, 0x0D, &little_pump_high_acc_mid) \
          << Commander::SDOitem(index, 0x0E, &little_pump_high_dec_mid) \
          << Commander::SDOitem(index, 0x0F, &little_pump_low_dec_mid) \
          << Commander::SDOitem(index, 0x10, &little_pump_stop_speed_mid) \
          << Commander::SDOitem(index, 0x11, &little_pump_start_speed_high) \
          << Commander::SDOitem(index, 0x12, &little_pump_low_acc_high) \
          << Commander::SDOitem(index, 0x13, &little_pump_threshold_speed_high)\
          << Commander::SDOitem(index, 0x14, &little_pump_high_acc_high) \
          << Commander::SDOitem(index, 0x15, &little_pump_high_dec_high) \
          << Commander::SDOitem(index, 0x16, &little_pump_low_dec_high) \
          << Commander::SDOitem(index, 0x17, &little_pump_stop_speed_high) \
          << Commander::SDOitem(index, 0x18, &dj1_z_fly_position) \
          << Commander::SDOitem(index, 0x19, &dj1_z_speed) \
          << Commander::SDOitem(index, 0x1A, &routine_clean_z_offset) \
          << Commander::SDOitem(index, 0x1B, &cleaning_liquid_z_offset) \
          << Commander::SDOitem(index, 0x1C, &pure_water_z_offset) \
          << Commander::SDOitem(index, 0x1D, &remainder_check_clean_z_offset) \
          << Commander::SDOitem(index, 0x1E, &init_clean_z_offset) \
          << Commander::SDOitem(index, 0x1F, &pump_return_difference_volume)\
          << Commander::SDOitem(index, 0x20, &claw_z_speed) \
          << Commander::SDOitem(index, 0x21, &routine_clean_aspirate_v1_volume) \
          << Commander::SDOitem(index, 0x22, &routine_clean_aspirate_v2_volume) \
          << Commander::SDOitem(index, 0x23, &routine_clean_aspirate_speed) \
          << Commander::SDOitem(index, 0x24, &routine_clean_z_leave_speed) \
          << Commander::SDOitem(index, 0x25, &routine_clean_aspirate_air_volume) \
          << Commander::SDOitem(index, 0x26, &cleaning_clean_aspirate_v1_volume) \
          << Commander::SDOitem(index, 0x27, &cleaning_clean_aspirate_v2_volume) \
          << Commander::SDOitem(index, 0x28, &cleaning_clean_aspirate_speed) \
          << Commander::SDOitem(index, 0x29, &cleaning_clean_z_leave_speed) \
          << Commander::SDOitem(index, 0x2A, &cleaning_clean_aspirate_air_volume);

    result = c.readRemoteData(items);

    if (!result) {
        params.little_pump_start_speed_low = DOUBLE_TO_OD_DATA(little_pump_start_speed_low);
        params.little_pump_low_acc_low = DOUBLE_TO_OD_DATA(little_pump_low_acc_low);
        params.little_pump_threshold_speed_low = DOUBLE_TO_OD_DATA(little_pump_threshold_speed_low);
        params.little_pump_high_acc_low = DOUBLE_TO_OD_DATA(little_pump_high_acc_low);
        params.little_pump_high_dec_low = DOUBLE_TO_OD_DATA(little_pump_high_dec_low);
        params.little_pump_low_dec_low = INT_TO_OD_DATA(little_pump_low_dec_low);
        params.little_pump_stop_speed_low = INT_TO_OD_DATA(little_pump_stop_speed_low);
        params.little_pump_start_speed_mid = INT_TO_OD_DATA(little_pump_start_speed_mid);
        params.little_pump_low_acc_mid = INT_TO_OD_DATA(little_pump_low_acc_mid);
        params.little_pump_threshold_speed_mid = DOUBLE_TO_OD_DATA(little_pump_threshold_speed_mid);
        params.little_pump_high_acc_mid = DOUBLE_TO_OD_DATA(little_pump_high_acc_mid);
        params.little_pump_high_dec_mid = DOUBLE_TO_OD_DATA(little_pump_high_dec_mid);
        params.little_pump_low_dec_mid = INT_TO_OD_DATA(little_pump_low_dec_mid);
        params.little_pump_stop_speed_mid = INT_TO_OD_DATA(little_pump_stop_speed_mid);
        params.little_pump_start_speed_high = INT_TO_OD_DATA(little_pump_start_speed_high);
        params.little_pump_low_acc_high = INT_TO_OD_DATA(little_pump_low_acc_high);
        params.little_pump_threshold_speed_high = DOUBLE_TO_OD_DATA(little_pump_threshold_speed_high);
        params.little_pump_high_acc_high = DOUBLE_TO_OD_DATA(little_pump_high_acc_high);
        params.little_pump_high_dec_high = DOUBLE_TO_OD_DATA(little_pump_high_dec_high);
        params.little_pump_low_dec_high = INT_TO_OD_DATA(little_pump_low_dec_high);
        params.little_pump_stop_speed_high = INT_TO_OD_DATA(little_pump_stop_speed_high);
        params.dj1_z_fly_position = INT_TO_OD_DATA(dj1_z_fly_position);
        params.dj1_z_speed = INT_TO_OD_DATA(dj1_z_speed);
        params.routine_clean_z_offset = INT_TO_OD_DATA(routine_clean_z_offset);
        params.cleaning_liquid_z_offset = INT_TO_OD_DATA(cleaning_liquid_z_offset);
        params.pure_water_z_offset = INT_TO_OD_DATA(pure_water_z_offset);
        params.remainder_check_clean_z_offset = INT_TO_OD_DATA(remainder_check_clean_z_offset);
        params.init_clean_z_offset = INT_TO_OD_DATA(init_clean_z_offset);
        params.pump_return_difference_volume = INT_TO_OD_DATA(pump_return_difference_volume);
        params.claw_z_speed = INT_TO_OD_DATA(claw_z_speed);
        params.routine_clean_aspirate_v1_volume = INT_TO_OD_DATA(routine_clean_aspirate_v1_volume);
        params.routine_clean_aspirate_v2_volume = INT_TO_OD_DATA(routine_clean_aspirate_v2_volume);
        params.routine_clean_aspirate_speed = INT_TO_OD_DATA(routine_clean_aspirate_speed);
        params.routine_clean_z_leave_speed = INT_TO_OD_DATA(routine_clean_z_leave_speed);
        params.routine_clean_aspirate_air_volume = INT_TO_OD_DATA(routine_clean_aspirate_air_volume);
        params.cleaning_clean_aspirate_v1_volume = INT_TO_OD_DATA(cleaning_clean_aspirate_v1_volume);
        params.cleaning_clean_aspirate_v2_volume = INT_TO_OD_DATA(cleaning_clean_aspirate_v2_volume);
        params.cleaning_clean_aspirate_speed = INT_TO_OD_DATA(cleaning_clean_aspirate_speed);
        params.cleaning_clean_z_leave_speed = INT_TO_OD_DATA(cleaning_clean_z_leave_speed);
        params.cleaning_clean_aspirate_air_volume = INT_TO_OD_DATA(cleaning_clean_aspirate_air_volume);
    }

    return result;
}

int Extract_DJ1::setParameter(const Extract_DJ1::ExtractDj1Params &params)
{
    int result = -1;
    quint16 index = DJ1_INDEX(PARAMETER);

    Commander c(DevicePart::NODE_EXTRACT_DJ1);
    QVector<Commander::SDOitem> items;

    int little_pump_start_speed_low = DOUBLE_FROM_OD_DATA(params.little_pump_start_speed_low);
    int little_pump_low_acc_low = DOUBLE_FROM_OD_DATA(params.little_pump_low_acc_low);
    int little_pump_threshold_speed_low = DOUBLE_FROM_OD_DATA(params.little_pump_threshold_speed_low);
    int little_pump_high_acc_low = DOUBLE_FROM_OD_DATA(params.little_pump_high_acc_low);
    int little_pump_high_dec_low = DOUBLE_FROM_OD_DATA(params.little_pump_high_dec_low);
    int little_pump_low_dec_low = DOUBLE_FROM_OD_DATA(params.little_pump_low_dec_low);
    int little_pump_stop_speed_low = DOUBLE_FROM_OD_DATA(params.little_pump_stop_speed_low);
    int little_pump_start_speed_mid = DOUBLE_FROM_OD_DATA(params.little_pump_start_speed_mid);
    int little_pump_low_acc_mid = DOUBLE_FROM_OD_DATA(params.little_pump_low_acc_mid);
    int little_pump_threshold_speed_mid = DOUBLE_FROM_OD_DATA(params.little_pump_threshold_speed_mid);
    int little_pump_high_acc_mid = DOUBLE_FROM_OD_DATA(params.little_pump_high_acc_mid);
    int little_pump_high_dec_mid = DOUBLE_FROM_OD_DATA(params.little_pump_high_dec_mid);
    int little_pump_low_dec_mid = DOUBLE_FROM_OD_DATA(params.little_pump_low_dec_mid);
    int little_pump_stop_speed_mid = DOUBLE_FROM_OD_DATA(params.little_pump_stop_speed_mid);
    int little_pump_start_speed_high = DOUBLE_FROM_OD_DATA(params.little_pump_start_speed_high);
    int little_pump_low_acc_high = DOUBLE_FROM_OD_DATA(params.little_pump_low_acc_high);
    int little_pump_threshold_speed_high = DOUBLE_FROM_OD_DATA(params.little_pump_threshold_speed_high);
    int little_pump_high_acc_high = DOUBLE_FROM_OD_DATA(params.little_pump_high_acc_high);
    int little_pump_high_dec_high = DOUBLE_FROM_OD_DATA(params.little_pump_high_dec_high);
    int little_pump_low_dec_high = DOUBLE_FROM_OD_DATA(params.little_pump_low_dec_high);
    int little_pump_stop_speed_high = DOUBLE_FROM_OD_DATA(params.little_pump_stop_speed_high);
    int dj1_z_fly_position = DOUBLE_FROM_OD_DATA(params.dj1_z_fly_position);
    int dj1_z_speed = DOUBLE_FROM_OD_DATA(params.dj1_z_speed);
    int routine_clean_z_offset = DOUBLE_FROM_OD_DATA(params.routine_clean_z_offset);
    int cleaning_liquid_z_offset = DOUBLE_FROM_OD_DATA(params.cleaning_liquid_z_offset);
    int pure_water_z_offset = DOUBLE_FROM_OD_DATA(params.pure_water_z_offset);
    int remainder_check_clean_z_offset = DOUBLE_FROM_OD_DATA(params.remainder_check_clean_z_offset);
    int init_clean_z_offset = DOUBLE_FROM_OD_DATA(params.init_clean_z_offset);
    int pump_return_difference_volume = DOUBLE_FROM_OD_DATA(params.pump_return_difference_volume);
    int claw_z_speed = DOUBLE_FROM_OD_DATA(params.claw_z_speed);
    int routine_clean_aspirate_v1_volume = DOUBLE_FROM_OD_DATA(params.routine_clean_aspirate_v1_volume);
    int routine_clean_aspirate_v2_volume = DOUBLE_FROM_OD_DATA(params.routine_clean_aspirate_v2_volume);
    int routine_clean_aspirate_speed = DOUBLE_FROM_OD_DATA(params.routine_clean_aspirate_speed);
    int routine_clean_z_leave_speed = DOUBLE_FROM_OD_DATA(params.routine_clean_z_leave_speed);
    int routine_clean_aspirate_air_volume = DOUBLE_FROM_OD_DATA(params.routine_clean_aspirate_air_volume);
    int cleaning_clean_aspirate_v1_volume = DOUBLE_FROM_OD_DATA(params.cleaning_clean_aspirate_v1_volume);
    int cleaning_clean_aspirate_v2_volume = DOUBLE_FROM_OD_DATA(params.cleaning_clean_aspirate_v2_volume);
    int cleaning_clean_aspirate_speed = DOUBLE_FROM_OD_DATA(params.cleaning_clean_aspirate_speed);
    int cleaning_clean_z_leave_speed = DOUBLE_FROM_OD_DATA(params.cleaning_clean_z_leave_speed);
    int cleaning_clean_aspirate_air_volume = DOUBLE_FROM_OD_DATA(params.cleaning_clean_aspirate_air_volume);

    items << Commander::SDOitem(index, 0x03, &little_pump_start_speed_low) \
          << Commander::SDOitem(index, 0x04, &little_pump_low_acc_low) \
          << Commander::SDOitem(index, 0x05, &little_pump_threshold_speed_low) \
          << Commander::SDOitem(index, 0x06, &little_pump_high_acc_low) \
          << Commander::SDOitem(index, 0x07, &little_pump_high_dec_low) \
          << Commander::SDOitem(index, 0x08, &little_pump_low_dec_low) \
          << Commander::SDOitem(index, 0x09, &little_pump_stop_speed_low) \
          << Commander::SDOitem(index, 0x0A, &little_pump_start_speed_mid) \
          << Commander::SDOitem(index, 0x0B, &little_pump_low_acc_mid) \
          << Commander::SDOitem(index, 0x0C, &little_pump_threshold_speed_mid) \
          << Commander::SDOitem(index, 0x0D, &little_pump_high_acc_mid) \
          << Commander::SDOitem(index, 0x0E, &little_pump_high_dec_mid) \
          << Commander::SDOitem(index, 0x0F, &little_pump_low_dec_mid) \
          << Commander::SDOitem(index, 0x10, &little_pump_stop_speed_mid) \
          << Commander::SDOitem(index, 0x11, &little_pump_start_speed_high) \
          << Commander::SDOitem(index, 0x12, &little_pump_low_acc_high) \
          << Commander::SDOitem(index, 0x13, &little_pump_threshold_speed_high) \
          << Commander::SDOitem(index, 0x14, &little_pump_high_acc_high) \
          << Commander::SDOitem(index, 0x15, &little_pump_high_dec_high) \
          << Commander::SDOitem(index, 0x16, &little_pump_low_dec_high) \
          << Commander::SDOitem(index, 0x17, &little_pump_stop_speed_high) \
          << Commander::SDOitem(index, 0x18, &dj1_z_fly_position) \
          << Commander::SDOitem(index, 0x19, &dj1_z_speed) \
          << Commander::SDOitem(index, 0x1A, &routine_clean_z_offset) \
          << Commander::SDOitem(index, 0x1B, &cleaning_liquid_z_offset) \
          << Commander::SDOitem(index, 0x1C, &pure_water_z_offset) \
          << Commander::SDOitem(index, 0x1D, &remainder_check_clean_z_offset) \
          << Commander::SDOitem(index, 0x1E, &init_clean_z_offset) \
          << Commander::SDOitem(index, 0x1F, &pump_return_difference_volume)\
          << Commander::SDOitem(index, 0x20, &claw_z_speed) \
          << Commander::SDOitem(index, 0x21, &routine_clean_aspirate_v1_volume) \
          << Commander::SDOitem(index, 0x22, &routine_clean_aspirate_v2_volume) \
          << Commander::SDOitem(index, 0x23, &routine_clean_aspirate_speed) \
          << Commander::SDOitem(index, 0x24, &routine_clean_z_leave_speed) \
          << Commander::SDOitem(index, 0x25, &routine_clean_aspirate_air_volume) \
          << Commander::SDOitem(index, 0x26, &cleaning_clean_aspirate_v1_volume) \
          << Commander::SDOitem(index, 0x27, &cleaning_clean_aspirate_v2_volume) \
          << Commander::SDOitem(index, 0x28, &cleaning_clean_aspirate_speed) \
          << Commander::SDOitem(index, 0x29, &cleaning_clean_z_leave_speed) \
          << Commander::SDOitem(index, 0x2A, &cleaning_clean_aspirate_air_volume) ;

    result = c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;

}
