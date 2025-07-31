#include "machinedebugwidget.h"
#include "ui_machinedebugwidget.h"
#include "application.h"
#include "AccuratingPosition/accuratingpositionwidget.h"
#include "Pipette/pipettewidget.h"
#include "ColdePerformance/coldeperformancewidget.h"
#include "WorkflowEmptyRun/workflowemptyrunwidget.h"
#include "DebugPosition/debugwidget.h"
#include "Other/otherwidget.h"

#include <QDateTime>
#include <QTimer>
#include <QDebug>

MachineDebugWidget::MachineDebugWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MachineDebugWidget)
{
    ui->setupUi(this);
    init();
}

MachineDebugWidget::~MachineDebugWidget()
{
    delete ui;
}

void MachineDebugWidget::init()
{
    initConnect();
    initStackWidget();
    startSystemDataTime();
}

void MachineDebugWidget::initConnect()
{
    connect(hApp->m_sigmanager, &SignalManager::updateSysTime, this, &MachineDebugWidget::onUpdateSysTime);
}

void MachineDebugWidget::initStackWidget()
{
    AccuratingPositionWidget *position = new AccuratingPositionWidget(this);
    ui->stackedWidget->insertWidget(ACCURATING_POSITION, position);

    PipetteWidget *pipette = new PipetteWidget(this);
    ui->stackedWidget->insertWidget(PIPETTING_PERFORMANCE, pipette);

    DebugWidget *debug = new DebugWidget(this);
    ui->stackedWidget->insertWidget(DEBUG_WIDGET, debug);

    ColdePerformanceWidget *cold = new ColdePerformanceWidget(this);
    ui->stackedWidget->insertWidget(COLDING_PERFORMANCE, cold);

    WorkflowEmptyRunWidget *work = new WorkflowEmptyRunWidget(this);
    ui->stackedWidget->insertWidget(WORKFLOW_EMPTY_RUN, work);

    OtherWidget *other = new OtherWidget(this);
    ui->stackedWidget->insertWidget(OTHER, other);

    ui->stackedWidget->setCurrentIndex(OTHER);
}

void MachineDebugWidget::startSystemDataTime()
{
    onUpdateSysTime();

    //不再设置计时器，与模块调试共用一个计时器
}

void MachineDebugWidget::onUpdateSysTime()
{
    QDateTime curdatetime = QDateTime::currentDateTime();
    ui->labTime->setText(curdatetime.toString("hh:mm:ss"));
    ui->labDate->setText(curdatetime.toString("yyyy/MM/dd"));
}

void MachineDebugWidget::on_btnExit_clicked()
{
    Q_EMIT hApp->m_sigmanager->backHome();
}

//位置校准
void MachineDebugWidget::on_btnHeatShaker_clicked()
{
    ui->stackedWidget->setCurrentIndex(ACCURATING_POSITION);
}

//移液性能
void MachineDebugWidget::on_btnTP08_clicked()
{
    ui->stackedWidget->setCurrentIndex(PIPETTING_PERFORMANCE);
}

//制冷性能
void MachineDebugWidget::on_btnHeatSeal_clicked()
{
    ui->stackedWidget->setCurrentIndex(COLDING_PERFORMANCE);
}

//流程空跑
void MachineDebugWidget::on_btnPCR_clicked()
{
    ui->stackedWidget->setCurrentIndex(WORKFLOW_EMPTY_RUN);
}

//框架
void MachineDebugWidget::on_btnOther_clicked()
{
    ui->stackedWidget->setCurrentIndex(OTHER);
}

//电机调试
void MachineDebugWidget::on_btnDebug_clicked()
{
    ui->stackedWidget->setCurrentIndex(DEBUG_WIDGET);
}

