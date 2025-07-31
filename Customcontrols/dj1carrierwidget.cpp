#include "dj1carrierwidget.h"
#include "ui_dj1carrierwidget.h"
#include "Controller/sportmethods.h"
#include "application.h"
#include "Function/data/dbmanager.h"
#include "hcscitoolkits/dialog/messagedialog.h"

DJ1CarrierWidget::DJ1CarrierWidget(Storage_Info storageInfo, PipetteInfo pipetteInfo, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DJ1CarrierWidget), m_storageInfo(storageInfo), m_pipetteInfo(pipetteInfo)
{
    ui->setupUi(this);
    init();
}

DJ1CarrierWidget::~DJ1CarrierWidget()
{
    delete ui;
}

void DJ1CarrierWidget::init()
{
    initStyleSheet();
    initData();
    initUI();
}

void DJ1CarrierWidget::initStyleSheet()
{
    ui->label_16->setObjectName("labModuleAction");
}

void DJ1CarrierWidget::initData()
{
    connect(hApp->m_sigmanager, &SignalManager::updatePipetteDeviation, this, &DJ1CarrierWidget::onUpdatePipetteDeviation);
}

void DJ1CarrierWidget::initUI()
{
    ui->label_2->setText(m_storageInfo.rack_name);
    ui->deviation_x_SpinBox->setValue(m_storageInfo.deviation_x);
    ui->deviation_y_SpinBox->setValue(m_storageInfo.deviation_y);
    ui->deviation_z_SpinBox->setValue(m_storageInfo.deviation_z);
}

//运动至载架校准位
void DJ1CarrierWidget::on_pushButton_17_clicked()
{
    SportMethods::instance()->moveToRackAccuratingPosition(m_storageInfo, m_pipetteInfo, ui->deviation_x_SpinBox->value(), ui->deviation_y_SpinBox->value(), ui->deviation_z_SpinBox->value());
}

//保存载架偏差
void DJ1CarrierWidget::on_btnSava_clicked()
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

        //因修正移液器偏差和校准夹爪需要
        Q_EMIT hApp->m_sigmanager->updateRackDeviation(m_storageInfo.rack_id);
    }
}

//更新移液器偏差
void DJ1CarrierWidget::onUpdatePipetteDeviation(ENUM_CLASS::Gantry gantry)
{
    if(gantry != m_pipetteInfo.module_id)
    {
        return;
    }

    m_pipetteInfo = DBManager::instance()->getPipetteInfoByModuleId(m_pipetteInfo.module_id);
}
