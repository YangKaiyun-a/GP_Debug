#include "canopenexecutor.h"

#include <QDebug>

QMutex CanopenExecutor::m_mutex;
CO_Data *CanopenExecutor::s_pDdata = &dp24EDS_Data;


CanopenExecutor::CanopenExecutor(uint8_t cob_id):m_Cobid(cob_id)
{

}

subindex *CanopenExecutor::searchSubindex(uint16_t index, uint8_t sub_index)
{
    ulong err;
    ODCallback_t *callbacks;
    const indextable *table = s_pDdata->scanIndexOD(index, &err, &callbacks);

    if (!table || sub_index >= table->bSubCount)
    {
        return NULL;
    }

    return &table->pSubindex[sub_index];
}

int CanopenExecutor::writeSDO(uint16_t index, uint8_t sub_index)
{
    QMutexLocker locker(&m_mutex);

    const subindex *si = searchSubindex(index, sub_index);

    if (!si)
        return 1;

    ulong code = writeNetworkDict(s_pDdata, m_Cobid, index, sub_index, si->size, si->bDataType, si->pObject, 0);
    if (code)
    {
        return 2;
    }

    uint8_t write_result;
    while (1)
    {
        write_result = getWriteResultNetworkDict(s_pDdata, m_Cobid, &code);

        if (write_result == SDO_DOWNLOAD_IN_PROGRESS)
            continue;
        else
            break;
    }

    if (write_result != SDO_FINISHED)
    {
        closeSDOtransfer(s_pDdata, m_Cobid, SDO_CLIENT);
        return -1;
    }
    closeSDOtransfer(s_pDdata, m_Cobid, SDO_CLIENT);

    return 0;
}

int CanopenExecutor::readSDO(uint16_t index, uint8_t sub_index)
{
    QMutexLocker locker(&m_mutex);

    const subindex *si = searchSubindex(index, sub_index);
    if (!si)
    {
        return 1;
    }

    UNS32 code = readNetworkDict(s_pDdata, m_Cobid, index, sub_index, si->bDataType, 0);

    if(code)
    {
        locker.unlock();
        return 2;
    }

    UNS8 read_result;
    UNS32 size = si->size;
    while (1)
    {
        read_result = getReadResultNetworkDict(s_pDdata, m_Cobid, si->pObject, &size, &code);

        if (read_result == SDO_UPLOAD_IN_PROGRESS)
        {
            continue;
        }
        else
        {
            break;
        }
    }
    locker.unlock();

    if(read_result != SDO_FINISHED)
    {
        closeSDOtransfer(s_pDdata, m_Cobid, SDO_CLIENT);
        return -1;
    }

    closeSDOtransfer(s_pDdata, m_Cobid, SDO_CLIENT);

    return 0;
}

int CanopenExecutor::writeSDO(uint16_t index, uint8_t sub_index, void *data)
{
    const subindex *si = searchSubindex(index, sub_index);

    if (!si)
        return 1;

    QMutexLocker locker(&m_mutex);
    ulong code = writeNetworkDict(s_pDdata, m_Cobid, index, sub_index, si->size, si->bDataType, data, 0);

    if(code)
    {
        return 2;
    }

    uint8_t write_result;
    while (1)
    {
        write_result = getWriteResultNetworkDict(s_pDdata, m_Cobid, &code);

        if (write_result == SDO_DOWNLOAD_IN_PROGRESS)
            continue;
        else
            break;
    }
    locker.unlock();

    if (write_result != SDO_FINISHED)
    {
        closeSDOtransfer(s_pDdata, m_Cobid, SDO_CLIENT);
        return write_result;
    }
    closeSDOtransfer(s_pDdata, m_Cobid, SDO_CLIENT);

    return 0;
}

int CanopenExecutor::readSDO(uint16_t index, uint8_t sub_index, void *data)
{
    const subindex *si = searchSubindex(index, sub_index);
    if (!si)
        return 1;

    QMutexLocker locker(&m_mutex);
    UNS32 code = readNetworkDict(s_pDdata, m_Cobid, index, sub_index, si->bDataType, 0);

    if (code)
    {
        return 2;
    }

    UNS8 read_result;
    UNS32 size = si->size;
    while (1)
    {
        read_result = getReadResultNetworkDict(s_pDdata, m_Cobid, data, &size, &code);

        if (read_result == SDO_UPLOAD_IN_PROGRESS)
            continue;
        else
            break;
    }

    if (read_result != SDO_FINISHED)
    {
        closeSDOtransfer(s_pDdata, m_Cobid, SDO_CLIENT);
        return -1;
    }

    closeSDOtransfer(s_pDdata, m_Cobid, SDO_CLIENT);

    return 0;
}
