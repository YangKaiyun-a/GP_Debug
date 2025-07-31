#include "dp8carrierwidget.h"
#include "ui_dp8carrierwidget.h"
#include "Controller/sportmethods.h"
#include "application.h"
#include "Function/data/dbmanager.h"
#include "hcscitoolkits/dialog/messagedialog.h"

#include <QDebug>


DP8CarrierWidget::DP8CarrierWidget(Storage_Info storageInfo, PipetteInfo pipetteInfo, float resetZ0, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DP8CarrierWidget), m_storageInfo(storageInfo), m_pipetteInfo(pipetteInfo), m_resetZ0(resetZ0)
{
    ui->setupUi(this);
    init();
}

DP8CarrierWidget::~DP8CarrierWidget()
{
    delete ui;
}

void DP8CarrierWidget::init()
{
    initStyleSheet();
    initData();
    initUI();
}

void DP8CarrierWidget::initStyleSheet()
{
    ui->label_32->setObjectName("labModuleAction");
}

void DP8CarrierWidget::initData()
{
    connect(hApp->m_sigmanager, &SignalManager::updatePipetteDeviation, this, &DP8CarrierWidget::onUpdatePipetteDeviation);
}

void DP8CarrierWidget::initUI()
{
    ui->label_2->setText(m_storageInfo.rack_name);
    ui->deviation_x_SpinBox->setValue(m_storageInfo.deviation_x);
    ui->deviation_y_SpinBox->setValue(m_storageInfo.deviation_y);
    ui->deviation_z_SpinBox->setValue(m_storageInfo.deviation_z);
}

//重新读取移液器偏差
void DP8CarrierWidget::onUpdatePipetteDeviation(ENUM_CLASS::Gantry gantry)
{
    if(gantry != m_pipetteInfo.module_id)
    {
        return;
    }

    m_pipetteInfo = DBManager::instance()->getPipetteInfoByModuleId(m_pipetteInfo.module_id);
}

//运动至载架校准点
void DP8CarrierWidget::on_pushButton_18_clicked()
{
    SportMethods::instance()->moveToRackAccuratingPositionDP8(m_storageInfo, m_pipetteInfo, ui->deviation_x_SpinBox->value(), ui->deviation_y_SpinBox->value(), m_resetZ0);
}

//运动至载架校准点偏移位
void DP8CarrierWidget::on_pushButton_33_clicked()
{
    SportMethods::instance()->moveToRackAccuratingPositionOffset(m_storageInfo, m_pipetteInfo, ui->deviation_x_SpinBox->value(), ui->deviation_y_SpinBox->value(), m_resetZ0);
}

//保存
void DP8CarrierWidget::on_btnSava_2_clicked()
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

        //保存成功后更新DP8Widget类中的坐标，因修正移液器偏差和校准夹爪需要
        Q_EMIT hApp->m_sigmanager->updateRackDeviation(m_storageInfo.rack_id);
    }
}

