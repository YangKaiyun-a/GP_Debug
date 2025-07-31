#解决dcf.c文件start_node()和start_and_seek_node()函数inline编译不过的问题
QMAKE_CFLAGS += -O1

#canfestival 日志信息
#QMAKE_CFLAGS += -DDEBUG_ERR_CONSOLE_ON -DDEBUG_WAR_CONSOLE_ON -g

INCLUDEPATH +=  $$PWD/include \
                                  $$PWD/include/win32

win32{
    LIBS += -L$$PWD/driver/lib/ -lControlCAN
    INCLUDEPATH += $$PWD/driver/include
}

SOURCES += \
    $$PWD/driver/src/timers_win32.c \
    $$PWD/driver/src/win32.c \
    $$PWD/src/dcf.c \
    $$PWD/src/emcy.c \
    $$PWD/src/lifegrd.c \
    $$PWD/src/lss.c \
    $$PWD/src/nmtMaster.c \
    $$PWD/src/nmtSlave.c \
    $$PWD/src/objacces.c \
    $$PWD/src/pdo.c \
    $$PWD/src/sdo.c \
    $$PWD/src/states.c \
    $$PWD/src/sync.c \
    $$PWD/src/timer.c
	
HEADERS += \
    $$PWD/include/can.h \
    $$PWD/include/can_driver.h \
    $$PWD/include/data.h \
    $$PWD/include/dcf.h \
    $$PWD/include/def.h \
    $$PWD/include/emcy.h \
    $$PWD/include/lifegrd.h \
    $$PWD/include/lss.h \
    $$PWD/include/nmtMaster.h \
    $$PWD/include/nmtSlave.h \
    $$PWD/include/objacces.h \
    $$PWD/include/objdictdef.h \
    $$PWD/include/pdo.h \
    $$PWD/include/sdo.h \
    $$PWD/include/states.h \
    $$PWD/include/sync.h \
    $$PWD/include/sysdep.h \
    $$PWD/include/timers.h \
    $$PWD/include/timers_driver.h \
    $$PWD/include/win32/applicfg.h \
    $$PWD/include/win32/canfestival.h \
    $$PWD/include/win32/config.h \
    $$PWD/include/win32/timerscfg.h
