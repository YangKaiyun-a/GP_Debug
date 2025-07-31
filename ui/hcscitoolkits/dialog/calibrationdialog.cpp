#include "calibrationdialog.h"
#include "ui_calibrationdialog.h"
#include "Function/data/mqttcommunication.h"
#include "Customcontrols/calibrationwidget.h"
#include "Function/Data/global.h"
#include "Controller/mqttexecutor.h"

#include <QPainter>
#include <QStyleOption>
#include <QDebug>

CalibrationDialog::CalibrationDialog(ENUM_CLASS::PCR_Id id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrationDialog), m_id(id)
{
    ui->setupUi(this);
    init();
}

CalibrationDialog::~CalibrationDialog()
{
    delete ui;
}

void CalibrationDialog::init()
{
    setWindowFlag(Qt::FramelessWindowHint);

    for(int i = 0; i < 5; ++i)
    {
        ENUM_CLASS::Light_Channel channel = static_cast<ENUM_CLASS::Light_Channel>(i + 1);
        CalibrationWidget *wgt = new CalibrationWidget(channel);

        ui->stackedWidget->insertWidget(i, wgt);
    }

    ui->stackedWidget->setCurrentIndex(0);

    initConnect();
}

void CalibrationDialog::initConnect()
{
    connect(MqttCommunication::instance(), &MqttCommunication::handleMessageInfo, this, &CalibrationDialog::onHandleMessageInfo);
}

void CalibrationDialog::setBtnEnable(bool enable)
{
    ui->btnConfirm->setEnabled(enable);
}

void CalibrationDialog::showSpinner()
{
    if(!m_spinner)
    {
        m_spinner = new HSpinner(ui->btnConfirm);
    }

    m_spinner->setGeometry(0, 0, ui->btnConfirm->width(), ui->btnConfirm->height());
    m_spinner->show();
    m_spinner->start();
}

void CalibrationDialog::hideSpinner()
{
    if(m_spinner)
    {
        m_spinner->stop();
        m_spinner->hide();
    }
}

void CalibrationDialog::click()
{
    ui->btnConfirm->click();
}

void CalibrationDialog::setConfirmText(const QString &msg)
{
    ui->btnConfirm->setText(msg);
}

CalibrationWidget *CalibrationDialog::getCurrentWidget()
{
    return qobject_cast<CalibrationWidget*>(ui->stackedWidget->currentWidget());
}

void CalibrationDialog::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

//接收校准结果
void CalibrationDialog::onHandleMessageInfo(const MessageInfo &info, ENUM_CLASS::PCR_Id id)
{
    if(m_id != id)
    {
        return;
    }

    if(info.command == "move_block")
    {
        if(info.result == "ok")
        {
            if(m_bockMoveType == TrayIn)
            {
                getCurrentWidget()->setResultMessage("托盘已推入，正在校准...");

                //开始拍照校准
                if(!MqttExecutor::instance()->startFluorescenceCalibration(m_id, m_channel, 1))
                {
                    getCurrentWidget()->setResultMessage("模块超时！");
                }
            }
            else if(m_bockMoveType == TrayOut)
            {
                getCurrentWidget()->setResultMessage("托盘已推出");
                //切换至下一个通道
                ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex() + 1);
                //更新当前通道
                m_channel = static_cast<ENUM_CLASS::Light_Channel>(ui->stackedWidget->currentIndex() + 1);
                hideSpinner();
                setConfirmText("确定");
                setBtnEnable(true);
            }

        }
        else if(info.result == "error")
        {
            getCurrentWidget()->setResultMessage("托盘推入失败");
            return;
        }
    }
    else if(info.command == "calibration")
    {
        if(info.result == "ok")
        {
            getCurrentWidget()->setResultMessage("拍照完成，正在推出...");

            m_bockMoveType = TrayOut;

            //托盘推出
            if(!MqttExecutor::instance()->blockMove(m_id, 1))
            {
                getCurrentWidget()->setResultMessage("模块超时！");
                return;
            }

            if(m_channel == ENUM_CLASS::CY5)
            {
                m_isFinished = true;
            }
        }
        else if(info.result == "error")
        {
            getCurrentWidget()->setResultMessage("模块出现错误！");
        }
    }
}

//下一步
void CalibrationDialog::on_btnConfirm_clicked()
{
    if(m_isFinished)
    {
        //校准结束
        Q_EMIT handleDialogClose(m_id, m_channel);
        QDialog::accept();
        return;
    }

    getCurrentWidget()->setResultMessage("托盘推入中...");

    m_bockMoveType = TrayIn;

    //托盘推入
    if(!MqttExecutor::instance()->blockMove(m_id, 0))
    {
        getCurrentWidget()->setResultMessage("模块超时！");
        return;
    }

    //此时不可点击
    setConfirmText("");
    showSpinner();
    setBtnEnable(false);
}

void CalibrationDialog::on_pushButton_clicked()
{
    Q_EMIT handleDialogClose(m_id, m_channel);
    QDialog::reject();
}

