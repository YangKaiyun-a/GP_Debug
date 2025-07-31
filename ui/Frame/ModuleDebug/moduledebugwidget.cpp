#include "moduledebugwidget.h"
#include "ui_moduledebugwidget.h"
#include "application.h"
#include "heatshakermodulewidget.h"
#include "tp08modulewidget.h"
#include "heatsealmodulewidget.h"
#include "pcrmodulewidget.h"

#include <QDateTime>
#include <QTimer>

ModuleDebugWidget::ModuleDebugWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModuleDebugWidget)
{
    ui->setupUi(this);
    init();
}

ModuleDebugWidget::~ModuleDebugWidget()
{
    delete ui;
}

void ModuleDebugWidget::init()
{
    initConnect();
    initStackWidget();
    startSystemDataTime();
}

void ModuleDebugWidget::initConnect()
{
    connect(hApp->m_sigmanager, &SignalManager::updateSysTime, this, &ModuleDebugWidget::onUpdateSysTime);
}

void ModuleDebugWidget::initStackWidget()
{
    HeatShakerModuleWidget *shaker = new HeatShakerModuleWidget(this);
    ui->stackedWidget->insertWidget(HEAT_SHAKER, shaker);

    TP08ModuleWidget *tp08 = new TP08ModuleWidget(this);
    ui->stackedWidget->insertWidget(TP08, tp08);

    HeatSealModuleWidget *seal = new HeatSealModuleWidget(this);
    ui->stackedWidget->insertWidget(HEAT_SEAL, seal);

    PCRModuleWidget *pcr = new PCRModuleWidget(this);
    ui->stackedWidget->insertWidget(PCR, pcr);

    ui->stackedWidget->setCurrentIndex(HEAT_SHAKER);
}

void ModuleDebugWidget::startSystemDataTime()
{
    onUpdateSysTime();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, hApp->m_sigmanager, &SignalManager::updateSysTime);
    timer->start(1000);
}

void ModuleDebugWidget::onUpdateSysTime()
{
    QDateTime curdatetime = QDateTime::currentDateTime();
    ui->labTime->setText(curdatetime.toString("hh:mm:ss"));
    ui->labDate->setText(curdatetime.toString("yyyy/MM/dd"));
}

void ModuleDebugWidget::on_btnHeatShaker_clicked()
{
    ui->stackedWidget->setCurrentIndex(HEAT_SHAKER);
}

void ModuleDebugWidget::on_btnTP08_clicked()
{
    ui->stackedWidget->setCurrentIndex(TP08);
}

void ModuleDebugWidget::on_btnHeatSeal_clicked()
{
    ui->stackedWidget->setCurrentIndex(HEAT_SEAL);
}

void ModuleDebugWidget::on_btnPCR_clicked()
{
    ui->stackedWidget->setCurrentIndex(PCR);
}

void ModuleDebugWidget::on_btnExit_clicked()
{
    Q_EMIT hApp->m_sigmanager->backHome();
}
