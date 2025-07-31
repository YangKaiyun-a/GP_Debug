#include "commander.h"

#include <QDebug>
#include <QThread>
#include <QMutexLocker>
#include <QWaitCondition>

#define TRANSFORM_HEX(value) (QString::number(value, 16))

QList<Commander*> Commander::m_CallBackIndexs;

static QMutex s_Mutex;
static QWaitCondition s_WaitContion;
QMutex Commander::m_mutex;

Commander::Commander(int cob_id) : m_CobId(cob_id)
{
    m_ActionReply = -1;
}

void Commander::writeLocalData(const QVector<Commander::SDOitem> &items)
{
    const subindex *si = nullptr;
    foreach (auto item, items) {
        si = CanopenExecutor::searchSubindex(item.index.index, item.index.sub_index);
        if (!si) {
            qDebug() << "不存在的subindex, index: " << TRANSFORM_HEX(item.index.index) << " subindex: " <<TRANSFORM_HEX(item.index.sub_index);
            break;
        }

        if (memcmp(si->pObject, item.data, si->size) != 0)
            memcpy(si->pObject, item.data, si->size);
    }
}

void Commander::readLocalData(QVector<Commander::SDOitem> &items)
{
    const subindex *si = nullptr;
    foreach (auto item, items)
    {
        si = CanopenExecutor::searchSubindex(item.index.index, item.index.sub_index);
        if (!si)
        {
            qDebug() << "不存在的subindex, index: " << TRANSFORM_HEX(item.index.index) << " subindex: " << TRANSFORM_HEX(item.index.sub_index);
            break;
        }

        memcpy(item.data, si->pObject, si->size);
    }
}

int Commander::writeRemoteData(const QVector<Commander::SDOitem> &items)
{
    writeLocalData(items);

    CanopenExecutor exec(m_CobId);
    foreach (auto item, items) {
        if (exec.writeSDO(item.index.index, item.index.sub_index)) {
            qDebug() << "写失败！index: " << TRANSFORM_HEX(item.index.index) << " subindex: " << TRANSFORM_HEX(item.index.sub_index);
            return 1;
        }
    }

    return 0;
}

int Commander::readRemoteData(QVector<Commander::SDOitem> &items)
{
    QMutexLocker locker(&m_mutex);

    CanopenExecutor exec(m_CobId);

    foreach (auto item, items)
    {
        if(exec.readSDO(item.index.index, item.index.sub_index))
        {
            qDebug() << "读失败！index: " << TRANSFORM_HEX(item.index.index) << " subindex: " << TRANSFORM_HEX(item.index.sub_index);
            return 1;
        }
    }

    readLocalData(items);

    return 0;
}

#define WAIT_ACTION_BACK
int Commander::exec(const SDOitem& controlItem, const SDOIndex& statusItem, quint32 timeout)
{
    CanopenExecutor exec(m_CobId);
    if (exec.writeSDO(controlItem.index.index, controlItem.index.sub_index, controlItem.data))
        return 1;

 #ifdef WAIT_ACTION_BACK
    registerStatusCallBack(statusItem);

    s_Mutex.lock();
    s_WaitContion.wait(&s_Mutex, timeout);
    m_CallBackIndexs.removeOne(this);
    s_Mutex.unlock();
#endif

    return m_ActionReply;
}

void Commander::registerStatusCallBack(const SDOIndex& index)
{
    m_StatusIndex = index;
    s_Mutex.lock();
    m_CallBackIndexs.append(this);
    s_Mutex.unlock();
    RegisterSetODentryCallBack(CanopenExecutor::coData(), index.index, index.sub_index, &Commander::reciveStatusCallBack);
}

void Commander::waitStatusReply(const Commander::SDOIndex &index, int &reply)
{

}

UNS32 Commander::reciveStatusCallBack(CO_Data *d, const indextable *table, uint8_t sub_index)
{
    Q_UNUSED(d)
    SDOIndex index;
    index.index = table->index;
    index.sub_index = sub_index;

    int reply = *(int*)table->pSubindex[sub_index].pObject;

    foreach (auto co, m_CallBackIndexs) {
       if (co->m_StatusIndex == index)
           co->handleActionResult(index, reply);
    }

    return 0;
}

void Commander::handleActionResult(const Commander::SDOIndex &index, int reply)
{
    Q_UNUSED(index)
    qDebug() << "Callback Index:"<<TRANSFORM_HEX(index.index) << " subindex: " << TRANSFORM_HEX(index.sub_index) << " Reply: " << reply;

    s_Mutex.lock();
    m_ActionReply = reply;
    s_WaitContion.wakeOne();
    s_Mutex.unlock();
}
