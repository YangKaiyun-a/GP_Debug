#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "application.h"
#include "ui/Frame/ModuleDebug/moduledebugwidget.h"
#include "Controller/Dp8/dp8.h"
#include "Controller/Dp1/dp1.h"
#include "Controller/HeatSealModule/heatsealpart.h"
#include "Controller/HeatSealModule/heatsealreadtemp.h"
#include "Controller/Extract_Dj1/extract_dj1.h"
#include "Controller/Amplification_Dj1/amplification_dj1.h"
#include "Controller/Transporter/transporterpart.h"
#include "Controller/Heat_Shake/heat_shake.h"
#include "Controller/CoolStoreModule/coolstorepart.h"
#include "Controller/Switch_Signal/switch_signal.h"
#include "Controller/Amplification_Cool/amplification_cool.h"

#include <QThread>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    initModuleThread();

    if(!m_moduleWidget)
    {
        m_moduleWidget = new ModuleDebugWidget();
    }
    ui->stackedWidget->addWidget(m_moduleWidget);

    if(!m_machineWidget)
    {
        m_machineWidget = new MachineDebugWidget();
    }
    ui->stackedWidget->addWidget(m_machineWidget);

    connect(hApp->m_sigmanager,&SignalManager::backHome,this,[=](){
        ui->stackedWidget->setCurrentIndex(0);
    });
}

void MainWindow::initModuleThread()
{
    m_pDp8Thread = new QThread;
    Dp8::instance()->moveToThread(m_pDp8Thread);
    m_pDp8Thread->start();

    m_pDp1Thread = new QThread;
    Dp1::instance()->moveToThread(m_pDp1Thread);
    m_pDp1Thread->start();

    m_pHeatSealThread = new QThread;
    HeatSealPart::instance()->moveToThread(m_pHeatSealThread);
    m_pHeatSealThread->start();

    m_pHeatTempReportThread = new QThread;
    HeatsealReadtemp::instance()->moveToThread(m_pHeatTempReportThread);
    m_pHeatTempReportThread->start();

    m_pExtractDj1Thread = new QThread;
    Extract_DJ1::instance()->moveToThread(m_pExtractDj1Thread);
    m_pExtractDj1Thread->start();

    m_pAmplificationDj1Thread = new QThread;
    Amplification_Dj1::instance()->moveToThread(m_pAmplificationDj1Thread);
    m_pAmplificationDj1Thread->start();

    m_pTransporterThread = new QThread;
    TransporterPart::instance()->moveToThread(m_pTransporterThread);
    m_pTransporterThread->start();

    m_pHeatShakeThread = new QThread;
    Heat_Shake::instance()->moveToThread(m_pHeatShakeThread);
    m_pHeatShakeThread->start();

    m_pHeatShakeReadTempThread = new QThread;
    Heat_Shake::read_temp_instance()->moveToThread(m_pHeatShakeReadTempThread);
    m_pHeatShakeReadTempThread->start();

    m_pCoolStoreThread = new QThread;
    CoolStorePart::instance()->moveToThread(m_pCoolStoreThread);
    m_pCoolStoreThread->start();

    m_pSwitchSignalThread = new QThread;
    Switch_Signal::instance()->moveToThread(m_pSwitchSignalThread);
    m_pSwitchSignalThread->start();

    m_pAmplificationCoolStoreThread = new QThread;
    Amplification_Cool::instance()->moveToThread(m_pAmplificationCoolStoreThread);
    m_pAmplificationCoolStoreThread->start();
}

//模块调试
void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_moduleWidget);
}

//整机调试
void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_machineWidget);
}

//退出
void MainWindow::on_pushButton_3_clicked()
{
    this->close();
}

