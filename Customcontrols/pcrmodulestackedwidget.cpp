#include "pcrmodulestackedwidget.h"
#include "ui_pcrmodulestackedwidget.h"
#include "pcrpostionaccuracywidget.h"
#include "pcrtemperatureaccuracywidget.h"
#include "pcrlightaccuracywidget.h"


PCRModuleStackedWidget::PCRModuleStackedWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PCRModuleStackedWidget)
{
    ui->setupUi(this);
    init();
}

PCRModuleStackedWidget::~PCRModuleStackedWidget()
{
    delete ui;
}

void PCRModuleStackedWidget::init()
{
    ui->btnM1->setObjectName("btnPCRM1M2");
    ui->btnM2->setObjectName("btnPCRM1M2");
}

void PCRModuleStackedWidget::insertStackedWidget(ENUM_CLASS::PCR_Fuction_Type type)
{
    QWidget *wgt1 = nullptr;
    QWidget *wgt2 = nullptr;

    switch (type)
    {
        case ENUM_CLASS::Position_Accuracy:
            wgt1 = new PCRPostionAccuracyWidget(ENUM_CLASS::M1, this);
            wgt2 = new PCRPostionAccuracyWidget(ENUM_CLASS::M2, this);
            break;
        case ENUM_CLASS::Temperature_Accuracy:
            wgt1 = new PCRTemperatureAccuracyWidget(ENUM_CLASS::M1, this);
            wgt2 = new PCRTemperatureAccuracyWidget(ENUM_CLASS::M2, this);
            break;
        case ENUM_CLASS::Light_Accuracy:
            wgt1 = new PCRLightAccuracyWidget(ENUM_CLASS::M1, this);
            wgt2 = new PCRLightAccuracyWidget(ENUM_CLASS::M2, this);
            break;
        default:
            return;
    }

    if(wgt1 && wgt2)
    {
        ui->m1m2StackedWidget->insertWidget(ENUM_CLASS::M1, wgt1);
        ui->m1m2StackedWidget->insertWidget(ENUM_CLASS::M2, wgt2);

        ui->m1m2StackedWidget->setCurrentIndex(ENUM_CLASS::M1);
    }
}

void PCRModuleStackedWidget::on_btnM1_clicked()
{
    ui->m1m2StackedWidget->setCurrentIndex(ENUM_CLASS::M1);
}


void PCRModuleStackedWidget::on_btnM2_clicked()
{
    ui->m1m2StackedWidget->setCurrentIndex(ENUM_CLASS::M2);
}

