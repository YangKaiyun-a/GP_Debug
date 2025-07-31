#include "dp8pcrwidget.h"
#include "ui_dp8pcrwidget.h"
#include "Controller/sportmethods.h"
#include "application.h"
#include "Function/data/dbmanager.h"
#include "hcscitoolkits/dialog/messagedialog.h"

#include <QDebug>

DP8PcrWidget::DP8PcrWidget(Storage_Info storageInfo, PipetteInfo pipetteInfo, ClawInfo clawInfo, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DP8PcrWidget), m_storageInfo(storageInfo), m_pipetteInfo(pipetteInfo), m_clawInfo(clawInfo)
{
    ui->setupUi(this);
    init();
}

DP8PcrWidget::~DP8PcrWidget()
{
    delete ui;
}

void DP8PcrWidget::init()
{
    initStyleSheet();
    initData();
    initUI();
}

void DP8PcrWidget::initStyleSheet()
{
    ui->label_48->setObjectName("labModuleAction");
}

void DP8PcrWidget::initData()
{
    connect(hApp->m_sigmanager, &SignalManager::updatePipetteDeviation, this, &DP8PcrWidget::onUpdatePipetteDeviation);
    connect(hApp->m_sigmanager, &SignalManager::updateClawInfo, this, &DP8PcrWidget::onUpdateClawInfo);
}

void DP8PcrWidget::initUI()
{
    ui->label_2->setText(m_storageInfo.rack_name);
    ui->deviation_x_SpinBox->setValue(m_storageInfo.deviation_x);
    ui->deviation_y_SpinBox->setValue(m_storageInfo.deviation_y);
    ui->deviation_z_SpinBox->setValue(m_storageInfo.deviation_z);
}

void DP8PcrWidget::onUpdateClawInfo(ENUM_CLASS::Gantry gantry)
{
    if(gantry != m_clawInfo.gantry_id)
    {
        return;
    }

    m_clawInfo = DBManager::instance()->getClawInfoByGantryId(m_clawInfo.gantry_id);

    //组合为夹爪相对位置最终值
    m_clawInfo.distance_end_x = m_clawInfo.distance_x - m_clawInfo.deviation_x;
    m_clawInfo.distance_end_y = m_clawInfo.distance_y - m_clawInfo.deviation_y;
    m_clawInfo.distance_end_z = m_clawInfo.distance_z - m_clawInfo.deviation_z;
}

void DP8PcrWidget::onUpdatePipetteDeviation(ENUM_CLASS::Gantry gantry)
{
    if(gantry != m_pipetteInfo.module_id)
    {
        return;
    }

    m_pipetteInfo = DBManager::instance()->getPipetteInfoByModuleId(m_pipetteInfo.module_id);
}

//运动至PCR校准点
void DP8PcrWidget::on_pushButton_35_clicked()
{
    SportMethods::instance()->moveToPcrAccuratingPosition(m_storageInfo, m_pipetteInfo, m_clawInfo, ui->deviation_x_SpinBox->value(), ui->deviation_y_SpinBox->value(), ui->deviation_z_SpinBox->value());
}

//保存
void DP8PcrWidget::on_btnSava_4_clicked()
{
    m_storageInfo.deviation_x = ui->deviation_x_SpinBox->value();
    m_storageInfo.deviation_y = ui->deviation_y_SpinBox->value();
    m_storageInfo.deviation_z = ui->deviation_z_SpinBox->value();

    if(!DBManager::instance()->setDeviationValue(m_storageInfo))
    {
        MessageDialog msg("保存失败！");
        msg.exec();
        return;
    }
    else
    {
        MessageDialog msg("保存成功！");
        msg.exec();
    }
}

