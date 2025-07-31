#include "pcrmodulewidget.h"
#include "ui_pcrmodulewidget.h"
#include "Customcontrols/pcrmodulestackedwidget.h"

#include <QButtonGroup>

PCRModuleWidget::PCRModuleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PCRModuleWidget)
{
    ui->setupUi(this);
    init();
}

PCRModuleWidget::~PCRModuleWidget()
{
    delete ui;
}

void PCRModuleWidget::init()
{
    ui->btnPosition->setObjectName("btnPCRModuleFuction");
    ui->btnTemperature->setObjectName("btnPCRModuleFuction");
    ui->btnLight->setObjectName("btnPCRModuleFuction");

    QButtonGroup *buttonGround = new QButtonGroup(this);
    buttonGround->addButton(ui->btnPosition);
    buttonGround->addButton(ui->btnTemperature);
    buttonGround->addButton(ui->btnLight);
    buttonGround->setExclusive(true);

    initStackedWidget();
}

void PCRModuleWidget::initStackedWidget()
{
    for(int i = 0; i < 3; ++i)
    {
        PCRModuleStackedWidget *stack = new PCRModuleStackedWidget(this);
        stack->insertStackedWidget(static_cast<ENUM_CLASS::PCR_Fuction_Type>(i));
        ui->moduleStackedWidget->insertWidget(i, stack);
    }

    ui->moduleStackedWidget->setCurrentIndex(ENUM_CLASS::Position_Accuracy);
}

void PCRModuleWidget::on_btnPosition_clicked()
{
    ui->moduleStackedWidget->setCurrentIndex(ENUM_CLASS::Position_Accuracy);
}

void PCRModuleWidget::on_btnTemperature_clicked()
{
    ui->moduleStackedWidget->setCurrentIndex(ENUM_CLASS::Temperature_Accuracy);
}

void PCRModuleWidget::on_btnLight_clicked()
{
    ui->moduleStackedWidget->setCurrentIndex(ENUM_CLASS::Light_Accuracy);
}
