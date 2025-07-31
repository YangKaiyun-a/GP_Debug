#include "dp8.h"
#include "Driver/commander.h"
#include <QDebug>

Dp8::Dp8()
{

}

Dp8 *Dp8::instance()
{
    static Dp8 Dp8_ins;
    return &Dp8_ins;
}

quint16 Dp8::select_motor_reset_index(int motor_id)
{
    quint16 index;
    switch (motor_id) {
    case 4:
        index = DP8_INDEX(MOTOR_RESET);
        break;
    case 5:
        index = DP8_INDEX(MOTOR_RESET);
        break;
    case 6:
        index = DP8_INDEX(MOTOR_RESET);
        break;
    case 7:
        index = CLAW_INDEX(CLAW_MOTOR_RESET);
        break;
    case 8:
        index = CLAW_INDEX(CLAW_MOTOR_RESET);
        break;
    }

    return index;
}

quint16 Dp8::select_motor_move_index(int motor_id)
{
    quint16 index;
    switch (motor_id) {
    case 4:
        index = DP8_INDEX(MOTOR_MOVE);
        break;
    case 5:
        index = DP8_INDEX(MOTOR_MOVE);
        break;
    case 6:
        index = DP8_INDEX(MOTOR_MOVE);
        break;
    case 7:
        index = CLAW_INDEX(CLAW_MOTOR_MOVE);
        break;
    case 8:
        index = CLAW_INDEX(CLAW_MOTOR_MOVE);
        break;
    }

    return index;
}

int Dp8::motorReset(int motorIndex)
{
    int result = -1;
    Commander::SDOitem controlItem;
    Commander::SDOIndex statusItem;

    quint16 index = select_motor_reset_index(motorIndex);

    Commander c(DevicePart::NODE_DP8);
    if(motorIndex == MOTOR_DP8_Z)
    {
        controlItem.index.index = index;
        controlItem.index.sub_index = 0x01;
        controlItem.data = ACTION_CONTROL;

        statusItem.index =  index;
        statusItem.sub_index = 0x02;
    }
    else if(motorIndex == MOTOR_DP8_PIPET)
    {
        controlItem.index.index = index;
        controlItem.index.sub_index = 0x07;
        controlItem.data = ACTION_CONTROL;

        statusItem.index =  index;
        statusItem.sub_index = 0x08;
    }
    else if(motorIndex == MOTOR_DP8_TIP)
    {
        controlItem.index.index = index;
        controlItem.index.sub_index = 0x04;
        controlItem.data = ACTION_CONTROL;

        statusItem.index =  index;
        statusItem.sub_index = 0x05;
    }
    else if(motorIndex == MOTOR_CLAW)
    {
        controlItem.index.index = index;
        controlItem.index.sub_index = 0x01;
        controlItem.data = ACTION_CONTROL;

        statusItem.index =  index;
        statusItem.sub_index = 0x02;
    }
    else if(motorIndex == MOTOR_CLAW_Z)
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

int Dp8::motorMove(int motorIndex, const DevicePart::MotorMotionParams &param)
{
    int result = -1;
    quint16 index = select_motor_move_index(motorIndex);

    Commander c(DevicePart::NODE_DP8);

    QVector<Commander::SDOitem> items;
    int targetPosition = DOUBLE_FROM_OD_DATA(param.targetPosition);
    int speed = DOUBLE_FROM_OD_DATA(param.speed);

    Commander::SDOitem controlItem;
    Commander::SDOIndex statusIndex;

    if (motorIndex == MOTOR_DP8_Z)
    {
        controlItem.index.index = index;
        controlItem.index.sub_index = 0x01;
        controlItem.data = ACTION_CONTROL;

        statusIndex.index =  index;
        statusIndex.sub_index = 0x02;

        items << Commander::SDOitem(index, 0x03, &speed) \
              << Commander::SDOitem(index, 0x04, &targetPosition);
    }
    else if (motorIndex == MOTOR_DP8_PIPET)
    {
        controlItem.index.index = index;
        controlItem.index.sub_index = 0x0B;
        controlItem.data = ACTION_CONTROL;

        statusIndex.index =  index;
        statusIndex.sub_index = 0x0C;

        items << Commander::SDOitem(index, 0x0D, &speed) \
              << Commander::SDOitem(index, 0x0E, &targetPosition);
    }
    else if (motorIndex == MOTOR_DP8_TIP)
    {
        controlItem.index.index = index;
        controlItem.index.sub_index = 0x06;
        controlItem.data = ACTION_CONTROL;

        statusIndex.index =  index;
        statusIndex.sub_index = 0x07;

        items << Commander::SDOitem(index, 0x08, &speed) \
              << Commander::SDOitem(index, 0x09, &targetPosition);
    }
    else if (motorIndex == MOTOR_CLAW)
    {
        controlItem.index.index = index;
        controlItem.index.sub_index = 0x01;
        controlItem.data = ACTION_CONTROL;

        statusIndex.index =  index;
        statusIndex.sub_index = 0x02;

        items << Commander::SDOitem(index, 0x03, &speed) \
             << Commander::SDOitem(index, 0x04, &targetPosition);
    }
    else if (motorIndex == MOTOR_CLAW_Z)
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

int Dp8::module_reset()
{
    int result = -1;
    quint16 index = MODULE_RESET_INDEX;

    Commander c(DevicePart::NODE_DP8);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Dp8::getTip(float z_pos)
{
    int result = -1;
    quint16 index = DP8_INDEX(GET_TIP);

    Commander c(DevicePart::NODE_DP8);

    QVector<Commander::SDOitem> items;
    int pos = DOUBLE_FROM_OD_DATA(z_pos);

    items << Commander::SDOitem(index, 0x03, &pos);
    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Dp8::removeTip(float z_pos)
{
    int result = -1;
    quint16 index = DP8_INDEX(REMOVE_TIP);

    Commander c(DevicePart::NODE_DP8);

    QVector<Commander::SDOitem> items;
    int pos = DOUBLE_FROM_OD_DATA(z_pos);

    items << Commander::SDOitem(index, 0x03, &pos);
    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Dp8::aspirate(const Dp8::AspiraterParams &param)
{
    int result = -1;
    quint16 index = DP8_INDEX(ASPIRATE);

    QVector<Commander::SDOitem> items;
    int aspirateStrategy = param.aspirateStrategy;
    int preAspirateVolume = INT_FROM_OD_DATA(param.preAspirateVolume);
    int aspirateVolume1 = INT_FROM_OD_DATA(param.aspirateVolume1);
    int aspirateSpeed1 = INT_FROM_OD_DATA(param.aspirateSpeed1);

    int wait_time = DOUBLE_FROM_OD_DATA(param.wait_time);

    int aspirateVolume2 = INT_FROM_OD_DATA(param.aspirateVolume2);
    int aspirateCompensateVolume = INT_FROM_OD_DATA(param.aspirateCompensateVolume);
    int aspirateSpeed2 = INT_FROM_OD_DATA(param.aspirateSpeed2);
    int underLiquidWaitTime = DOUBLE_FROM_OD_DATA(param.underLiquidWaitTime);

    int squitTrimVolume = INT_FROM_OD_DATA(param.squitTrimVolume);
    int squitTrimSpeed = INT_FROM_OD_DATA(param.squitTrimSpeed);
    int squirt_wait_time = DOUBLE_FROM_OD_DATA(param.squirt_wait_time);

    int leaveLiquidZPosition = DOUBLE_FROM_OD_DATA(param.leaveLiquidZPosition);
    int leaveLiquidZSpeed = INT_FROM_OD_DATA(param.leaveLiquidZSpeed);
    int postAspirateVolume = INT_FROM_OD_DATA(param.postAspirateVolume);
    int postAspirateSpeed = INT_FROM_OD_DATA(param.postAspirateSpeed);
    int aspirateZPositionStart = DOUBLE_FROM_OD_DATA(param.aspirateZPositionStart);
    int aspirateZPositionMiddle = DOUBLE_FROM_OD_DATA(param.aspirateZPositionMiddle);
    int aspirateZPositionEnd = DOUBLE_FROM_OD_DATA(param.aspirateZPositionEnd);
    int sectionalArea = DOUBLE_FROM_OD_DATA(param.sectionalArea);
    int flux = param.flux;
    int through_film_symbol = param.through_film_symbol;
    int use_claw_symbol = param.use_claw_symbol;
    int through_film_z_position = DOUBLE_FROM_OD_DATA(param.through_film_z_position);
    int through_film_z_offset = DOUBLE_FROM_OD_DATA(param.through_film_z_offset);
    int adjustment_k = DOUBLE_FROM_OD_DATA(param.adjustment_k);
    int adjustment_b = DOUBLE_FROM_OD_DATA(param.adjustment_b);
    int check_channel = param.check_channel;
    int check_threshold = DOUBLE_FROM_OD_DATA(param.check_threshold);
    int finish_z_position = DOUBLE_FROM_OD_DATA(param.finish_z_position);
    int cross_expand_symbol = param.cross_expand_symbol;

    items << Commander::SDOitem(index, 0x03, &aspirateStrategy) \
          << Commander::SDOitem(index, 0x04, &preAspirateVolume) \
          << Commander::SDOitem(index, 0x05, &aspirateVolume1) \
          << Commander::SDOitem(index, 0x06, &aspirateSpeed1) \
          << Commander::SDOitem(index, 0x07, &wait_time) \
          << Commander::SDOitem(index, 0x08, &aspirateVolume2) \
          << Commander::SDOitem(index, 0x09, &aspirateCompensateVolume) \
          << Commander::SDOitem(index, 0x0A, &aspirateSpeed2) \
          << Commander::SDOitem(index, 0x0B, &underLiquidWaitTime) \
          << Commander::SDOitem(index, 0x0C, &squitTrimVolume) \
          << Commander::SDOitem(index, 0x0D, &squitTrimSpeed) \
          << Commander::SDOitem(index, 0x0E, &squirt_wait_time) \
          << Commander::SDOitem(index, 0x0F, &leaveLiquidZPosition) \
          << Commander::SDOitem(index, 0x10, &leaveLiquidZSpeed) \
          << Commander::SDOitem(index, 0x11, &postAspirateVolume) \
          << Commander::SDOitem(index, 0x12, &postAspirateSpeed) \
          << Commander::SDOitem(index, 0x13, &aspirateZPositionStart) \
          << Commander::SDOitem(index, 0x14, &aspirateZPositionMiddle) \
          << Commander::SDOitem(index, 0x15, &aspirateZPositionEnd) \
          << Commander::SDOitem(index, 0x16, &sectionalArea) \
          << Commander::SDOitem(index, 0x17, &flux) \
          << Commander::SDOitem(index, 0x18, &adjustment_k) \
          << Commander::SDOitem(index, 0x19, &adjustment_b) \
          << Commander::SDOitem(index, 0x1A, &through_film_symbol) \
          << Commander::SDOitem(index, 0x1B, &use_claw_symbol) \
          << Commander::SDOitem(index, 0x1C, &through_film_z_position) \
          << Commander::SDOitem(index, 0x1D, &through_film_z_offset) \
          << Commander::SDOitem(index, 0x1E, &check_channel) \
          << Commander::SDOitem(index, 0x1F, &check_threshold) \
          << Commander::SDOitem(index, 0x20, &finish_z_position)\
          << Commander::SDOitem(index, 0x21, &cross_expand_symbol);

    Commander c(DevicePart::NODE_DP8);
    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Dp8::squirt(const Dp8::SquirtParams &param)
{
    int result = -1;
    quint16 index = DP8_INDEX(SQUIRT);

    QVector<Commander::SDOitem> items;
    int squirtStrategy = param.squirtStrategy;
    int squirtVolume_1 = INT_FROM_OD_DATA(param.squirtVolume_1);
    int squirtSpeed_1 = INT_FROM_OD_DATA(param.squirtSpeed_1);
    int wait_time = INT_FROM_OD_DATA(param.wait_time);
    int squirtVolume_2 = INT_FROM_OD_DATA(param.squirtVolume_2);
    int squirtSpeed_2 = INT_FROM_OD_DATA(param.squirtSpeed_2);
    int squitCompensateVolume = INT_FROM_OD_DATA(param.squitCompensateVolume);

    int underLiquidWaitTime = DOUBLE_FROM_OD_DATA(param.underLiquidWaitTime);
    int leaveLiquidZPosition = DOUBLE_FROM_OD_DATA(param.leaveLiquidZPosition);
    int leaveLiquidZSpeed = INT_FROM_OD_DATA(param.leaveLiquidZSpeed);
    int postAspirateVolume = INT_FROM_OD_DATA(param.postAspirateVolume);
    int postAspirateSpeed = INT_FROM_OD_DATA(param.postAspirateSpeed);
    int squirtZPositionStart = DOUBLE_FROM_OD_DATA(param.squirtZPositionStart);
    int squirtZPositionMid = DOUBLE_FROM_OD_DATA(param.squirtZPositionMid);
    int squirtZPositionEnd = DOUBLE_FROM_OD_DATA(param.squirtZPositionEnd);
    int sectionalArea = DOUBLE_FROM_OD_DATA(param.sectionalArea);
    int flux = param.flux;
    int adjustment_k = DOUBLE_FROM_OD_DATA(param.adjustment_k);
    int adjustment_b = DOUBLE_FROM_OD_DATA(param.adjustment_b);
    int check_channel = param.check_channel;
    int check_threshold = DOUBLE_FROM_OD_DATA(param.check_threshold);

    items << Commander::SDOitem(index, 0x03, &squirtStrategy) \
          << Commander::SDOitem(index, 0x04, &squirtVolume_1) \
          << Commander::SDOitem(index, 0x05, &squirtSpeed_1) \
          << Commander::SDOitem(index, 0x06, &wait_time) \
          << Commander::SDOitem(index, 0x07, &squirtVolume_2) \
          << Commander::SDOitem(index, 0x08, &squitCompensateVolume) \
          << Commander::SDOitem(index, 0x09, &squirtSpeed_2) \
          << Commander::SDOitem(index, 0x0A, &underLiquidWaitTime) \
          << Commander::SDOitem(index, 0x0B, &leaveLiquidZPosition) \
          << Commander::SDOitem(index, 0x0C, &leaveLiquidZSpeed) \
          << Commander::SDOitem(index, 0x0D, &postAspirateVolume) \
          << Commander::SDOitem(index, 0x0E, &postAspirateSpeed) \
          << Commander::SDOitem(index, 0x0F, &squirtZPositionStart) \
          << Commander::SDOitem(index, 0x10, &squirtZPositionMid) \
          << Commander::SDOitem(index, 0x11, &squirtZPositionEnd) \
          << Commander::SDOitem(index, 0x12, &sectionalArea) \
          << Commander::SDOitem(index, 0x13, &flux) \
          << Commander::SDOitem(index, 0x14, &adjustment_k) \
          << Commander::SDOitem(index, 0x15, &adjustment_b) \
          << Commander::SDOitem(index, 0x16, &check_channel) \
          << Commander::SDOitem(index, 0x17, &check_threshold);

    Commander c(DevicePart::NODE_DP8);
    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Dp8::liquidMix(const Dp8::LiquidMixParams &param)
{
    int result = -1;
    quint16 index = DP8_INDEX(LIQUID_MIX);

    QVector<Commander::SDOitem> items;
    int mixId = param.mixStrategy;
    int preAspirateVolume = INT_FROM_OD_DATA(param.preAspirateVolume);
    int firstAspirateVolume = INT_FROM_OD_DATA(param.firstAspirateVolume);
    int firstAspirateSpeed = INT_FROM_OD_DATA(param.firstAspirateSpeed);
    int mixVolume = INT_FROM_OD_DATA(param.mixVolume);
    int mixTimes = param.mixTimes;
    int mixSpeed = INT_FROM_OD_DATA(param.mixSpeed);
    int lastSquirtVolume_1 = INT_FROM_OD_DATA(param.lastSquirtVolume_1);
    int lastSquirtSpeed_1 = INT_FROM_OD_DATA(param.lastSquirtSpeed_1);
    int wait_time = INT_FROM_OD_DATA(param.wait_time);
    int lastSquirtVolume_2 = INT_FROM_OD_DATA(param.lastSquirtVolume_2);
    int lastSquirtSpeed_2 = INT_FROM_OD_DATA(param.lastSquirtSpeed_2);
    int squitCompensateVolume = INT_FROM_OD_DATA(param.squitCompensateVolume);
    int underLiquidWaitTime = DOUBLE_FROM_OD_DATA(param.underLiquidWaitTime);
    int leaveLiquidZPosition = DOUBLE_FROM_OD_DATA(param.leaveLiquidZPosition);
    int leaveLiquidZSpeed = INT_FROM_OD_DATA(param.leaveLiquidZSpeed);
    int postLiquidZPosition = DOUBLE_FROM_OD_DATA(param.postLiquidZPosition);
    int postLiquidZSpeed = INT_FROM_OD_DATA(param.postLiquidZSpeed);
    int mixZPosition_sta = DOUBLE_FROM_OD_DATA(param.mixZPosition_sta);
    int mixZPosition_mid = DOUBLE_FROM_OD_DATA(param.mixZPosition_mid);
    int mixZPosition_end = DOUBLE_FROM_OD_DATA(param.mixZPosition_end);
    int sectionalArea = DOUBLE_FROM_OD_DATA(param.sectionalArea);
    int flux = param.flux;
    int adjustment_k = DOUBLE_FROM_OD_DATA(param.adjustment_k);
    int adjustment_b = DOUBLE_FROM_OD_DATA(param.adjustment_b);
    int check_channel = param.check_channel;
    int check_threshold = DOUBLE_FROM_OD_DATA(param.check_threshold);

    items << Commander::SDOitem(index, 0x03, &mixId) \
          << Commander::SDOitem(index, 0x04, &preAspirateVolume) \
          << Commander::SDOitem(index, 0x05, &firstAspirateVolume) \
          << Commander::SDOitem(index, 0x06, &firstAspirateSpeed) \
          << Commander::SDOitem(index, 0x07, &mixVolume) \
          << Commander::SDOitem(index, 0x08, &mixTimes) \
          << Commander::SDOitem(index, 0x09, &mixSpeed) \
          << Commander::SDOitem(index, 0x0A, &lastSquirtVolume_1) \
          << Commander::SDOitem(index, 0x0B, &lastSquirtSpeed_1) \
          << Commander::SDOitem(index, 0x0C, &wait_time) \
          << Commander::SDOitem(index, 0x0D, &lastSquirtVolume_2) \
          << Commander::SDOitem(index, 0x0E, &squitCompensateVolume)   \
          << Commander::SDOitem(index, 0x0F, &lastSquirtSpeed_2)   \
          << Commander::SDOitem(index, 0x10, &underLiquidWaitTime) \
          << Commander::SDOitem(index, 0x11, &leaveLiquidZPosition) \
          << Commander::SDOitem(index, 0x12, &leaveLiquidZSpeed) \
          << Commander::SDOitem(index, 0x13, &postLiquidZPosition) \
          << Commander::SDOitem(index, 0x14, &postLiquidZSpeed) \
          << Commander::SDOitem(index, 0x15, &mixZPosition_sta) \
          << Commander::SDOitem(index, 0x16, &mixZPosition_mid) \
          << Commander::SDOitem(index, 0x17, &mixZPosition_end) \
          << Commander::SDOitem(index, 0x18, &sectionalArea) \
          << Commander::SDOitem(index, 0x19, &flux) \
          << Commander::SDOitem(index, 0x1A, &adjustment_k) \
          << Commander::SDOitem(index, 0x1B, &adjustment_b) \
          << Commander::SDOitem(index, 0x1C, &check_channel) \
          << Commander::SDOitem(index, 0x1D, &check_threshold) ;

    Commander c(DevicePart::NODE_DP8);
    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Dp8::pause()
{

}

int Dp8::resume()
{

}

int Dp8::aspirate_waste(float z_pos)
{

}

void Dp8::test(Dp8::testparam param, int is_pause)
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
        Dp8_Xy::RackMotionParams xy_param;
        xy_param.posNumber = 1;
        xy_param.speed = 250;
        xy_param.positions.clear();
        xy_param.positions << param.x_stop_positon << param.y_stop_positon;
        Dp8_Xy::instance()->move(xy_param);

        DevicePart::MotorMotionParams z_param;

        z_param.speed =85;
        z_param.targetPosition = param.z_stop_positon;

        Dp8::instance()->motorMove(MOTOR_DP8_Z, z_param);

        z_param.speed =85;
        z_param.targetPosition = param.z_start_positon;

        Dp8::instance()->motorMove(MOTOR_DP8_Z, z_param);

        DevicePart::MotorMotionParams jaw_z_param;

        jaw_z_param.speed =85;
        jaw_z_param.targetPosition = param.jaw_z_stop_positon;

        Dp8::instance()->motorMove(MOTOR_CLAW_Z, jaw_z_param);

        jaw_z_param.speed =85;
        jaw_z_param.targetPosition = param.jaw_z_start_positon;

        Dp8::instance()->motorMove(MOTOR_CLAW_Z, jaw_z_param);

        DevicePart::MotorMotionParams tip_z_param;

        tip_z_param.speed =85;
        tip_z_param.targetPosition = param.tip_stop_positon;

        Dp8::instance()->motorMove(MOTOR_DP8_TIP, tip_z_param);

        tip_z_param.speed =85;
        tip_z_param.targetPosition = param.tip_start_positon;

        Dp8::instance()->motorMove(MOTOR_DP8_TIP, tip_z_param);

        xy_param.posNumber = 1;
        xy_param.speed = 250;
        xy_param.positions.clear();
        xy_param.positions << param.x_start_positon << param.y_start_positon;
        Dp8_Xy::instance()->move(xy_param);
    }

    m_test = false;
}

void Dp8::stoptest()
{
    m_test = false;
}

int Dp8::glab(float glabWidth)
{
    int result = -1;
    quint16 index = CLAW_INDEX(CLAW_GLAB);

    Commander c(DevicePart::NODE_DP8);

    QVector<Commander::SDOitem> items;
    int width = DOUBLE_FROM_OD_DATA(glabWidth);

    items << Commander::SDOitem(index, 0x03, &width);
    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Dp8::release(float releaseWidth)
{
    int result = -1;
    quint16 index = CLAW_INDEX(CLAW_RELEASE);

    Commander c(DevicePart::NODE_DP8);

    QVector<Commander::SDOitem> items;
    int width = DOUBLE_FROM_OD_DATA(releaseWidth);
    items << Commander::SDOitem(index, 0x03, &width);

    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}

int Dp8::toZ(float zPosition, float openWidth)
{
    int result = -1;
    quint16 index = CLAW_INDEX(CLAW_TO_Z);
    Commander c(DevicePart::NODE_DP8);

    QVector<Commander::SDOitem> items;
    int zPos = DOUBLE_FROM_OD_DATA(zPosition);
    int width = DOUBLE_FROM_OD_DATA(openWidth);

    items << Commander::SDOitem(index, 0x03, &zPos) << Commander::SDOitem(index, 0x04, &width);
    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}

int Dp8::toZGlab(float zPosition, float openWidth, float closeWidth, float finish_z_position)
{
    int result = -1;
    quint16 index = CLAW_INDEX(CLAW_TO_Z_GLAB);

    QVector<Commander::SDOitem> items;
    int zPos = DOUBLE_FROM_OD_DATA(zPosition);
    int widthOpen = DOUBLE_FROM_OD_DATA(openWidth);
    int CloseOpen = DOUBLE_FROM_OD_DATA(closeWidth);
    int finish_positiopn = DOUBLE_FROM_OD_DATA(finish_z_position);

    items << Commander::SDOitem(index, 0x03, &zPos) \
          << Commander::SDOitem(index, 0x04, &widthOpen) \
          << Commander::SDOitem(index, 0x05, &CloseOpen)  \
          << Commander::SDOitem(index, 0x06, &finish_positiopn) ;

    Commander c(DevicePart::NODE_DP8);
    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}

int Dp8::toZRelease(float zPosition, float releaseWidth)
{
    int result = -1;
    quint16 index = CLAW_INDEX(CLAW_TO_Z_RELEASE);

    QVector<Commander::SDOitem> items;
    int zPos = DOUBLE_FROM_OD_DATA(zPosition);
    int width = DOUBLE_FROM_OD_DATA(releaseWidth);

    items << Commander::SDOitem(index, 0x03, &zPos) << Commander::SDOitem(index, 0x04, &width);

    Commander c(DevicePart::NODE_DP8);
    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}

int Dp8::relative_aspirate(float aspirate_speed, float aspirate_volume)
{
    int result = -1;
    quint16 index = DP8_INDEX(RELATIVE_ASPIRATE);

    QVector<Commander::SDOitem> items;
    int asp_speed = DOUBLE_FROM_OD_DATA(aspirate_speed);
    int asp_volume = DOUBLE_FROM_OD_DATA(aspirate_volume);

    items   << Commander::SDOitem(index, 0x03, &asp_speed) \
            << Commander::SDOitem(index, 0x04, &asp_volume);

    Commander c(DevicePart::NODE_DP8);
    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}

int Dp8::getParameter(Dp8::Dp8_jawParams &params)
{
    int result = -1;

    quint16 index = DP8_INDEX(PARAMETER);

    Commander c(DevicePart::NODE_DP8);
    QVector<Commander::SDOitem> items;

    int zFlyPosition ;
    int jawZDownPosition;
    int z_speed;
    int claw_z_speed ;
    int claw_z_zero_offset ;

    items << Commander::SDOitem(index, 0x03, &zFlyPosition) \
          << Commander::SDOitem(index, 0x04, &jawZDownPosition) \
          << Commander::SDOitem(index, 0x05, &z_speed) \
          << Commander::SDOitem(index, 0x06, &claw_z_speed) \
          << Commander::SDOitem(index, 0x07, &claw_z_zero_offset);

    result = c.readRemoteData(items);

    if (!result) {
        params.zFlyPosition = DOUBLE_TO_OD_DATA(zFlyPosition);
        params.jawZDownPosition = DOUBLE_TO_OD_DATA(jawZDownPosition);
        params.z_speed = DOUBLE_TO_OD_DATA(z_speed);
        params.claw_z_speed = DOUBLE_TO_OD_DATA(claw_z_speed);
        params.claw_z_zero_offset = DOUBLE_TO_OD_DATA(claw_z_zero_offset);

    }

    return result;
}

int Dp8::setParameter(const Dp8::Dp8_jawParams &params)
{
    int result = -1;

    quint16 index = DP8_INDEX(PARAMETER);

    Commander c(DevicePart::NODE_DP8);
    QVector<Commander::SDOitem> items;

    int zFlyPosition = DOUBLE_FROM_OD_DATA(params.zFlyPosition);
    int jawZDownPosition = DOUBLE_FROM_OD_DATA(params.jawZDownPosition);
    int z_speed = DOUBLE_FROM_OD_DATA(params.z_speed);
    int claw_z_speed = DOUBLE_FROM_OD_DATA(params.claw_z_speed);
    int claw_z_zero_offset = DOUBLE_FROM_OD_DATA(params.claw_z_zero_offset);

    items << Commander::SDOitem(index, 0x03, &zFlyPosition) \
          << Commander::SDOitem(index, 0x04, &jawZDownPosition) \
          << Commander::SDOitem(index, 0x05, &z_speed)\
          << Commander::SDOitem(index, 0x06, &claw_z_speed)\
          << Commander::SDOitem(index, 0x07, &claw_z_zero_offset);

    result = c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}
