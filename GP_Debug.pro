QT       += core gui network sql charts axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ui
INCLUDEPATH += Function
INCLUDEPATH += Customcontrols
INCLUDEPATH += hcscitoolkits
INCLUDEPATH += CanFestival


include(ui/ui.pri)
include(Function/Function.pri)
include(Customcontrols/Customcontrols.pri)
include(hcscitoolkits/hcscitoolkits.pri)
include(CanFestival/CanFestival.pri)


SOURCES += \
    Controller/air_pressure/air_pressure.cpp \
    Controller/Amplification_Cool/amplification_cool.cpp \
    Controller/Amplification_Dj1/amplification_dj1.cpp \
    Controller/CoolStoreModule/coolstorepart.cpp \
    Controller/Dp1/dp1.cpp \
    Controller/Dp1_xy/dp1_xy.cpp \
    Controller/Extract_Dj1/extract_dj1.cpp \
    Controller/HeatSealModule/heatsealpart.cpp \
    Controller/HeatSealModule/heatsealreadtemp.cpp \
    Controller/Heat_Shake/heat_shake.cpp \
    Controller/Switch_Signal/switch_signal.cpp \
    Controller/Transporter/transporterpart.cpp \
    Controller/abstractpart.cpp \
    Controller/Dp8/dp8.cpp \
    Controller/Dp8_xy/dp8_xy.cpp \
    Controller/xzclaw/xzclaw_control.cpp \
    Controller/mqttexecutor.cpp \
    Controller/sportmethods.cpp \
    Controller/Light_Status/light_status.cpp \
    Driver/canmgr.cpp \
    Driver/canopenexecutor.cpp \
    Driver/commander.cpp \
    Driver/dp24EDS.c \
    application.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Controller/air_pressure/air_pressure.h \
    Controller/mqttexecutor.h \
    Controller/sportmethods.h \
    application.h \
    Controller/Amplification_Cool/amplification_cool.h \
    Controller/Amplification_Dj1/amplification_dj1.h \
    Controller/CoolStoreModule/coolstorepart.h \
    Controller/Dp1/dp1.h \
    Controller/Dp1_xy/dp1_xy.h \
    Controller/Extract_Dj1/extract_dj1.h \
    Controller/HeatSealModule/heatsealpart.h \
    Controller/HeatSealModule/heatsealreadtemp.h \
    Controller/Heat_Shake/heat_shake.h \
    Controller/Switch_Signal/switch_signal.h \
    Controller/Transporter/transporterpart.h \
    Controller/abstractpart.h \
    Controller/Dp8/dp8.h \
    Controller/Dp8_xy/dp8_xy.h \
    Controller/xzclaw/xzclaw_control.h \
    Controller/Light_Status/light_status.h \
    Driver/canmgr.h \
    Driver/canopenexecutor.h \
    Driver/commander.h \
    Driver/dp24EDS.h \
    mainwindow.h

FORMS += \
    mainwindow.ui




# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

unix|win32: LIBS += -L$$PWD/lib/ -lQt5Mqtt

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
