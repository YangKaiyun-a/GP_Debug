#include "transporterpart.h"

#include <QThread>
#include <QDebug>

#include "Driver/commander.h"

TransporterPart *TransporterPart::instance()
{
    static TransporterPart transporter;

    return &transporter;
}

void TransporterPart::init()
{

}

int TransporterPart::process_1(int positionId, int isPlaceItem)
{
    int result = -1;
    quint16 index = TRANSPORTER_INDEX(3);

    Commander c(DevicePart::NODE_TRANSPORTER);

    QVector<Commander::SDOitem> items;
    int id = positionId;
    items << Commander::SDOitem(index, 0x03, &id) \
          << Commander::SDOitem(index, 0x04, &isPlaceItem);

    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int TransporterPart::process_2(int positionId)
{
    int result = -1;
    quint16 index = TRANSPORTER_INDEX(5);

    Commander c(DevicePart::NODE_TRANSPORTER);

    QVector<Commander::SDOitem> items;
    int id = positionId + 2;
    items << Commander::SDOitem(index, 0x03, &id);

    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int TransporterPart::moduleReset()
{
    int result = -1;
    quint16 index = MODULE_RESET_INDEX;

    Commander c(DevicePart::NODE_TRANSPORTER);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int TransporterPart::motorReset(int motorIndex)
{
    int result = -1;
    Commander::SDOitem controlItem;
    Commander::SDOIndex statusItem;

    quint16 index = TRANSPORTER_INDEX(MOTOR_RESET);

    Commander c(DevicePart::NODE_TRANSPORTER);

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
        controlItem.index.sub_index = 0x07;
        controlItem.data = ACTION_CONTROL;

        statusItem.index =  index;
        statusItem.sub_index = 0x08;
    }

    result = c.exec(controlItem, statusItem);

    return result;
}

int TransporterPart::motorMove(int motorIndex, const DevicePart::MotorMotionParams &param)
{

    int result = -1;
    quint16 index = TRANSPORTER_INDEX(MOTOR_MOVE);

    Commander c(DevicePart::NODE_TRANSPORTER);

    QVector<Commander::SDOitem> items;
    int targetPosition = DOUBLE_FROM_OD_DATA(param.targetPosition);
    int speed = DOUBLE_FROM_OD_DATA(param.speed);

    Commander::SDOitem controlItem;
    Commander::SDOIndex statusIndex;
    Commander::SDOIndex speedItem;
    Commander::SDOIndex curPositionItem;
    Commander::SDOIndex targetPositionItem;

    if (MOTOR_X == motorIndex)
    {
        controlItem.index.index = index;
        controlItem.index.sub_index = 0x01;
        controlItem.data = ACTION_CONTROL;

        statusIndex.index =  index;
        statusIndex.sub_index = 0x02;

        items << Commander::SDOitem(index, 0x03, &speed) \
              << Commander::SDOitem(index, 0x04, &targetPosition);
    }
    else if (MOTOR_Y == motorIndex)
    {
        controlItem.index.index = index;
        controlItem.index.sub_index = 0x06;
        controlItem.data = ACTION_CONTROL;

        statusIndex.index =  index;
        statusIndex.sub_index = 0x07;

        items << Commander::SDOitem(index, 0x08, &speed) \
              << Commander::SDOitem(index, 0x09, &targetPosition);

    }
    else if (MOTOR_Z == motorIndex)
    {
        controlItem.index.index = index;
        controlItem.index.sub_index = 0x0B;
        controlItem.data = ACTION_CONTROL;

        statusIndex.index =  index;
        statusIndex.sub_index = 0x0C;

        items << Commander::SDOitem(index, 0x0D, &speed) \
              << Commander::SDOitem(index, 0x0E, &targetPosition);
    }

    c.writeRemoteData(items);
    result = c.exec(controlItem, statusIndex);

    return result;
}

int TransporterPart::getParameter(TransporterPart::TransportParams &params)
{
    int result = -1;
    quint16 index = TRANSPORTER_INDEX(4);

    Commander c(DevicePart::NODE_TRANSPORTER);
    QVector<Commander::SDOitem> items;

    int y_to_exit_position = -1;
    int y_put_position = -1;
    int z_exit_position = -1;
    int z_prepare_position = -1;
    int z_put_position = -1;
    int x_put_position = -1;
    int left_tray_to_exit_position = -1;
    int right_tray_to_exit_position = -1;
    int right_tray_to_pcr_position = -1;
    int x_speed = -1;
    int z_speed = -1;
    int y_speed = -1;

    items << Commander::SDOitem(index, 0x03, &y_to_exit_position) \
          << Commander::SDOitem(index, 0x04, &y_put_position) \
          << Commander::SDOitem(index, 0x05, &z_exit_position) \
          << Commander::SDOitem(index, 0x06, &z_prepare_position) \
          << Commander::SDOitem(index, 0x07, &z_put_position) \
          << Commander::SDOitem(index, 0x08, &x_put_position) \
          << Commander::SDOitem(index, 0x09, &left_tray_to_exit_position) \
          << Commander::SDOitem(index, 0x0A, &right_tray_to_exit_position) \
          << Commander::SDOitem(index, 0x0B, &right_tray_to_pcr_position) \
          << Commander::SDOitem(index, 0x0C, &x_speed) \
          << Commander::SDOitem(index, 0x0D, &z_speed) \
          << Commander::SDOitem(index, 0x0E, &y_speed);

    result = c.readRemoteData(items);

    if (!result) {
        params.y_to_exit_position = DOUBLE_TO_OD_DATA(y_to_exit_position);
        params.y_put_position = DOUBLE_TO_OD_DATA(y_put_position);
        params.z_exit_position = DOUBLE_TO_OD_DATA(z_exit_position);

        params.z_prepare_position = DOUBLE_TO_OD_DATA(z_prepare_position);
        params.z_put_position = DOUBLE_TO_OD_DATA(z_put_position);
        params.x_put_position = DOUBLE_TO_OD_DATA(x_put_position);

        params.left_tray_to_exit_position = DOUBLE_TO_OD_DATA(left_tray_to_exit_position);
        params.right_tray_to_exit_position = DOUBLE_TO_OD_DATA(right_tray_to_exit_position);
        params.right_tray_to_pcr_position = DOUBLE_TO_OD_DATA(right_tray_to_pcr_position);

        params.x_speed = INT_TO_OD_DATA(x_speed);
        params.z_speed = INT_TO_OD_DATA(z_speed);
        params.y_speed = INT_TO_OD_DATA(y_speed);

    }

    return result;
}

int TransporterPart::setParameter(const TransporterPart::TransportParams &params)
{
    int result = -1;
    quint16 index = TRANSPORTER_INDEX(4);

    Commander c(DevicePart::NODE_TRANSPORTER);
    QVector<Commander::SDOitem> items;

    int y_put_position = DOUBLE_FROM_OD_DATA(params.y_put_position);
    int x_put_position = DOUBLE_FROM_OD_DATA(params.x_put_position);
    int left_tray_to_exit_position = DOUBLE_FROM_OD_DATA(params.left_tray_to_exit_position);
    int right_tray_to_exit_position = DOUBLE_FROM_OD_DATA(params.right_tray_to_exit_position);
    int right_tray_to_pcr_position = DOUBLE_FROM_OD_DATA(params.right_tray_to_pcr_position);

    items << Commander::SDOitem(index, 0x04, &y_put_position) \
          << Commander::SDOitem(index, 0x08, &x_put_position) \
          << Commander::SDOitem(index, 0x09, &left_tray_to_exit_position) \
          << Commander::SDOitem(index, 0x0A, &right_tray_to_exit_position) \
          << Commander::SDOitem(index, 0x0B, &right_tray_to_pcr_position);

    result = c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    c.exec(controlItem, statusIndex);

    return result;
}

void TransporterPart::agingStop(bool stop)
{
    m_aging = stop;
}

void TransporterPart::test(TransporterPart::testparam param, int is_pause)
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

        DevicePart::MotorMotionParams x_param;
        x_param.speed = 100;
        x_param.targetPosition = param.x_stop_positon;
        TransporterPart::instance()->motorMove(MOTOR_X, x_param);
        QThread::msleep(10);
        DevicePart::MotorMotionParams y_param;
        y_param.speed = 100;
        y_param.targetPosition = param.y_start_positon;
        TransporterPart::instance()->motorMove(MOTOR_Y, y_param);
        QThread::msleep(10);
        DevicePart::MotorMotionParams z_param;
        z_param.speed = 10;
        z_param.targetPosition = param.z_stop_positon;
        TransporterPart::instance()->motorMove(MOTOR_Z, z_param);
        QThread::msleep(10);
        z_param.speed = 10;
        z_param.targetPosition = param.z_start_positon;
        TransporterPart::instance()->motorMove(MOTOR_Z, z_param);
        QThread::msleep(10);
        y_param.speed = 100;
        y_param.targetPosition = param.y_stop_positon;
        TransporterPart::instance()->motorMove(MOTOR_Y, y_param);
        QThread::msleep(10);
        y_param.speed = 100;
        y_param.targetPosition = param.y_start_positon;
        TransporterPart::instance()->motorMove(MOTOR_Y, y_param);
        QThread::msleep(10);
        x_param.speed = 100;
        x_param.targetPosition = param.x_start_positon;
        TransporterPart::instance()->motorMove(MOTOR_X, x_param);
        QThread::msleep(10);
    }

    m_test = false;
}

//在位检测
int TransporterPart::checkBoardPlace(BoardCheckParam &params)
{
    int result = -1;
    quint16 index = TRANSPORTER_INDEX(6);

    Commander c(DevicePart::NODE_TRANSPORTER);
    QVector<Commander::SDOitem> items;

    int y_status = -1;
    int x_left_status = -1;
    int x_right_status = -1;

    items << Commander::SDOitem(index, 0x01, &y_status) \
          << Commander::SDOitem(index, 0x02, &x_left_status) \
          << Commander::SDOitem(index, 0x03, &x_right_status);

    result = c.readRemoteData(items);

    if(!result)
    {
        params.y_status = y_status;
        params.x_left_status = x_left_status;
        params.x_right_status = x_right_status;
    }

    return result;
}

void TransporterPart::stoptest()
{
    m_test = false;
}

TransporterPart::TransporterPart(QObject *parent) : AbstractPart(parent)
{
    m_aging = false;
}
