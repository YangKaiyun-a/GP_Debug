#include "pipettewidget.h"
#include "ui_pipettewidget.h"
#include "Controller/Amplification_Dj1/amplification_dj1.h"
#include "Controller/Extract_Dj1/extract_dj1.h"
#include "Controller/Dp1/dp1.h"
#include "Controller/Dp8/dp8.h"
#include "Controller/mqttexecutor.h"
#include "Function/data/dbmanager.h"
#include "hcscitoolkits/dialog/messagedialog.h"
#include "Function/data/parsemessageinfo.h"
#include "Controller/mqttexecutor.h"
#include "Function/data/mqttcommunication.h"
#include "Controller/Switch_Signal/switch_signal.h"

#include <QDir>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

PipetteWidget::PipetteWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PipetteWidget)
{
    ui->setupUi(this);
    init();
}

PipetteWidget::~PipetteWidget()
{
    delete ui;
}

void PipetteWidget::init()
{
    initData();
    initStyleSheet();
    initConnect();
}

void PipetteWidget::initData()
{
    //注册类型
    qRegisterMetaType<ENUM_CLASS::PCR_Id> ("ENUM_CLASS::PCR_Id");
    qRegisterMetaType<ENUM_CLASS::Position_Module> ("ENUM_CLASS::Position_Module");

    //读取流程文件
    QString folderPath = JSON_FILE_PATH("workFlowConfig");

    QDir dir(folderPath);
    if (!dir.exists())
    {
        qDebug() << "文件夹不存在：" << folderPath;
        return;
    }

    QStringList files = dir.entryList(QDir::Files);

    foreach(const QString& fileName, files)
    {
        QStringList parts = fileName.split('_');

        if(parts.size() >= 3)
        {
            QString displayName = fileName.chopped(5);
            m_workFlowMap[parts[0]].append(displayName);
        }
    }

    ui->comboBox_DJ1->addItems(m_workFlowMap.value("DJ1"));
    ui->comboBox_DJ2->addItems(m_workFlowMap.value("DJ2"));
    ui->comboBox_DP1->addItems(m_workFlowMap.value("DP1"));
    ui->comboBox_DP8->addItems(m_workFlowMap.value("DP8"));

    m_stageErrorMsg.insert(Load_Carrier, "加载载架失败！");
    m_stageErrorMsg.insert(Load_Project, "加载流程失败！");
    m_stageErrorMsg.insert(Load_Process_Table, "加载台面失败！");
    m_stageErrorMsg.insert(Run_Project, "启动流程失败！");
}

void PipetteWidget::initStyleSheet()
{
    ui->widget_7->setObjectName("stepsWidget");
    ui->widget_8->setObjectName("stepsWidget");
    ui->widget_9->setObjectName("stepsWidget");
    ui->widget_10->setObjectName("stepsWidget");
    ui->widget_11->setObjectName("stepsWidget");
    ui->widget_12->setObjectName("stepsWidget");
    ui->widget_13->setObjectName("stepsWidget");
    ui->widget_14->setObjectName("stepsWidget");

    ui->label_31->setObjectName("labModuleAction");
    ui->label_32->setObjectName("labModuleAction");
    ui->label_44->setObjectName("labModuleAction");
    ui->label_55->setObjectName("labModuleAction");
}

void PipetteWidget::initConnect()
{
    connect(MqttCommunication::instance(), &MqttCommunication::handleMiddleInfo, this, &PipetteWidget::onHandleMiddleInfo);
    //DJ1泵阀控制
    connect(ui->swControlDuct_2, &SwitchButton::statusChanged, this, &PipetteWidget::onStatusChanged_2);
    connect(ui->swControlDuct_3, &SwitchButton::statusChanged, this, &PipetteWidget::onStatusChanged_3);
    connect(ui->swControlDuct_4, &SwitchButton::statusChanged, this, &PipetteWidget::onStatusChanged_4);
    connect(ui->swControlDuct_5, &SwitchButton::statusChanged, this, &PipetteWidget::onStatusChanged_5);
    connect(ui->swControlDuct_6, &SwitchButton::statusChanged, this, &PipetteWidget::onStatusChanged_6);
    connect(ui->swControlDuct_7, &SwitchButton::statusChanged, this, &PipetteWidget::onStatusChanged_7);
    connect(ui->swControlDuct_8, &SwitchButton::statusChanged, this, &PipetteWidget::onStatusChanged_8);
    //DJ2泵阀控制
    connect(ui->swControlDuct_10, &SwitchButton::statusChanged, this, &PipetteWidget::onStatusChanged_10);
    connect(ui->swControlDuct_11, &SwitchButton::statusChanged, this, &PipetteWidget::onStatusChanged_11);
    connect(ui->swControlDuct_13, &SwitchButton::statusChanged, this, &PipetteWidget::onStatusChanged_13);
    connect(ui->swControlDuct_14, &SwitchButton::statusChanged, this, &PipetteWidget::onStatusChanged_14);
    connect(ui->swControlDuct_16, &SwitchButton::statusChanged, this, &PipetteWidget::onStatusChanged_16);
    connect(ui->swControlDuct_9, &SwitchButton::statusChanged, this, &PipetteWidget::onStatusChanged_9);
    connect(ui->swControlDuct_12, &SwitchButton::statusChanged, this, &PipetteWidget::onStatusChanged_12);
    //DP8泵阀控制
    connect(ui->swControlDuct_15, &SwitchButton::statusChanged, this, &PipetteWidget::onStatusChanged_15);
}

//发送流程的处理逻辑
void PipetteWidget::handleRunWorkFlow(ENUM_CLASS::Position_Module pipiet_id, const QString &workFlowName)
{
    m_currentWorkFlowInfo.currentPipette = pipiet_id;
    m_currentWorkFlowInfo.workFlowName = workFlowName;
    m_currentWorkFlowInfo.currentStage = Load_Carrier;
    m_currentWorkFlowInfo.workflowJson = ParseMessageInfo::getWorkFlowJson(workFlowName);

    //发送载架
    sendCarrier();
}

//发送载架
void PipetteWidget::sendCarrier()
{
    if(!MqttExecutor::instance()->publishCarrier(ENUM_CLASS::Middle, m_currentWorkFlowInfo.workflowJson))
    {
        MessageDialog msg("下发载架失败，通信失败");
        msg.exec();
    }
}

//发送流程
void PipetteWidget::sendWorkFlow()
{
    if(!MqttExecutor::instance()->publishTestProject(ENUM_CLASS::Middle, m_currentWorkFlowInfo.workflowJson))
    {
        MessageDialog msg("下发流程失败，通信失败");
        msg.exec();
    }
}

//发送台面
void PipetteWidget::sendProcessTable()
{
    if(!MqttExecutor::instance()->publishProcessTable(ENUM_CLASS::Middle, m_currentWorkFlowInfo.currentPipette, m_currentWorkFlowInfo.workFlowName))
    {
        MessageDialog msg("下发台面失败，通信失败");
        msg.exec();
    }
}

//开启实验
void PipetteWidget::sendRunProject()
{
    if(!MqttExecutor::instance()->pubishRunProject(ENUM_CLASS::Middle))
    {
        MessageDialog msg("开启实验失败，通信失败");
        msg.exec();
    }
}

void PipetteWidget::onHandleMiddleInfo(const MiddleMessageInfo &info, ENUM_CLASS::PCR_Id id)
{
    if(id != ENUM_CLASS::Middle || m_currentWorkFlowInfo.currentStage == Uninit)
    {
        return;
    }

    //错误的情况
    if(info.status != 0)
    {
        MessageDialog msg(m_stageErrorMsg.value(m_currentWorkFlowInfo.currentStage));
        msg.exec();

        m_currentWorkFlowInfo.currentStage = Uninit;

        return;
    }

    //成功的情况
    if(m_currentWorkFlowInfo.currentStage < Run_Project)
    {
        m_currentWorkFlowInfo.currentStage += 1;

        switch (m_currentWorkFlowInfo.currentStage)
        {
        case Load_Project:
            sendWorkFlow();
            break;
        case Load_Process_Table:
            sendProcessTable();
            break;
        case Run_Project:
            sendRunProject();
            break;
        default:
            break;
        }
    }
    else
    {
        MessageDialog msg("流程启动成功！");
        msg.exec();

        m_currentWorkFlowInfo.currentStage = Uninit;

        return;
    }
}


/**********************************************DJ1**********************************************/

//DJ1柱塞泵复位
void PipetteWidget::on_pushButton_17_clicked()
{
    Amplification_Dj1::instance()->motorReset(Amplification_Dj1::MOTOR_PIPET);
}

//DJ1模块复位
void PipetteWidget::on_pushButton_18_clicked()
{
    Amplification_Dj1::instance()->module_reset();
}

//DJ1移液
void PipetteWidget::on_pushButton_26_clicked()
{
    handleRunWorkFlow(ENUM_CLASS::DJ1, ui->comboBox_DJ1->currentText());
}

//kb保存
void PipetteWidget::on_pushButton_28_clicked()
{
    if(DBManager::instance()->saveMaterialStrategy(ui->comboBox_DJ1->currentText(), ui->dj1_k->value(), ui->dj1_b->value()))
    {
        MessageDialog msg("保存成功");
        msg.exec();
    }
    else
    {
        MessageDialog msg("保存失败");
        msg.exec();
    }
}

void PipetteWidget::on_comboBox_DJ1_currentTextChanged(const QString &arg1)
{
    QVector<float> vector = DBManager::instance()->getMaterialStrategy(arg1);

    if(vector.isEmpty())
    {
        ui->pushButton_28->setEnabled(false);
        ui->label_10->setText("未找到该流程的补偿策略：" + ParseMessageInfo::getMaterialStrategy(arg1));
        return;
    }

    ui->pushButton_28->setEnabled(true);
    ui->label_10->setText("");

    ui->dj1_k->setValue(vector.at(0));
    ui->dj1_b->setValue(vector.at(1));
}

void PipetteWidget::onStatusChanged_2(bool status)
{
    if(status)
    {
        //废液泵打开
        qDebug() << "DJ1废液泵打开";
        Amplification_Dj1::instance()->open_waste_pump();
    }
    else
    {
        //废液泵关闭
        qDebug() << "DJ1废液泵关闭";
        Amplification_Dj1::instance()->close_waste_pump();
    }
}

void PipetteWidget::onStatusChanged_3(bool status)
{
    if(status)
    {
        //内壁阀打开
        qDebug() << "DJ1内壁阀打开";
        Amplification_Dj1::instance()->open_inside_valve();
    }
    else
    {
        //内壁阀关闭
        qDebug() << "DJ1内壁阀关闭";
        Amplification_Dj1::instance()->close_inside_valve();
    }
}

void PipetteWidget::onStatusChanged_4(bool status)
{
    if(status)
    {
        //清洗阀打开
        qDebug() << "DJ1清洗阀打开";
        Amplification_Dj1::instance()->open_clean_valve();
    }
    else
    {
        //清洗阀关闭
        qDebug() << "DJ1清洗阀关闭";
        Amplification_Dj1::instance()->close_clean_valve();
    }
}

void PipetteWidget::onStatusChanged_5(bool status)
{
    if(status)
    {
        //外壁阀打开
        qDebug() << "DJ1外壁阀打开";
        Amplification_Dj1::instance()->open_outside_valve();
    }
    else
    {
        //外壁阀关闭
        qDebug() << "DJ1外壁阀关闭";
        Amplification_Dj1::instance()->close_outside_valve();
    }
}

void PipetteWidget::onStatusChanged_6(bool status)
{
    if(status)
    {
        //内壁泵打开
        qDebug() << "DJ1内壁泵打开";
        Amplification_Dj1::instance()->open_inside_pump();
    }
    else
    {
        //内壁泵关闭
        qDebug() << "DJ1内壁泵关闭";
        Amplification_Dj1::instance()->close_inside_pump();
    }
}

void PipetteWidget::onStatusChanged_7(bool status)
{
    if(status)
    {
        //清洗泵打开
        qDebug() << "DJ1清洗泵打开";
        Amplification_Dj1::instance()->open_clean_pump();
    }
    else
    {
        //清洗泵关闭
        qDebug() << "DJ1清洗泵关闭";
        Amplification_Dj1::instance()->close_clean_pump();
    }
}

void PipetteWidget::onStatusChanged_8(bool status)
{
    if(status)
    {
        //外壁泵打开
        qDebug() << "DJ1外壁泵打开";
        Amplification_Dj1::instance()->open_outside_pump();
    }
    else
    {
        //外壁泵关闭
        qDebug() << "DJ1外壁泵关闭";
        Amplification_Dj1::instance()->close_outside_pump();
    }
}


/**********************************************DJ2**********************************************/


//DJ2柱塞泵复位
void PipetteWidget::on_pushButton_29_clicked()
{
    Extract_DJ1::instance()->motorReset(Extract_DJ1::MOTOR_BIG_PIPET);
}

//DJ2模块复位
void PipetteWidget::on_pushButton_30_clicked()
{
    Extract_DJ1::instance()->module_reset();
}

//移液
void PipetteWidget::on_pushButton_38_clicked()
{
    handleRunWorkFlow(ENUM_CLASS::DJ2, ui->comboBox_DJ2->currentText());
}

//DJ2保存
void PipetteWidget::on_pushButton_40_clicked()
{
    if(DBManager::instance()->saveMaterialStrategy(ui->comboBox_DJ2->currentText(), ui->dj2_k->value(), ui->dj2_b->value()))
    {
        MessageDialog msg("保存成功");
        msg.exec();
    }
    else
    {
        MessageDialog msg("保存失败");
        msg.exec();
    }
}

void PipetteWidget::on_comboBox_DJ2_currentTextChanged(const QString &arg1)
{
    QVector<float> vector = DBManager::instance()->getMaterialStrategy(arg1);

    if(vector.isEmpty())
    {
        ui->pushButton_40->setEnabled(false);
        ui->label_12->setText("未找到该流程的补偿策略：" + ParseMessageInfo::getMaterialStrategy(arg1));
        return;
    }

    ui->pushButton_40->setEnabled(true);
    ui->label_12->setText("");

    ui->dj2_k->setValue(vector.at(0));
    ui->dj2_b->setValue(vector.at(1));
}

void PipetteWidget::onStatusChanged_10(bool status)
{
    if(status)
    {
        //废液泵打开
        qDebug() << "DJ2废液泵打开";
        Extract_DJ1::instance()->open_waste_pump();
    }
    else
    {
        //废液泵关闭
        qDebug() << "DJ2废液泵关闭";
        Extract_DJ1::instance()->close_waste_pump();
    }
}

void PipetteWidget::onStatusChanged_11(bool status)
{
    if(status)
    {
        //内壁阀打开
        qDebug() << "DJ2内壁阀打开";
        Extract_DJ1::instance()->open_little_inside_valve();
    }
    else
    {
        //内壁阀关闭
        qDebug() << "DJ2内壁阀关闭";
        Extract_DJ1::instance()->close_little_inside_valve();
    }
}

void PipetteWidget::onStatusChanged_13(bool status)
{
    if(status)
    {
        //外壁阀打开
        qDebug() << "DJ2外壁阀打开";
        Extract_DJ1::instance()->open_outside_valve();
    }
    else
    {
        //外壁阀关闭
        qDebug() << "DJ2外壁阀关闭";
        Extract_DJ1::instance()->close_outside_valve();
    }
}

void PipetteWidget::onStatusChanged_14(bool status)
{
    if(status)
    {
        //内壁泵打开
        qDebug() << "DJ2内壁泵打开";
        Extract_DJ1::instance()->open_inside_pump();
    }
    else
    {
        //内壁泵关闭
        qDebug() << "DJ2内壁泵关闭";
        Extract_DJ1::instance()->close_inside_pump();
    }
}

void PipetteWidget::onStatusChanged_16(bool status)
{
    if(status)
    {
        //外壁泵打开
        qDebug() << "DJ2外壁泵打开";
        Extract_DJ1::instance()->open_outside_pump();
    }
    else
    {
        //外壁泵关闭
        qDebug() << "DJ2外壁泵关闭";
        Extract_DJ1::instance()->close_outside_pump();
    }
}

void PipetteWidget::onStatusChanged_9(bool status)
{
    if(status)
    {
        //清洗阀打开
        qDebug() << "DJ2清洗阀打开";
        Extract_DJ1::instance()->open_clean_valve();
    }
    else
    {
        //清洗阀关闭
        qDebug() << "DJ2清洗阀关闭";
        Extract_DJ1::instance()->close_clean_valve();
    }
}

void PipetteWidget::onStatusChanged_12(bool status)
{
    if(status)
    {
        //清洗泵打开
        qDebug() << "DJ2清洗泵打开";
        Extract_DJ1::instance()->open_clean_pump();
    }
    else
    {
        //清洗泵关闭
        qDebug() << "DJ2清洗泵关闭";
        Extract_DJ1::instance()->close_clean_pump();
    }
}

/**********************************************DP1**********************************************/

//DP1模块复位
void PipetteWidget::on_pushButton_42_clicked()
{
    Dp1::instance()->module_reset();
    Dp1_Xy::instance()->module_reset();
}

//移液
void PipetteWidget::on_pushButton_53_clicked()
{
    handleRunWorkFlow(ENUM_CLASS::DP1, ui->comboBox_DP1->currentText());
}

//DP1保存
void PipetteWidget::on_pushButton_55_clicked()
{
    if(DBManager::instance()->saveMaterialStrategy(ui->comboBox_DP1->currentText(), ui->dp1_k->value(), ui->dp1_b->value()))
    {
        MessageDialog msg("保存成功");
        msg.exec();
    }
    else
    {
        MessageDialog msg("保存失败");
        msg.exec();
    }
}

void PipetteWidget::on_comboBox_DP1_currentTextChanged(const QString &arg1)
{
    QVector<float> vector = DBManager::instance()->getMaterialStrategy(arg1);

    if(vector.isEmpty())
    {
        ui->pushButton_55->setEnabled(false);
        ui->label_24->setText("未找到该流程的补偿策略：" + ParseMessageInfo::getMaterialStrategy(arg1));
        return;
    }

    ui->pushButton_55->setEnabled(true);
    ui->label_24->setText("");

    ui->dp1_k->setValue(vector.at(0));
    ui->dp1_b->setValue(vector.at(1));
}


/**********************************************DP8**********************************************/


//DP8模块复位
void PipetteWidget::on_pushButton_44_clicked()
{
    Dp8::instance()->module_reset();
    Dp8_Xy::instance()->module_reset();
}

//DP8保存
void PipetteWidget::on_pushButton_58_clicked()
{
    if(DBManager::instance()->saveMaterialStrategy(ui->comboBox_DP8->currentText(), ui->dp8_k->value(), ui->dp8_b->value()))
    {
        MessageDialog msg("保存成功");
        msg.exec();
    }
    else
    {
        MessageDialog msg("保存失败");
        msg.exec();
    }
}

//整机复位
void PipetteWidget::on_pushButton_45_clicked()
{
    if(!MqttExecutor::instance()->machineRest(ENUM_CLASS::Middle))
    {
        MessageDialog msg("下发失败");
        msg.exec();
        return;
    }

    //等待中位机返回
    MessageDialog *msg = new MessageDialog("正在复位");
    QMetaObject::Connection conn = connect(MqttCommunication::instance(), &MqttCommunication::handleMiddleMachineResetInfo,[=](MiddleMessageInfo info, ENUM_CLASS::PCR_Id id){

        if(ENUM_CLASS::PCR_Id::Middle != id)
        {
            return;
        }

        if(info.status == 0)
        {
            m_currentWorkFlowInfo.currentStage = Uninit;

            msg->setMsg("复位成功！");
            msg->hideSpinner();
            msg->setConfirmText("确定");
            msg->setBtnEnable(true);
        }
        else
        {
            msg->setMsg("复位失败！");
            msg->hideSpinner();
            msg->setConfirmText("确定");
            msg->setBtnEnable(true);
        }
    });

    msg->setConfirmText("");
    msg->showSpinner();
    msg->setBtnEnable(false);
    msg->exec();
    disconnect(conn);
}

//DP8移液
void PipetteWidget::on_pushButton_56_clicked()
{
    handleRunWorkFlow(ENUM_CLASS::DP8, ui->comboBox_DP8->currentText());
}

void PipetteWidget::on_comboBox_DP8_currentTextChanged(const QString &arg1)
{
    QVector<float> vector = DBManager::instance()->getMaterialStrategy(arg1);

    if(vector.isEmpty())
    {
        ui->pushButton_58->setEnabled(false);
        ui->label_13->setText("未找到该流程的补偿策略：" + ParseMessageInfo::getMaterialStrategy(arg1));
        return;
    }

    ui->pushButton_58->setEnabled(true);
    ui->label_13->setText("");

    ui->dp8_k->setValue(vector.at(0));
    ui->dp8_b->setValue(vector.at(1));
}

void PipetteWidget::onStatusChanged_15(bool status)
{
    if(status)
    {
        //废液泵打开
        qDebug() << "DP8废液泵打开";
        Switch_Signal::instance()->waste_pump_open();
    }
    else
    {
        //废液泵关闭
        qDebug() << "DP8废液泵关闭";
        Switch_Signal::instance()->waste_pump_close();
    }
}
