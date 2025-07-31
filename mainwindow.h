#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ui/Frame/ModuleDebug/moduledebugwidget.h"
#include "ui/Frame/MachineDebug/machinedebugwidget.h"


#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QThread;

class GeneralStackWidget;
class Dp8StackWidget;
class Dp1StackWidget;
class ExtractDj1Stackwidget;
class HeatSealStack;
class AmplificationDj1StackWidget;
class TransporterStackWidget;
class HeatShakeStack;
class CoolStoreStack;
class SwitchSignalStackWidget;
class AmplificationCoolStack;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();
    void initModuleThread();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    ModuleDebugWidget *m_moduleWidget = nullptr;
    MachineDebugWidget *m_machineWidget = nullptr;

    GeneralStackWidget *m_pGeneralStackWidget;
    Dp8StackWidget *m_pDp8StackWidget;
    HeatSealStack *m_pHeatSealStack;
    Dp1StackWidget *m_pDp1StackWidget;
    ExtractDj1Stackwidget *m_pExtractDj1Stackwidget;
    AmplificationDj1StackWidget *m_pAmplificationDj1StackWidget;
    TransporterStackWidget *m_pTransporterStackWidget;
    HeatShakeStack *m_pHeatShakeStackWidget;
    CoolStoreStack *m_pCoolStoreStackWidget;
    SwitchSignalStackWidget *m_pSwitchSignalStackWidget;
    AmplificationCoolStack *m_pAmplificationCoolStack;

    QThread *m_pDp8Thread;
    QThread *m_pHeatSealThread;
    QThread *m_pHeatTempReportThread;
    QThread *m_pDp1Thread;
    QThread *m_pExtractDj1Thread;
    QThread *m_pAmplificationDj1Thread;
    QThread *m_pTransporterThread;
    QThread *m_pHeatShakeThread;
    QThread *m_pHeatShakeReadTempThread;
    QThread *m_pCoolStoreThread;
    QThread *m_pSwitchSignalThread;
    QThread *m_pAmplificationCoolStoreThread;
};

#endif // MAINWINDOW_H
