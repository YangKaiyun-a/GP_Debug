/*
This file is part of CanFestival, a library implementing CanOpen Stack.

Copyright (C): Edouard TISSERANT and Francis DUPIN
Copyright (C) Win32 Port Leonid Tochinski
Modified by: Jaroslav Fojtik

See COPYING file for copyrights details.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
 CAN driver interface.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "canfestival.h"
#include "timers.h"
#include "timers_driver.h"

#include "Driver/dp24EDS.h"

#include "ControlCAN.h"

#include <time.h>

#ifdef __cplusplus
};
#endif

#define UNUSED(x) (void)(x)
#define DEVICE_ID  0
#define CAN_INDEX 0
#define FRAME_LENG 1

static void writeLogFile(const VCI_CAN_OBJ can_obj)
{
#ifdef SAVE_LOG_TO_FILE
    FILE *fp =  fopen("./Log/log.txt", "a+");

    time_t ltime; /* calendar time */
    ltime=time(NULL); /* get current cal time */
    fprintf(fp, "%s", asctime(localtime(&ltime)));

    fprintf(fp, "Send Node: %x Data:", can_obj.ID);
    for (int i = 0; i < 8; i++) {
        if (can_obj.Data[i] < 0x10)
            fprintf(fp, " 0x0%x ", can_obj.Data[i]);
        else
            fprintf(fp, " 0x%x ", can_obj.Data[i]);
    }
    fprintf(fp, "\n");

    fflush(fp);
    fclose(fp);

#else
    printf("Send Node: %x Data:", can_obj.ID);
    for (int i = 0; i < 8; i++) {
        if (can_obj.Data[i] < 0x10)
            printf(" 0x0%x ", can_obj.Data[i]);
        else
            printf( " 0x%x ", can_obj.Data[i]);
    }
    printf("\n");

    fflush(stdout);

#endif
}


UNS8 canChangeBaudRate(CAN_PORT port, char* baud)
{
    UNUSED(port);
    UNUSED(baud);

    return 0;
}

UNS8 UnLoadCanDriver(LIB_HANDLE handle)
{
    UNUSED(handle);

    return 0;
}

LIB_HANDLE LoadCanDriver(LPCSTR driver_name)
{
    UNUSED(driver_name);

    return NULL;
}

CAN_PORT canOpen(s_BOARD *board, CO_Data * d)
{
    UNUSED(board);
    UNUSED(d);

    return NULL;
}

int canClose(CO_Data * d)
{
    UNUSED(d);

    return 0;
}

UNS8 canSend(CAN_PORT port, Message *m)
{
    UNUSED(port);

    VCI_CAN_OBJ can_obj;

    can_obj.ID = m->cob_id;
    //发送类型
    can_obj.SendType = 0;
    //数据类型
    can_obj.RemoteFlag = 0;
    //是否扩展帧
    can_obj.ExternFlag = 0;
    //数据长度
    can_obj.DataLen = m->len;

    memcpy(can_obj.Data, m->data, m->len);

    writeLogFile(can_obj);

    return !VCI_Transmit(VCI_USBCAN2, DEVICE_ID, CAN_INDEX, can_obj, FRAME_LENG);
}

CO_Data *dictdata = &dp24EDS_Data;

int CanOpenStart()
{
    TimerInit();

    setNodeId(dictdata, BOARD_NODE);

    setState(dictdata, Initialisation);
    setState(dictdata, Operational);

    masterSendNMTstateChange(dictdata, 0, NMT_Start_Node);

    StartTimerLoop(NULL);

    return 1;
}



