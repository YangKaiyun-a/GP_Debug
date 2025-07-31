#include "dp1carrierwidget.h"
#include "ui_dp1carrierwidget.h"
#include "Controller/sportmethods.h"
#include "application.h"
#include "Function/data/dbmanager.h"
#include "hcscitoolkits/dialog/messagedialog.h"
#include "Function/data/parsemessageinfo.h"

#include <QDebug>

DP1CarrierWidget::DP1CarrierWidget(Storage_Info storageInfo, PipetteInfo pipetteInfo, LaserInfo laserInfo, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DP1CarrierWidget), m_storageInfo(storageInfo), m_pipetteInfo(pipetteInfo), m_laserInfo(laserInfo)
{
    ui->setupUi(this);
    init();
}

DP1CarrierWidget::~DP1CarrierWidget()
{
    delete ui;
}

void DP1CarrierWidget::init()
{
    initStyleSheet();
    initData();
    initUI();
}

void DP1CarrierWidget::initStyleSheet()
{
    ui->label_32->setObjectName("labModuleAction");
}

void DP1CarrierWidget::initData()
{
    connect(hApp->m_sigmanager, &SignalManager::updatePipetteDeviation, this, &DP1CarrierWidget::onUpdatePipetteDeviation);
}

void DP1CarrierWidget::initUI()
{
    ui->label_2->setText(m_storageInfo.rack_name);
    ui->deviation_x_SpinBox->setValue(m_storageInfo.deviation_x);
    ui->deviation_y_SpinBox->setValue(m_storageInfo.deviation_y);
    ui->deviation_z_SpinBox->setValue(m_storageInfo.deviation_z);
}

//更新移液器偏差
void DP1CarrierWidget::onUpdatePipetteDeviation(ENUM_CLASS::Gantry gantry)
{
    if(gantry != m_pipetteInfo.module_id)
    {
        return;
    }

    m_pipetteInfo = DBManager::instance()->getPipetteInfoByModuleId(m_pipetteInfo.module_id);
}

//运动至校准点偏移位
void DP1CarrierWidget::on_pushButton_33_clicked()
{
    SportMethods::instance()->moveToRackAccuratingPositionOffset(m_laserInfo, m_storageInfo, m_pipetteInfo, ui->deviation_x_SpinBox->value(), ui->deviation_y_SpinBox->value());
}

//运动至校准点
void DP1CarrierWidget::on_pushButton_18_clicked()
{
    SportMethods::instance()->moveToRackAccuratingPosition(m_laserInfo, m_storageInfo, m_pipetteInfo, ui->deviation_x_SpinBox->value(), ui->deviation_y_SpinBox->value());
}

//保存载架偏差
void DP1CarrierWidget::on_btnSava_2_clicked()
{
    m_storageInfo.deviation_x = ui->deviation_x_SpinBox->value();
    m_storageInfo.deviation_y = ui->deviation_y_SpinBox->value();
    m_storageInfo.deviation_z = ui->deviation_z_SpinBox->value();

    if(m_storageInfo.rack_name == "E1机位")
    {
        Storage_Info h_info;
        h_info.deviation_x = m_storageInfo.deviation_x;
        h_info.deviation_y = m_storageInfo.deviation_y;
        h_info.deviation_z = m_storageInfo.deviation_z;
        h_info.rack_id = ParseMessageInfo::getRackIdByTableIdAndRackName(2, "H机位");

        if(h_info.rack_id == 0)
        {
            MessageDialog msg("H机位id获取失败");
            msg.exec();
            return;
        }

        if(DBManager::instance()->setDeviationValue(m_storageInfo) && DBManager::instance()->setDeviationValue(h_info))
        {
            MessageDialog msg("同时保存E1，H机位偏差成功！");
            msg.exec();

            Q_EMIT hApp->m_sigmanager->updateRackDeviation(m_storageInfo.rack_id);
        }
        else
        {
            MessageDialog msg("同时保存E1，H机位偏差失败！");
            msg.exec();
        }
    }
    else
    {
        if(!DBManager::instance()->setDeviationValue(m_storageInfo))
        {
            MessageDialog msg("保存失败！");
            msg.exec();
        }
        else
        {
            MessageDialog msg("保存成功！");
            msg.exec();

            Q_EMIT hApp->m_sigmanager->updateRackDeviation(m_storageInfo.rack_id);
        }
    }
}

