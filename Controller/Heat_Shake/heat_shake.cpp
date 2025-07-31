#include "heat_shake.h"
#include "Driver/commander.h"

#include <QThread>
#include <QDebug>


Heat_Shake::Heat_Shake()
{

}

Heat_Shake *Heat_Shake::instance()
{
    static Heat_Shake heat_shake_ins;
    return &heat_shake_ins;
}

Heat_Shake *Heat_Shake::read_temp_instance()
{
    static Heat_Shake read_temp_ins;
    return &read_temp_ins;
}

int Heat_Shake::moduleReset(int moduleIndex)
{
    int result = -1;
    quint16 index = MODULE_RESET_INDEX;

    Commander c(moduleIndex);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Heat_Shake::motorReset(int motorIndex)
{
    int result = -1;
    quint16 index = HEATSHAKE_INDEX(MOTOR_RESET);

    Commander c(motorIndex);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

//吸磁电机运动
int Heat_Shake::motorMove(int motorIndex, const DevicePart::MotorMotionParams &param)
{
    int result = -1;
    quint16 index = HEATSHAKE_INDEX(MOTOR_MOVE);

    Commander c(motorIndex);

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

//开始加热
int Heat_Shake::start_heat(int moduleIndex, float target_temperature)
{
    int result = -1;
    int target_tem;

    quint16 index = HEATSHAKE_INDEX(HEAT_START);

    Commander c(moduleIndex);

    target_tem = DOUBLE_FROM_OD_DATA(target_temperature);

    QVector<Commander::SDOitem> items;
    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    items << Commander::SDOitem(index, 0x03, &target_tem);

    c.writeRemoteData(items);

    result = c.exec(controlItem, statusItem);

    return result;
}

//停止加热
int Heat_Shake::stop_heat(int moduleIndex)
{
    int result = -1;

    quint16 index = HEATSHAKE_INDEX(HEAT_STOP);

    Commander c(moduleIndex);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Heat_Shake::start_shake(int moduleIndex, int target_rpm)
{
    int result = -1;
    int rpm;

    quint16 index = HEATSHAKE_INDEX(SHAKE_START);

    Commander c(moduleIndex);

    rpm = target_rpm;

    QVector<Commander::SDOitem> items;
    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    items << Commander::SDOitem(index, 0x03, &rpm);

    c.writeRemoteData(items);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Heat_Shake::stop_shake(int moduleIndex)
{
    int result = -1;

    quint16 index = HEATSHAKE_INDEX(SHAKE_STOP);

    Commander c(moduleIndex);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Heat_Shake::magnet_out(int moduleIndex)
{
    int result = -1;

    quint16 index = HEATSHAKE_INDEX(MAGNET_OUT);

    Commander c(moduleIndex);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Heat_Shake::magnet_in(int moduleIndex)
{
    int result = -1;

    quint16 index = HEATSHAKE_INDEX(MAGNET_IN);

    Commander c(moduleIndex);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

void Heat_Shake::stop_readTemp()
{
    m_bReadTemp = false;
}

void Heat_Shake::test(int moduleIndex, Heat_Shake::testparam param, int is_pause)
{
    if(m_test)
    {
        return;
    }

    m_test = true;

    if(is_pause == 0)
    {
        current_loop_num = 0;
    }

    DevicePart::MotorMotionParams start_params;
    DevicePart::MotorMotionParams next_params;
    DevicePart::MotorMotionParams thrid_params;
    DevicePart::MotorMotionParams fourth_params;

    start_params.targetPosition = param.start_position;
    start_params.speed = param.cycle_speed;
    start_params.sleep_time = param.start_sleep_time;

    next_params.targetPosition = param.next_position;
    next_params.speed = param.cycle_speed;
    next_params.sleep_time = param.next_sleep_time;

    thrid_params.targetPosition = param.third_position;
    thrid_params.speed = param.cycle_speed;
    thrid_params.sleep_time = param.third_sleep_time;

    fourth_params.targetPosition = param.fourth_position;
    fourth_params.speed = param.cycle_speed;
    fourth_params.sleep_time = param.fourth_sleep_time;

    while(m_test && (current_loop_num < param.loop_num))
    {
        current_loop_num++;
        motorMove(moduleIndex, start_params);
        QThread::sleep(start_params.sleep_time);    //阻塞延时

        motorMove(moduleIndex, next_params);
        QThread::sleep(next_params.sleep_time);

        motorMove(moduleIndex, thrid_params);
        QThread::sleep(thrid_params.sleep_time);

        motorMove(moduleIndex, fourth_params);
        QThread::sleep(fourth_params.sleep_time);

        //电机复位
        motorReset(moduleIndex);

        Q_EMIT testCount(current_loop_num);

        QThread::sleep(1);
    }

    m_test = false;
}

void Heat_Shake::stoptest()
{
    m_test = false;

    qDebug() << QThread::currentThread();
}

int Heat_Shake::getParameter(int moduleIndex, Heat_Shake::HeatShakeParams &params)
{
    int result = -1;

    quint16 index = HEATSHAKE_INDEX(PARAMETER);

    Commander c(moduleIndex);
    QVector<Commander::SDOitem> items;

    int magnet_out_pos = -1;
    int magnet_in_pos = -1;
    int magnet_speed = -1;
    int magnet_in_position_1 = -1;
    int magnet_in_position_2 = -1;
    int magnet_in_position_3 = -1;
    int magnet_in_position_4 = -1;
    int heat_k = -1;
    int heat_b = -1;

    items << Commander::SDOitem(index, 0x03, &magnet_out_pos) \
          << Commander::SDOitem(index, 0x04, &magnet_in_pos) \
          << Commander::SDOitem(index, 0x05, &magnet_speed)\
          << Commander::SDOitem(index, 0x07, &magnet_in_position_1)\
          << Commander::SDOitem(index, 0x08, &magnet_in_position_2)\
          << Commander::SDOitem(index, 0x09, &magnet_in_position_3)\
          << Commander::SDOitem(index, 0x0A, &magnet_in_position_4)\
          << Commander::SDOitem(index, 0x0B, &heat_k)\
          << Commander::SDOitem(index, 0x0C, &heat_b);

    result = c.readRemoteData(items);

    if (!result) {
        params.magnet_out_position = DOUBLE_TO_OD_DATA(magnet_out_pos);
        params.magnet_in_position = DOUBLE_TO_OD_DATA(magnet_in_pos);
        params.magnet_speed = DOUBLE_TO_OD_DATA(magnet_speed);
        params.magnet_in_position_1 = DOUBLE_TO_OD_DATA(magnet_in_position_1);
        params.magnet_in_position_2 = DOUBLE_TO_OD_DATA(magnet_in_position_2);
        params.magnet_in_position_3 = DOUBLE_TO_OD_DATA(magnet_in_position_3);
        params.magnet_in_position_4 = DOUBLE_TO_OD_DATA(magnet_in_position_4);
        params.heat_k = DOUBLE_TO_OD_DATA(heat_k);
        params.heat_b = DOUBLE_TO_OD_DATA(heat_b);
    }

    return result;
}

int Heat_Shake::setParameter(int moduleIndex, const Heat_Shake::HeatShakeParams &params)
{
    int result = -1;
    quint16 index = HEATSHAKE_INDEX(PARAMETER);

    Commander c(moduleIndex);
    QVector<Commander::SDOitem> items;

    int magnet_speed = DOUBLE_FROM_OD_DATA(params.magnet_speed);
    int magnet_in_position_1 = DOUBLE_FROM_OD_DATA(params.magnet_in_position_1);
    int magnet_in_position_3 = DOUBLE_FROM_OD_DATA(params.magnet_in_position_3);
    int magnet_in_position_4 = DOUBLE_FROM_OD_DATA(params.magnet_in_position_4);
    int heat_k = DOUBLE_FROM_OD_DATA(params.heat_k);
    int heat_b = DOUBLE_FROM_OD_DATA(params.heat_b);

    items << Commander::SDOitem(index, 0x05, &magnet_speed)\
          << Commander::SDOitem(index, 0x07, &magnet_in_position_1)\
          << Commander::SDOitem(index, 0x09, &magnet_in_position_3)\
          << Commander::SDOitem(index, 0x0A, &magnet_in_position_4)\
          << Commander::SDOitem(index, 0x0B, &heat_k)\
          << Commander::SDOitem(index, 0x0C, &heat_b);

    result = c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

void Heat_Shake::readTemp(int moduleIndex)
{
    if(m_bReadTemp)
    {
        return;
    }

    m_bReadTemp = true;

    quint16 index = HEATSHAKE_INDEX(TEMP_CURRENT);

    Commander c(moduleIndex);

    int temp1 = 0;
    QList<float> temps;
    QVector<Commander::SDOitem> items;
    items << Commander::SDOitem(index, 0x01, &temp1);

    while(m_bReadTemp)
    {
        QThread::msleep(1000);
        temps.clear();
        c.readRemoteData(items);
        temps << DOUBLE_TO_OD_DATA(temp1);
        Q_EMIT sigTemp(moduleIndex, temps);
    }
}
