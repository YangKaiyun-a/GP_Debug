#include "dp1.h"
#include "Driver/commander.h"

Dp1::Dp1()
{

}

Dp1 *Dp1::instance()
{
    static Dp1 Dp1_ins;
    return &Dp1_ins;
}

quint16 Dp1::select_motor_reset_index(int motor_id)
{
    quint16 index;
    switch (motor_id) {
    case 2:
        index = DP1_INDEX(MOTOR_RESET);
        break;
    case 3:
        index = CLAW_INDEX(CLAW_MOTOR_RESET);
        break;
    }

    return index;
}

quint16 Dp1::select_motor_move_index(int motor_id)
{
    quint16 index;
    switch (motor_id) {
    case 2:
        index = DP1_INDEX(MOTOR_MOVE);
        break;
    case 3:
        index = CLAW_INDEX(CLAW_MOTOR_MOVE);
        break;
    }

    return index;
}

int Dp1::motorReset(int motorIndex)
{
    int result = -1;
    Commander::SDOitem controlItem;
    Commander::SDOIndex statusItem;

    quint16 index = select_motor_reset_index(motorIndex);

    Commander c(DevicePart::NODE_DP1_JAW);
    if(motorIndex == MOTOR_Dp1_Z)
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

int Dp1::motorMove(int motorIndex, const DevicePart::MotorMotionParams &param)
{
    int result = -1;
    quint16 index = select_motor_move_index(motorIndex);

    Commander c(DevicePart::NODE_DP1_JAW);

    QVector<Commander::SDOitem> items;
    int targetPosition = DOUBLE_FROM_OD_DATA(param.targetPosition);
    int speed = DOUBLE_FROM_OD_DATA(param.speed);

    Commander::SDOitem controlItem;
    Commander::SDOIndex statusIndex;

    if (motorIndex == MOTOR_Dp1_Z)
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
        controlItem.index.sub_index = 0x01;
        controlItem.data = ACTION_CONTROL;

        statusIndex.index =  index;
        statusIndex.sub_index = 0x02;

        items << Commander::SDOitem(index, 0x03, &speed) \
             << Commander::SDOitem(index, 0x04, &targetPosition);
    }
    c.writeRemoteData(items);

    result = c.exec(controlItem, statusIndex);

    return result;
}

int Dp1::module_reset()
{
    int result = -1;
    quint16 index = MODULE_RESET_INDEX;

    Commander c(DevicePart::NODE_DP1_JAW);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Dp1::getTip(float z_pos)
{
    int result = -1;
    quint16 index = DP1_INDEX(GET_TIP);

    Commander c(DevicePart::NODE_DP1_JAW);

    QVector<Commander::SDOitem> items;
    int pos = DOUBLE_FROM_OD_DATA(z_pos);

    items << Commander::SDOitem(index, 0x03, &pos);
    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Dp1::removeTip(float z_pos)
{
    int result = -1;
    quint16 index = DP1_INDEX(REMOVE_TIP);

    Commander c(DevicePart::NODE_DP1_JAW);

    QVector<Commander::SDOitem> items;
    int pos = DOUBLE_FROM_OD_DATA(z_pos);

    items << Commander::SDOitem(index, 0x03, &pos);
    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Dp1::aspirate(const Dp1::AspiraterParams &param)
{
    int result = -1;
    quint16 index = DP1_INDEX(ASPIRATE);

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

    Commander c(DevicePart::NODE_DP1_JAW);
    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Dp1::squirt(const Dp1::SquirtParams &param)
{
    int result = -1;
    quint16 index = DP1_INDEX(SQUIRT);

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

    Commander c(DevicePart::NODE_DP1_JAW);
    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Dp1::liquidMix(const Dp1::LiquidMixParams &param)
{
    int result = -1;
    quint16 index = DP1_INDEX(LIQUID_MIX);

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

    Commander c(DevicePart::NODE_DP1_JAW);
    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Dp1::pause()
{

}

int Dp1::resume()
{

}

int Dp1::aspirate_waste(float z_pos)
{

}

void Dp1::test(Dp1::testparam param, int is_pause)
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
        Dp1_Xy::RackMotionParams xy_param;
        xy_param.posNumber = 1;
        xy_param.speed = 250;
        xy_param.positions.clear();
        xy_param.positions << param.x_stop_positon << param.y_stop_positon;
        Dp1_Xy::instance()->move(xy_param);

        DevicePart::MotorMotionParams z_param;

        z_param.speed =85;
        z_param.targetPosition = param.z_stop_positon;

        Dp1::instance()->motorMove(MOTOR_Dp1_Z, z_param);

        z_param.speed =85;
        z_param.targetPosition = param.z_start_positon;

        Dp1::instance()->motorMove(MOTOR_Dp1_Z, z_param);

        DevicePart::MotorMotionParams jaw_z_param;

        jaw_z_param.speed =85;
        jaw_z_param.targetPosition = param.jaw_z_stop_positon;

        Dp1::instance()->motorMove(MOTOR_CLAW_Z, jaw_z_param);

        jaw_z_param.speed =85;
        jaw_z_param.targetPosition = param.jaw_z_start_positon;

        Dp1::instance()->motorMove(MOTOR_CLAW_Z, jaw_z_param);

        xy_param.posNumber = 1;
        xy_param.speed = 250;
        xy_param.positions.clear();
        xy_param.positions << param.x_start_positon << param.y_start_positon;
        Dp1_Xy::instance()->move(xy_param);
    }

    m_test = false;
}

void Dp1::stoptest()
{
    m_test = false;
}

float Dp1::get_distance()
{
    quint16 index = DP1_INDEX(DISTANCE);
    Commander c(DevicePart::NODE_DP1_JAW);

    int distance = 0;

    QVector<Commander::SDOitem> items;
    items << Commander::SDOitem(index, 0x03, &distance);

     c.readRemoteData(items);

     return  DOUBLE_TO_OD_DATA(distance);

}

int Dp1::claw_reset()
{
    int result = -1;
    quint16 index = CLAW_INDEX(CLAW_MOTOR_RESET);

    Commander c(DevicePart::NODE_DP1_JAW);

    Commander::SDOitem controlItem;
    Commander::SDOIndex statusIndex;

    controlItem.index.index = index;
    controlItem.index.sub_index = 0x01;
    controlItem.data = ACTION_CONTROL;

    statusIndex.index = index;
    statusIndex.sub_index = 0x02;

    result = c.exec(controlItem, statusIndex);

    return result;
}

int Dp1::glab(float glabWidth)
{
    int result = -1;
    quint16 index = CLAW_INDEX(CLAW_GLAB);

    Commander c(DevicePart::NODE_DP1_JAW);

    QVector<Commander::SDOitem> items;
    int width = DOUBLE_FROM_OD_DATA(glabWidth);

    items << Commander::SDOitem(index, 0x03, &width);
    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Dp1::release(float releaseWidth)
{
    int result = -1;
    quint16 index = CLAW_INDEX(CLAW_RELEASE);

    Commander c(DevicePart::NODE_DP1_JAW);

    QVector<Commander::SDOitem> items;
    int width = DOUBLE_FROM_OD_DATA(releaseWidth);
    items << Commander::SDOitem(index, 0x03, &width);

    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}

int Dp1::toZ(float zPosition, float openWidth)
{
    int result = -1;
    quint16 index = CLAW_INDEX(CLAW_TO_Z);
    Commander c(DevicePart::NODE_DP1_JAW);

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

int Dp1::toZGlab(float zPosition, float openWidth,int force)
{
    int result = -1;
    quint16 index = CLAW_INDEX(CLAW_TO_Z_GLAB);

    QVector<Commander::SDOitem> items;
    int zPos = DOUBLE_FROM_OD_DATA(zPosition);
    int widthOpen = DOUBLE_FROM_OD_DATA(openWidth);

    items << Commander::SDOitem(index, 0x03, &zPos) \
          << Commander::SDOitem(index, 0x04, &widthOpen)\
          << Commander::SDOitem(index, 0x07, &force);

    Commander c(DevicePart::NODE_DP1_JAW);
    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}

int Dp1::toZRelease(float zPosition, float releaseWidth)
{
    int result = -1;
    quint16 index = CLAW_INDEX(CLAW_TO_Z_RELEASE);

    QVector<Commander::SDOitem> items;
    int zPos = DOUBLE_FROM_OD_DATA(zPosition);
    int width = DOUBLE_FROM_OD_DATA(releaseWidth);

    items << Commander::SDOitem(index, 0x03, &zPos) << Commander::SDOitem(index, 0x04, &width);

    Commander c(DevicePart::NODE_DP1_JAW);
    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}

int Dp1::claw_spin(float angle, float speed)
{
    int result = -1;
    quint16 index = CLAW_INDEX(CLAW_SPIN);

    QVector<Commander::SDOitem> items;
    int spin_angle = DOUBLE_FROM_OD_DATA(angle);
    int spin_speed = DOUBLE_FROM_OD_DATA(speed);

    items << Commander::SDOitem(index, 0x03, &spin_speed) << Commander::SDOitem(index, 0x04, &spin_angle);

    Commander c(DevicePart::NODE_DP1_JAW);
    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}

int Dp1::claw_2_reset()
{
    int result = -1;
    quint16 index = CLAW_INDEX(CLAW_2_RESET);

    Commander c(DevicePart::NODE_DP1_JAW);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}

int Dp1::claw_2_glab(float glabWidth)
{
    int result = -1;
    quint16 index = CLAW_INDEX(CLAW_2_GLAB);

    Commander c(DevicePart::NODE_DP1_JAW);

    QVector<Commander::SDOitem> items;
    int width = DOUBLE_FROM_OD_DATA(glabWidth);
    items << Commander::SDOitem(index, 0x03, &width);

    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}

int Dp1::claw_2_release(float releaseWidth)
{
    int result = -1;
    quint16 index = CLAW_INDEX(CLAW_2_RELEASE);

    Commander c(DevicePart::NODE_DP1_JAW);

    QVector<Commander::SDOitem> items;
    int width = DOUBLE_FROM_OD_DATA(releaseWidth);
    items << Commander::SDOitem(index, 0x03, &width);

    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}

int Dp1::adp_reset()
{
    int result = -1;
    quint16 index = DP1_INDEX(ADP_RESET);

    Commander c(DevicePart::NODE_DP1_JAW);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}

int Dp1::adp_remove_tip()
{
    int result = -1;
    quint16 index = DP1_INDEX(ADP_REMOVE_TIP);

    Commander c(DevicePart::NODE_DP1_JAW);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}

int Dp1::adp_aspirate(float volume, float speed)
{
    int result = -1;
    quint16 index = DP1_INDEX(ADP_ASPIRATE);

    Commander c(DevicePart::NODE_DP1_JAW);

    QVector<Commander::SDOitem> items;
    int aspirate_volume = DOUBLE_FROM_OD_DATA(volume);
    int aspirate_speed = DOUBLE_FROM_OD_DATA(speed);
    items << Commander::SDOitem(index, 0x03, &aspirate_volume)      \
            <<Commander::SDOitem(index, 0x04, &aspirate_speed);

    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}

int Dp1::adp_squirt(float volume, float speed)
{
    int result = -1;
    quint16 index = DP1_INDEX(ADP_SQUIRT);

    Commander c(DevicePart::NODE_DP1_JAW);

    QVector<Commander::SDOitem> items;
    int squirt_volume = DOUBLE_FROM_OD_DATA(volume);
    int squirt_speed = DOUBLE_FROM_OD_DATA(speed);
    items << Commander::SDOitem(index, 0x03, &squirt_volume)      \
            <<Commander::SDOitem(index, 0x04, &squirt_speed);

    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}

int Dp1::adp_sesarch_liquid()
{
    int result = -1;
    quint16 index = DP1_INDEX(ADP_SEARCH_LIQUID);

    Commander c(DevicePart::NODE_DP1_JAW);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}

int Dp1::open_cup(float z_position)
{
    int result = -1;
    quint16 index = CLAW_INDEX(OPEN_CUP);

    Commander c(DevicePart::NODE_DP1_JAW);

    QVector<Commander::SDOitem> items;
    int position = DOUBLE_FROM_OD_DATA(z_position);

    items << Commander::SDOitem(index, 0x03, &position);

    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}

int Dp1::close_cup(float z_position)
{
    int result = -1;
    quint16 index = CLAW_INDEX(CLOSE_CUP);

    Commander c(DevicePart::NODE_DP1_JAW);

    QVector<Commander::SDOitem> items;
    int position = DOUBLE_FROM_OD_DATA(z_position);

    items << Commander::SDOitem(index, 0x03, &position);

    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}



