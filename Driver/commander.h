#ifndef COMMANDER_H
#define COMMANDER_H

#include <QObject>
#include <QVector>

#include "canopenexecutor.h"

class QWaitCondition;

class Commander : public QObject
{
    Q_OBJECT

public:
    struct SDOIndex {
        quint16 index;
        quint8 sub_index;

        bool operator==(const SDOIndex& index) const {
            if (this->index == index.index && this->sub_index == index.sub_index)
                return true;
            else
                return false;
        }

        void operator=(const SDOIndex& index) {
            this->index = index.index;
            this->sub_index = index.sub_index;
        }

        SDOIndex() {

        }

        SDOIndex(quint16 index, quint8 subindex) {
            this->index = index;
            this->sub_index = subindex;
        }

    };

    struct SDOitem {
       SDOIndex index;
       void *data;

       SDOitem() {}

       SDOitem(quint16 index, quint8 subindex, void* data) {
           this->index.index = index;
           this->index.sub_index = subindex;
           this->data = data;
       }

       SDOitem(const SDOIndex& index, void* data) {
           this->index = index;
           this->data = data;
       }
    };

public:
    explicit Commander(int cob_id);
    ~Commander() {};

public:
    void writeLocalData(const QVector<SDOitem>& items);
    void readLocalData(QVector<SDOitem>& items);

    int writeRemoteData(const QVector<SDOitem>& items);
    int readRemoteData(QVector<SDOitem>& items);

    int exec(const SDOitem& controlItem, const SDOIndex& statusItem, quint32 timeout = 30000);

    void registerStatusCallBack(const SDOIndex& index);

    void waitStatusReply(const SDOIndex& index, int &reply);

private:
    static UNS32 reciveStatusCallBack(CO_Data *d, const indextable *table, uint8_t sub_index);
    void handleActionResult(const SDOIndex& index, int reply);

private:
    int m_CobId;
    int m_ActionReply;

    SDOIndex m_StatusIndex;
    static QList<Commander*> m_CallBackIndexs;
    static QList<SDOIndex> m_StatusCallBackIndexs;

    static QMutex m_mutex;
};

#endif // COMMANDER_H
