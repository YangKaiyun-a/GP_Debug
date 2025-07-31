#ifndef CANOPENEXECUTOR_H
#define CANOPENEXECUTOR_H

#include <QMutexLocker>

#ifdef __cplusplus
extern "C" {
#endif

#include "canfestival.h"
#include "Driver/dp24EDS.h"

#ifdef __cplusplus
};
#endif

class CanopenExecutor
{

public:
    explicit CanopenExecutor(uint8_t cob_id);
    ~CanopenExecutor() {};

    static subindex *searchSubindex(uint16_t index, uint8_t sub_index);
    int writeSDO(uint16_t index, uint8_t sub_index);
    int readSDO(uint16_t index, uint8_t sub_index);

    int writeSDO(uint16_t index, uint8_t sub_index, void* data);
    int readSDO(uint16_t index, uint8_t sub_index, void* data);

    inline static CO_Data *coData()
    {
        return s_pDdata;
    }

private:
    uint8_t m_Cobid;
    static QMutex m_mutex;
    static CO_Data *s_pDdata;
};

#endif // CANOPENEXECUTOR_H
