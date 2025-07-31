#include "heatshakermodulewidget.h"
#include "ui_heatshakermodulewidget.h"
#include "Customcontrols/heatshakerdatawidget.h"
#include "Controller/abstractpart.h"
#include "Controller/Heat_Shake/heat_shake.h"

#include <QButtonGroup>

HeatShakerModuleWidget::HeatShakerModuleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeatShakerModuleWidget)
{
    ui->setupUi(this);
    init();
}

HeatShakerModuleWidget::~HeatShakerModuleWidget()
{
    delete ui;
}

void HeatShakerModuleWidget::init()
{
    initStackedWidget();
    initButton();

    ui->label->setObjectName("labModuleAction");
    ui->line_9->setObjectName("blackLine");
}

void HeatShakerModuleWidget::initStackedWidget()
{
    for(int i = 38; i < 43; ++i)
    {
        HeatShakerDataWidget *shakerData = new HeatShakerDataWidget(i, this);
        ui->shakerStackedWidget->addWidget(shakerData);
        m_map.insert(i, shakerData);
    }

    ui->shakerStackedWidget->setCurrentWidget(m_map.value(DevicePart::NODE_HEAT_SHAKE_1));
}

void HeatShakerModuleWidget::initButton()
{
    QButtonGroup *buttonGround = new QButtonGroup(this);
    buttonGround->addButton(ui->btnNumber1);
    buttonGround->addButton(ui->btnNumber2);
    buttonGround->addButton(ui->btnNumber3);
    buttonGround->addButton(ui->btnNumber4);
    buttonGround->addButton(ui->btnNumber5);
    buttonGround->setExclusive(true);
}

void HeatShakerModuleWidget::on_btnNumber1_clicked()
{
    ui->shakerStackedWidget->setCurrentWidget(m_map.value(DevicePart::NODE_HEAT_SHAKE_1));
}

void HeatShakerModuleWidget::on_btnNumber2_clicked()
{
    ui->shakerStackedWidget->setCurrentWidget(m_map.value(DevicePart::NODE_HEAT_SHAKE_2));
}

void HeatShakerModuleWidget::on_btnNumber3_clicked()
{
    ui->shakerStackedWidget->setCurrentWidget(m_map.value(DevicePart::NODE_HEAT_SHAKE_3));
}

void HeatShakerModuleWidget::on_btnNumber4_clicked()
{
    ui->shakerStackedWidget->setCurrentWidget(m_map.value(DevicePart::NODE_HEAT_SHAKE_4));
}

void HeatShakerModuleWidget::on_btnNumber5_clicked()
{
    ui->shakerStackedWidget->setCurrentWidget(m_map.value(DevicePart::NODE_HEAT_SHAKE_5));
}

//磁珠振荡位-模块复位
void HeatShakerModuleWidget::on_btnMagnetic_clicked()
{
    Heat_Shake::instance()->moduleReset(DevicePart::NODE_HEAT_SHAKE_6);
}

//低速振荡
void HeatShakerModuleWidget::on_btnMagnetic_2_clicked()
{
    Heat_Shake::instance()->start_shake(DevicePart::NODE_HEAT_SHAKE_6, 900);
}

//高速振荡
void HeatShakerModuleWidget::on_btnMagnetic_3_clicked()
{
    Heat_Shake::instance()->start_shake(DevicePart::NODE_HEAT_SHAKE_6, 2000);
}

//停止振荡
void HeatShakerModuleWidget::on_btnMagnetic_4_clicked()
{
    Heat_Shake::instance()->stop_shake(DevicePart::NODE_HEAT_SHAKE_6);
}

