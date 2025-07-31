#include "heatsealreadtemp.h"
#include "Driver/commander.h"
#include <QThread>

HeatsealReadtemp::HeatsealReadtemp()
{

}

HeatsealReadtemp *HeatsealReadtemp::instance()
{
    static HeatsealReadtemp s;
    return &s;
}

void HeatsealReadtemp::readTemp()
{
    m_bReadTemp = true;

    quint16 index = HEATSEAL_INDEX(TEMP_CURRENT);
    Commander c(DevicePart::NODE_HEATSEAL);

    int temp1 = 0, temp2 = 0, temp3 = 0;
    QList<float> temps;
    QVector<Commander::SDOitem> items;
    items << Commander::SDOitem(index, 0x01, &temp1) \
          << Commander::SDOitem(index, 0x02, &temp2) \
          << Commander::SDOitem(index, 0x03, &temp3);

    while(m_bReadTemp)
    {
        QThread::msleep(300);
        temps.clear();
        c.readRemoteData(items);
        temps << DOUBLE_TO_OD_DATA(temp1) << DOUBLE_TO_OD_DATA(temp2)<<DOUBLE_TO_OD_DATA(temp3);
        Q_EMIT sigCurrentTemp(temps);

    }
}

int HeatsealReadtemp::motorMove(int motorIndex, const DevicePart::MotorMotionParams &)
{

}

int HeatsealReadtemp::motorReset(int motorIndex)
{

}
