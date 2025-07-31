#include "debugwidget.h"
#include "ui_debugwidget.h"
#include "dj1debugwidget.h"
#include "dj2debugwidget.h"
#include "dp1debugwidget.h"
#include "dp8debugwidget.h"
#include "data/global.h"

#include <QButtonGroup>

DebugWidget::DebugWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DebugWidget)
{
    ui->setupUi(this);
    init();
}

DebugWidget::~DebugWidget()
{
    delete ui;
}

void DebugWidget::init()
{
    //复用PCR模块的按钮样式
    ui->btnDJ1->setObjectName("btnPCRModuleFuction");
    ui->btnDJ2->setObjectName("btnPCRModuleFuction");
    ui->btnDP1->setObjectName("btnPCRModuleFuction");
    ui->btnDP8->setObjectName("btnPCRModuleFuction");

    QButtonGroup *buttonGround = new QButtonGroup(this);
    buttonGround->addButton(ui->btnDJ1);
    buttonGround->addButton(ui->btnDJ2);
    buttonGround->addButton(ui->btnDP1);
    buttonGround->addButton(ui->btnDP8);
    buttonGround->setExclusive(true);

    initStackedWidget();
}

void DebugWidget::initStackedWidget()
{
    DJ1DebugWidget *dj1 = new DJ1DebugWidget(this);
    ui->moduleStackedWidget->insertWidget(ENUM_CLASS::DJ1, dj1);

    DJ2DebugWidget *dj2 = new DJ2DebugWidget(this);
    ui->moduleStackedWidget->insertWidget(ENUM_CLASS::DJ2, dj2);

    DP1DebugWidget *dp1 = new DP1DebugWidget(this);
    ui->moduleStackedWidget->insertWidget(ENUM_CLASS::DP1, dp1);

    DP8DebugWidget *dp8 = new DP8DebugWidget(this);
    ui->moduleStackedWidget->insertWidget(ENUM_CLASS::DP8, dp8);

    ui->moduleStackedWidget->setCurrentIndex(ENUM_CLASS::DJ1);
}

void DebugWidget::on_btnDJ1_clicked()
{
    ui->moduleStackedWidget->setCurrentIndex(ENUM_CLASS::DJ1);
}

void DebugWidget::on_btnDJ2_clicked()
{
    ui->moduleStackedWidget->setCurrentIndex(ENUM_CLASS::DJ2);
}

void DebugWidget::on_btnDP1_clicked()
{
    ui->moduleStackedWidget->setCurrentIndex(ENUM_CLASS::DP1);
}

void DebugWidget::on_btnDP8_clicked()
{
    ui->moduleStackedWidget->setCurrentIndex(ENUM_CLASS::DP8);
}

