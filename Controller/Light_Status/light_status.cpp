#include "light_status.h"
#include "Driver/commander.h"

Light_Status::Light_Status()
{

}

Light_Status *Light_Status::instance()
{
    static Light_Status light_status_ins;
    return &light_status_ins;
}

int Light_Status::uvlight_control(int node_id, int index_offset)
{
    int result = -1;
    quint16 index = UVLIGHT_INDEX(UVLIGHT_1_OPEN + index_offset);

    Commander c(node_id);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}

int Light_Status::blink_control(int id)
{
    int result = -1;

    quint16 index = PERIPHERALS_INDEX(PERIPHERALS_BLINK_CONTROL);

    Commander c(DevicePart::NODE_LIGHT_STATUA);

    QVector<Commander::SDOitem> items;

    items << Commander::SDOitem(index, 0x03, &id);

    c.writeRemoteData(items);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusIndex(index, 0x02);

    result = c.exec(controlItem, statusIndex);

    return result;
}

int Light_Status::light_control(int index_offset)
{
    int result = -1;
    quint16 index = PERIPHERALS_INDEX(index_offset);

    Commander c(DevicePart::NODE_LIGHT_STATUA);

    Commander::SDOitem controlItem(index, 0x01, ACTION_CONTROL);
    Commander::SDOIndex statusItem(index, 0x02);

    result = c.exec(controlItem, statusItem);

    return result;
}
