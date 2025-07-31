#include "air_pressure.h"
#include "Driver/commander.h"
#include <QtDebug>
#include <QThread>

air_pressure::air_pressure()
{

}

air_pressure *air_pressure::instance()
{
    static air_pressure air_pressure_ins;
    return &air_pressure_ins;
}

//12号板读取参数
int air_pressure::air_getParameter(air_pressure_Params &params)
{
    int result = -1;
    quint16 index = AIR_INDEX(PARAMETER);

    Commander c(DevicePart::NODE_ENVIRONMENT_MONITOR);
    QVector<Commander::SDOitem> items;

    int pcrmix_pressure_target;
    int pipet_pressure_target;
    int pcrmix_pressure_offset;
    int pipet_pressure_offset;
    int pwm_dytu_ratio;
    int pcrmix_pressure_duty;
    int pipet_pressure_duty;

    items << Commander::SDOitem(index, 0x03, &pcrmix_pressure_target) \
          << Commander::SDOitem(index, 0x04, &pipet_pressure_target) \
          << Commander::SDOitem(index, 0x06, &pcrmix_pressure_offset) \
          << Commander::SDOitem(index, 0x07, &pipet_pressure_offset) \
          << Commander::SDOitem(index, 0x09, &pwm_dytu_ratio) \
          << Commander::SDOitem(index, 0x0A, &pcrmix_pressure_duty) \
          << Commander::SDOitem(index, 0x0B, &pipet_pressure_duty);

    result = c.readRemoteData(items);

    if (!result) {
        params.pcrmix_pressure_target = DOUBLE_TO_OD_DATA(pcrmix_pressure_target);
        params.pipet_pressure_target = DOUBLE_TO_OD_DATA(pipet_pressure_target);
        params.pcrmix_pressure_offset = DOUBLE_TO_OD_DATA(pcrmix_pressure_offset);
        params.pipet_pressure_offset = DOUBLE_TO_OD_DATA(pipet_pressure_offset);
        params.pwm_dytu_ratio = DOUBLE_TO_OD_DATA(pwm_dytu_ratio);
        params.pcrmix_pressure_duty = DOUBLE_TO_OD_DATA(pcrmix_pressure_duty);
        params.pipet_pressure_duty = DOUBLE_TO_OD_DATA(pipet_pressure_duty);
    }

    return result;
}

//17号板读取参数
int air_pressure::xz_getParameter(air_pressure_Params &params)
{
    int result = -1;
    quint16 index = AIR_INDEX(PARAMETER);

    Commander c(DevicePart::NODE_XZ_CLAW);
    QVector<Commander::SDOitem> items;

    int qpcr_pressure_target;
    int qpcr_pressure_offset;
    int pwm_dytu_ratio;
    int qpcr_pressure_duty;

    items << Commander::SDOitem(index, 0x05, &qpcr_pressure_target) \
          << Commander::SDOitem(index, 0x08, &qpcr_pressure_offset) \
          << Commander::SDOitem(index, 0x09, &pwm_dytu_ratio)
          << Commander::SDOitem(index, 0x0C, &qpcr_pressure_duty);

    result = c.readRemoteData(items);

    if (!result) {
        params.qpcr_pressure_target = DOUBLE_TO_OD_DATA(qpcr_pressure_target);
        params.qpcr_pressure_offset = DOUBLE_TO_OD_DATA(qpcr_pressure_offset);
        params.pwm_dytu_ratio = DOUBLE_TO_OD_DATA(pwm_dytu_ratio);
        params.qpcr_pressure_duty = DOUBLE_TO_OD_DATA(qpcr_pressure_duty);
    }

    return result;
}

//12号板设置参数
int air_pressure::air_setParameter(const air_pressure_Params &params)
{
    int result = -1;
    quint16 index = AIR_INDEX(PARAMETER);

    Commander c(DevicePart::NODE_ENVIRONMENT_MONITOR);
    QVector<Commander::SDOitem> items;

    int pcrmix_pressure_target = DOUBLE_FROM_OD_DATA(params.pcrmix_pressure_target);
    int pipet_pressure_target = DOUBLE_FROM_OD_DATA(params.pipet_pressure_target);
    int pcrmix_pressure_offset = DOUBLE_FROM_OD_DATA(params.pcrmix_pressure_offset);
    int pipet_pressure_offset = DOUBLE_FROM_OD_DATA(params.pipet_pressure_offset);
    int pwm_dytu_ratio = DOUBLE_FROM_OD_DATA(params.pwm_dytu_ratio);
    int pcrmix_pressure_duty = DOUBLE_FROM_OD_DATA(params.pcrmix_pressure_duty);
    int pipet_pressure_duty = DOUBLE_FROM_OD_DATA(params.pipet_pressure_duty);

    items << Commander::SDOitem(index, 0x03, &pcrmix_pressure_target) \
          << Commander::SDOitem(index, 0x04, &pipet_pressure_target) \
          << Commander::SDOitem(index, 0x06, &pcrmix_pressure_offset) \
          << Commander::SDOitem(index, 0x07, &pipet_pressure_offset) \
          << Commander::SDOitem(index, 0x09, &pwm_dytu_ratio) \
          << Commander::SDOitem(index, 0x0A, &pcrmix_pressure_duty) \
          << Commander::SDOitem(index, 0x0B, &pipet_pressure_duty);

    result = c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    c.exec(controlItem, statusIndex);

    return result;
}

//17号板设置参数
int air_pressure::xz_setParameter(const air_pressure_Params &params)
{
    int result = -1;
    quint16 index = AIR_INDEX(PARAMETER);

    Commander c(DevicePart::NODE_XZ_CLAW);
    QVector<Commander::SDOitem> items;

    int qpcr_pressure_target = DOUBLE_FROM_OD_DATA(params.qpcr_pressure_target);
    int qpcr_pressure_offset = DOUBLE_FROM_OD_DATA(params.qpcr_pressure_offset);
    int pwm_dytu_ratio = DOUBLE_FROM_OD_DATA(params.pwm_dytu_ratio);
    int qpcr_pressure_duty = DOUBLE_FROM_OD_DATA(params.qpcr_pressure_duty);

    items << Commander::SDOitem(index, 0x05, &qpcr_pressure_target) \
          << Commander::SDOitem(index, 0x08, &qpcr_pressure_offset) \
          << Commander::SDOitem(index, 0x09, &pwm_dytu_ratio) \
          << Commander::SDOitem(index, 0x0C, &qpcr_pressure_duty);

    result = c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    c.exec(controlItem, statusIndex);

    return result;
}

int air_pressure::pcrmix_chamber_start(int storeIndex)
{
    int result = -1;
    quint16 index = AIR_INDEX(PCRMIX_START);

    Commander c(DevicePart::NODE_ENVIRONMENT_MONITOR);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int air_pressure::pipette_chamber_start(int storeIndex)
{
    int result = -1;
    quint16 index = AIR_INDEX(PIPET_START);

    Commander c(DevicePart::NODE_ENVIRONMENT_MONITOR);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int air_pressure::qpcr_chamber_start(int storeIndex)
{
    int result = -1;
    quint16 index = AIR_INDEX(QPCR_START);

    Commander c(DevicePart::NODE_XZ_CLAW);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int air_pressure::pcrmix_chamber_stop(int storeIndex)
{
    int result = -1;
    quint16 index = AIR_INDEX(PCRMIX_STOP);

    Commander c(DevicePart::NODE_ENVIRONMENT_MONITOR);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int air_pressure::pipette_chamber_stop(int storeIndex)
{
    int result = -1;
    quint16 index = AIR_INDEX(PIPET_STOP);

    Commander c(DevicePart::NODE_ENVIRONMENT_MONITOR);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int air_pressure::qpcr_chamber_stop(int storeIndex)
{
    int result = -1;
    quint16 index = AIR_INDEX(QPCR_STOP);

    Commander c(DevicePart::NODE_XZ_CLAW);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int air_pressure::keep_fan_rpm(int storeIndex)
{
    int result = -1;
    quint16 index = AIR_INDEX(SPECIAL_STOP);

    Commander c(DevicePart::NODE_ENVIRONMENT_MONITOR);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    Commander cc(DevicePart::NODE_XZ_CLAW);

    Commander::SDOitem ccontrolItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex cstatusItem(index, 0x02);

    result = cc.exec(ccontrolItem, cstatusItem);

    return result;
}

void air_pressure::readpressure(int storeIndex)
{
    m_bReadTemp = true;
    qDebug()<<storeIndex;
    quint16 air_index = AIR_INDEX(CURRENT_PRESSURE);
    quint16 fan_index = AIR_INDEX(CURRENT_FAN);
    Commander c(DevicePart::NODE_ENVIRONMENT_MONITOR);
    Commander cc(DevicePart::NODE_XZ_CLAW);

    int pcrmix_chamber_pressure = 0, pipet_chamber_pressure = 0, qpcr_chamber_pressure = 0;
    int pcrmix_fan_1_rpm = 0;
    int pipet_fan_1_rpm = 0, pipet_fan_2_rpm = 0, pipet_fan_3_rpm = 0;
    int qpcr_fan_1_rpm = 0, qpcr_fan_2_rpm = 0;

    QList<float> pressures_rpm;
    QVector<Commander::SDOitem> items, xzitems;
    items << Commander::SDOitem(air_index, 0x03, &pcrmix_chamber_pressure) \
          << Commander::SDOitem(air_index, 0x04, &pipet_chamber_pressure) \
          << Commander::SDOitem(fan_index, 0x03, &pcrmix_fan_1_rpm) \
          << Commander::SDOitem(fan_index, 0x04, &pipet_fan_1_rpm) \
          << Commander::SDOitem(fan_index, 0x05, &pipet_fan_2_rpm) \
          << Commander::SDOitem(fan_index, 0x06, &pipet_fan_3_rpm);

    xzitems << Commander::SDOitem(air_index, 0x05, &qpcr_chamber_pressure) \
          << Commander::SDOitem(fan_index, 0x0B, &qpcr_fan_1_rpm) \
          << Commander::SDOitem(fan_index, 0x0C, &qpcr_fan_2_rpm);

//    while(m_bReadTemp)
//    {
        QThread::msleep(500);

        pressures_rpm.clear();
        c.readRemoteData(items);
//        cc.readRemoteData(xzitems);

        pressures_rpm << DOUBLE_TO_OD_DATA(pcrmix_chamber_pressure) << DOUBLE_TO_OD_DATA(pipet_chamber_pressure) << DOUBLE_TO_OD_DATA(qpcr_chamber_pressure)\
                        <<pcrmix_fan_1_rpm\
                        <<pipet_fan_1_rpm<<pipet_fan_2_rpm<<pipet_fan_3_rpm\
                        <<qpcr_fan_1_rpm<<qpcr_fan_2_rpm;

        Q_EMIT sigCurrentPressure(pressures_rpm);
//    }
}

void air_pressure::stop_readpressure()
{
    m_bReadTemp = false;
}




readpressure_Thread::readpressure_Thread()
{

}

void readpressure_Thread::Thread_continue() //继续
{
//    m_Paused = false;
}

void readpressure_Thread::Thread_stop() //停止
{
    T_stop = true;
}

void readpressure_Thread::run()
{
    T_stop = false;
    quint16 air_index = 0x2C87;
    quint16 fan_index = 0x2C88;
    Commander c(DevicePart::NODE_ENVIRONMENT_MONITOR);
    Commander cc(DevicePart::NODE_XZ_CLAW);

    int pcrmix_chamber_pressure = 0, pipet_chamber_pressure = 0, qpcr_chamber_pressure = 0;
    int pcrmix_fan_1_rpm = 0;
    int pipet_fan_1_rpm = 0, pipet_fan_2_rpm = 0, pipet_fan_3_rpm = 0;
    int qpcr_fan_1_rpm = 0, qpcr_fan_2_rpm = 0;

    QList<float> pressures_rpm;
    QVector<Commander::SDOitem> items, xzitems;
    items << Commander::SDOitem(air_index, 0x03, &pcrmix_chamber_pressure) \
          << Commander::SDOitem(air_index, 0x04, &pipet_chamber_pressure) \
          << Commander::SDOitem(fan_index, 0x03, &pcrmix_fan_1_rpm) \
          << Commander::SDOitem(fan_index, 0x04, &pipet_fan_1_rpm) \
          << Commander::SDOitem(fan_index, 0x05, &pipet_fan_2_rpm) \
          << Commander::SDOitem(fan_index, 0x06, &pipet_fan_3_rpm);

    xzitems << Commander::SDOitem(air_index, 0x05, &qpcr_chamber_pressure) \
          << Commander::SDOitem(fan_index, 0x0D, &qpcr_fan_1_rpm) \
          << Commander::SDOitem(fan_index, 0x0E, &qpcr_fan_2_rpm);

    while(T_stop != true)
    {
        sleep(1);

        pressures_rpm.clear();
        c.readRemoteData(items);
        cc.readRemoteData(xzitems);

        pressures_rpm << DOUBLE_TO_OD_DATA(pcrmix_chamber_pressure) << DOUBLE_TO_OD_DATA(pipet_chamber_pressure) << DOUBLE_TO_OD_DATA(qpcr_chamber_pressure)\
                        <<pcrmix_fan_1_rpm\
                        <<pipet_fan_1_rpm<<pipet_fan_2_rpm<<pipet_fan_3_rpm\
                        <<qpcr_fan_1_rpm<<qpcr_fan_2_rpm;

        emit newValue(pressures_rpm, true);
    }

    quit();
}
