#include "pcrpostionaccuracywidget.h"
#include "ui_pcrpostionaccuracywidget.h"
#include "Function/Data/mqttcommunication.h"
#include "hcscitoolkits/dialog/messagedialog.h"
#include "Function/Data/dbmanager.h"
#include "Controller/mqttexecutor.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <cmath>
#include <QRegExpValidator>

//校准位Z运动时间
const QString CAP_Z_MOVE_TIME = "1500";

//托盘Y轴偏移量运动时间
const QString BLOCK_Y_MOVE_TIME = "1500";

PCRPostionAccuracyWidget::PCRPostionAccuracyWidget(ENUM_CLASS::PCR_Id id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PCRPostionAccuracyWidget), m_id(id)
{
    ui->setupUi(this);
    init();
}

PCRPostionAccuracyWidget::~PCRPostionAccuracyWidget()
{
    delete ui;
}

void PCRPostionAccuracyWidget::init()
{
    ui->label_3->setObjectName("labModuleDataAction");
    ui->line_3->setObjectName("blueDashLine");

    //正则表达式：只能输入整数/小数
    QRegExp regExp("^-?\\d+(\\.\\d+)?$");
    ui->lineEdit_6->setValidator(new QRegExpValidator(regExp, this));
    ui->lineEdit_7->setValidator(new QRegExpValidator(regExp, this));
    ui->lineEdit_8->setValidator(new QRegExpValidator(regExp, this));

    initConnect();
}

void PCRPostionAccuracyWidget::initConnect()
{
    connect(MqttCommunication::instance(), &MqttCommunication::handleMessageInfo, this, &PCRPostionAccuracyWidget::onHandleSetParams);
}

//托盘推入到原点位置
void PCRPostionAccuracyWidget::blockToOriginPoint()
{
    if(!MqttExecutor::instance()->blockMove(m_id, 0))
    {
        MessageDialog msg("模块连接超时！");
        msg.exec();
        return;
    }

    //等待中位机返回
    MessageDialog *msg = new MessageDialog("正在执行");
    QMetaObject::Connection conn = connect(MqttCommunication::instance(), &MqttCommunication::handleMessageInfo,[=](MessageInfo info, ENUM_CLASS::PCR_Id id){

        if(m_id != id)
        {
            return;
        }

        if(info.command == "move_block" && info.result == "ok")
        {
            msg->setMsg("执行成功！");
            msg->hideSpinner();
            msg->setConfirmText("确定");
            msg->setBtnEnable(true);
            msg->click();
        }
        else
        {
            msg->setMsg("模块出现错误！");
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

void PCRPostionAccuracyWidget::blockToPhotoPosition(float position)
{
    if(!MqttExecutor::instance()->motorMove(m_id, ENUM_CLASS::Block, position))
    {
        MessageDialog msg("模块连接超时！");
        msg.exec();
        return;
    }

    //等待中位机返回
    MessageDialog *msg = new MessageDialog("正在执行");
    QMetaObject::Connection conn = connect(MqttCommunication::instance(), &MqttCommunication::handleMessageInfo, [=](MessageInfo info, ENUM_CLASS::PCR_Id id){

        if(m_id != id)
        {
            return;
        }

        if(info.command == "move_block" && info.result == "ok")
        {
            msg->setMsg("执行成功！");
            msg->hideSpinner();
            msg->setConfirmText("确定");
            msg->setBtnEnable(true);
        }
        else
        {
            msg->setMsg("模块出现错误！");
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

//处理设置参数的中位机返回
void PCRPostionAccuracyWidget::onHandleSetParams(const MessageInfo &info, ENUM_CLASS::PCR_Id id)
{
    if(m_id != id)
    {
        return;
    }

    if(info.uuid == "set_photo_position")
    {
        if(info.result == "ok")
        {
            m_setPhotoPositionStatus = ENUM_CLASS::SetParams_Status::Success;
        }
        else
        {
            m_setPhotoPositionStatus = ENUM_CLASS::SetParams_Status::failed;
        }
    }
    else if(info.uuid == "set_compact_position")
    {
        if(info.result == "ok")
        {
            m_setCompactPositionStatus = ENUM_CLASS::SetParams_Status::Success;
        }
        else
        {
            m_setCompactPositionStatus = ENUM_CLASS::SetParams_Status::failed;
        }
    }
    else if(info.uuid == "ipconfig")
    {
        if(info.result == "ok")
        {
            MessageDialog msg("设置ip成功！");
            msg.exec();
        }
        else
        {
            MessageDialog msg("设置ip失败！");
            msg.exec();
        }

        return;
    }

    int overallStatus = std::min({m_setPhotoPositionStatus, m_setCompactPositionStatus});

    if(overallStatus == ENUM_CLASS::SetParams_Status::Success)
    {
        MessageDialog msg("保存成功！");
        msg.exec();

        m_setPhotoPositionStatus = ENUM_CLASS::SetParams_Status::Uninit;
        m_setCompactPositionStatus = ENUM_CLASS::SetParams_Status::Uninit;
    }
    else if(overallStatus == ENUM_CLASS::SetParams_Status::failed)
    {
        MessageDialog msg("保存失败！");
        msg.exec();

        m_setPhotoPositionStatus = ENUM_CLASS::SetParams_Status::Uninit;
        m_setCompactPositionStatus = ENUM_CLASS::SetParams_Status::Uninit;
    }
}

//模块复位
void PCRPostionAccuracyWidget::on_pushButton_clicked()
{
    if(!MqttExecutor::instance()->moduleReset(m_id))
    {
        MessageDialog msg("模块连接超时！");
        msg.exec();
        return;
    }

    //等待中位机返回
    MessageDialog *msg = new MessageDialog("正在复位");
    QMetaObject::Connection conn = connect(MqttCommunication::instance(), &MqttCommunication::handleMessageInfo,[=](MessageInfo info, ENUM_CLASS::PCR_Id id){

        if(m_id != id)
        {
            return;
        }

        if(info.command == "init_dev" && info.result == "ok")
        {
            msg->setMsg("复位成功！");
            msg->hideSpinner();
            msg->setConfirmText("确定");
            msg->setBtnEnable(true);
        }
        else
        {
            msg->setMsg("模块出现错误！");
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


//推入
void PCRPostionAccuracyWidget::on_pushButton_10_clicked()
{
    //获取拍照位
    BlockParams blockParam = DBManager::instance()->getBlockPosition(m_id);

    //托盘推入到原点位置
    blockToOriginPoint();

    //托盘运动到拍照位
    blockToPhotoPosition(blockParam.getPhotoPosition.toFloat());
}

//推出
void PCRPostionAccuracyWidget::on_pushButton_9_clicked()
{
    if(!MqttExecutor::instance()->blockMove(m_id, 1))
    {
        MessageDialog msg("模块连接超时！");
        msg.exec();
        return;
    }

    //等待中位机返回
    MessageDialog *msg = new MessageDialog("正在执行");
    QMetaObject::Connection conn = connect(MqttCommunication::instance(), &MqttCommunication::handleMessageInfo,[=](MessageInfo info, ENUM_CLASS::PCR_Id id){

        if(m_id != id && info.command != "move_block")
        {
            return;
        }

        if(info.result == "ok")
        {
            msg->setMsg("执行成功！");
            msg->hideSpinner();
            msg->setConfirmText("确定");
            msg->setBtnEnable(true);
        }
        else
        {
            msg->setMsg("模块出现错误！");
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

//托盘Y轴复位
void PCRPostionAccuracyWidget::on_pushButton_17_clicked()
{
    if(!MqttExecutor::instance()->motorReset(m_id, ENUM_CLASS::Block))
    {
        MessageDialog msg("模块连接超时！");
        msg.exec();
        return;
    }

    //等待中位机返回
    MessageDialog *msg = new MessageDialog("正在复位");
    QMetaObject::Connection conn = connect(MqttCommunication::instance(), &MqttCommunication::handleMessageInfo,[=](MessageInfo info, ENUM_CLASS::PCR_Id id){

        if(m_id != id)
        {
            return;
        }

        if(info.command == "reset_mechanism" && info.result == "ok" && info.resetObject == "block")
        {
            msg->setMsg("复位成功！");
            msg->hideSpinner();
            msg->setConfirmText("确定");
            msg->setBtnEnable(true);
        }
        else
        {
            msg->setMsg("模块出现错误！");
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

//运动至顶升位
void PCRPostionAccuracyWidget::on_pushButton_11_clicked()
{
    //获取顶升位参数
    float position = DBManager::instance()->getCapUpPosition(m_id);

    if(position == -1.0)
    {
        MessageDialog msg("数据获取失败！");
        msg.exec();
        return;
    }

    if(!MqttExecutor::instance()->motorMove(m_id, ENUM_CLASS::Cap, position))
    {
        MessageDialog msg("模块连接超时！");
        msg.exec();
        return;
    }

    //等待中位机返回
    MessageDialog *msg = new MessageDialog("正在运动");
    QMetaObject::Connection conn = connect(MqttCommunication::instance(), &MqttCommunication::handleMessageInfo,[=](MessageInfo info, ENUM_CLASS::PCR_Id id){

        if(m_id != id)
        {
            return;
        }

        if(info.command == "move_cap" && info.result == "ok")
        {
            msg->setMsg("运动结束！");
            msg->hideSpinner();
            msg->setConfirmText("确定");
            msg->setBtnEnable(true);
        }
        else
        {
            msg->setMsg("模块出现错误！");
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

//热盖Z复位
void PCRPostionAccuracyWidget::on_pushButton_19_clicked()
{
    if(!MqttExecutor::instance()->motorReset(m_id, ENUM_CLASS::Cap))
    {
        MessageDialog msg("模块连接超时！");
        msg.exec();
        return;
    }

    //等待中位机返回
    MessageDialog *msg = new MessageDialog("正在复位");
    QMetaObject::Connection conn = connect(MqttCommunication::instance(), &MqttCommunication::handleMessageInfo,[=](MessageInfo info, ENUM_CLASS::PCR_Id id){

        if(m_id != id)
        {
            return;
        }

        if(info.command == "reset_mechanism" && info.result == "ok" && info.resetObject == "cap")
        {
            msg->setMsg("复位成功！");
            msg->hideSpinner();
            msg->setConfirmText("确定");
            msg->setBtnEnable(true);
        }
        else
        {
            msg->setMsg("模块出现错误！");
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

//获取参数
void PCRPostionAccuracyWidget::on_pushButton_2_clicked()
{
    BlockParams blockParam = DBManager::instance()->getBlockPosition(m_id);

    //推出位
    ui->lineEdit_7->setText(blockParam.blockOutPosition);

    //推入位（拍照位）
    ui->lineEdit_6->setText(blockParam.getPhotoPosition);

    //顶升位
    float position = DBManager::instance()->getCapUpPosition(m_id);

    if(position == -1.0)
    {
        MessageDialog msg("获取顶升位参数失败！");
        msg.exec();
        return;
    }

    ui->lineEdit_8->setText(QString::number(position, 'f', 2));
}

//保存参数
void PCRPostionAccuracyWidget::on_pushButton_18_clicked()
{
    m_setPhotoPositionStatus = ENUM_CLASS::SetParams_Status::Uninit;
    m_setCompactPositionStatus = ENUM_CLASS::SetParams_Status::Uninit;

    //保存推入/推出位
    BlockParams blockParam;
    blockParam.getPhotoPosition = ui->lineEdit_6->text();
    blockParam.blockOutPosition = ui->lineEdit_7->text();

    if(!MqttExecutor::instance()->setBlockPosition(m_id, blockParam))
    {
        MessageDialog msg("模块断开连接！");
        msg.exec();
        return;
    }

    if(!MqttExecutor::instance()->setCompactPosition(m_id, ui->lineEdit_8->text()))
    {
        MessageDialog msg("模块断开连接！");
        msg.exec();
        return;
    }
}

//设置ip
void PCRPostionAccuracyWidget::on_pushButton_3_clicked()
{
    if(ui->lineEdit_9->text().isEmpty())
    {
        MessageDialog msg("ip地址不可为空");
        msg.exec();
        return;
    }

    if(!MqttExecutor::instance()->setIpAddress(m_id, ui->lineEdit_9->text()))
    {
        MessageDialog msg("模块未连接！");
        msg.exec();
    }
}

