#include "pcrtemperatureaccuracywidget.h"
#include "ui_pcrtemperatureaccuracywidget.h"
#include "Function/Data/mqttcommunication.h"
#include "hcscitoolkits/dialog/messagedialog.h"
#include "Function/data/dbmanager.h"
#include "Controller/mqttexecutor.h"
#include "Function/data/parsemessageinfo.h"


#include <QScrollBar>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QRegularExpressionValidator>
#include <cmath>
#include <QScreen>

//校准位Z运动时间
const QString CAP_Z_MOVE_TIME = "1500";

PCRTemperatureAccuracyWidget::PCRTemperatureAccuracyWidget(ENUM_CLASS::PCR_Id id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PCRTemperatureAccuracyWidget), m_id(id)
{
    ui->setupUi(this);
    init();
}

PCRTemperatureAccuracyWidget::~PCRTemperatureAccuracyWidget()
{
    delete ui;
}

void PCRTemperatureAccuracyWidget::init()
{
    initStyleSheet();
    initScrollBar();
    initTableWidget();
    initTemperatureWidget();
    initConnect();
}

void PCRTemperatureAccuracyWidget::initStyleSheet()
{
    ui->line->setObjectName("whiteDashLine");

    ui->label_2->setObjectName("labModuleDataAction");
    ui->line_2->setObjectName("blueDashLine");

    ui->label_3->setObjectName("labModuleDataAction");
    ui->line_3->setObjectName("blueDashLine");

    //正则表达式：只能输入整数/小数
    QRegExp regExp("^-?\\d+(\\.\\d+)?$");
    ui->lineEdit_5->setValidator(new QRegExpValidator(regExp, this));
    ui->lineEdit_6->setValidator(new QRegExpValidator(regExp, this));
    ui->lineEdit_7->setValidator(new QRegExpValidator(regExp, this));
    ui->lineEdit_9->setValidator(new QRegExpValidator(regExp, this));
    ui->editK->setValidator(new QRegExpValidator(regExp, this));
    ui->editB->setValidator(new QRegExpValidator(regExp, this));

    ui->comboBox->addItems(QStringList() << "温度K、B补偿方案" << "均一性方案" << "升降温速率方案");
}

void PCRTemperatureAccuracyWidget::initScrollBar()
{
    ui->scrollArea->verticalScrollBar()->hide();
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    if(!m_timer)
    {
        m_timer = new QTimer(this);
    }

    connect(m_timer, &QTimer::timeout, [=]() {
        ui->scrollArea->verticalScrollBar()->hide();
        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_timer->stop();
    });

    ui->scrollArea->viewport()->installEventFilter(this);
}

void PCRTemperatureAccuracyWidget::initConnect()
{
    connect(ui->swControlDuct, &SwitchButton::statusChanged, this, &PCRTemperatureAccuracyWidget::onStatusChanged);
    connect(ui->swControlDuct_2, &SwitchButton::statusChanged, this, &PCRTemperatureAccuracyWidget::onStatusChanged_2);
    connect(MqttCommunication::instance(), &MqttCommunication::handleMessageInfo, this, &PCRTemperatureAccuracyWidget::onHandleSetParams);
    connect(MqttCommunication::instance(), &MqttCommunication::handleExperimentFinished, this, &PCRTemperatureAccuracyWidget::onHandleFinished);

    //为了使SwitchButton::statusChanged生效，特意放在connect后
    ui->swControlDuct_2->setChecked(false);
}

void PCRTemperatureAccuracyWidget::initTableWidget()
{
    //列宽自适应
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableWidget->horizontalHeader()->setMinimumHeight(50);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
}

void PCRTemperatureAccuracyWidget::initTemperatureWidget()
{
    if(!m_temperatureWidget)
    {
        m_temperatureWidget = new TemperatureWidget(m_id, this);
    }

    m_temperatureWidget->hide();
}

//发送sample参数
bool PCRTemperatureAccuracyWidget::setSampleParams()
{
    SampleParams params;
    params.k = ui->doubleSpinBox->value();
    params.b = ui->doubleSpinBox_2->value();
    params.duration = ui->spinBox->value();

    if(!MqttExecutor::instance()->setSampleParams(m_id, params))
    {
        MessageDialog msg("下发Sample参数超时！");
        msg.exec();
        return false;
    }

    bool result = false;
    MessageDialog *msg = new MessageDialog("下发Sample参数...");
    QMetaObject::Connection conn = connect(MqttCommunication::instance(), &MqttCommunication::handleMessageInfo, [&](MessageInfo info, ENUM_CLASS::PCR_Id id) {

        if(m_id != id)
        {
            return;
        }

        if(info.result == "ok" && info.command == "set_sample_param")
        {
            msg->setMsg("下发Sample成功！");
            msg->hideSpinner();
            msg->setConfirmText("确定");
            msg->setBtnEnable(true);

            result = true;

            msg->click();
        }
        else if(info.result == "error"  && info.command == "set_sample_param")
        {
            msg->setMsg("下发Sample参数失败！");
            msg->hideSpinner();
            msg->setConfirmText("确定");
            msg->setBtnEnable(true);

            result = false;
        }
    });
    msg->setConfirmText("");
    msg->showSpinner();
    msg->setBtnEnable(false);
    msg->exec();
    disconnect(conn);

    return result;
}

//下发方案
bool PCRTemperatureAccuracyWidget::setScheme()
{
    if(!MqttExecutor::instance()->setCurrentScheme(m_id, m_currentPcrConfig, ui->comboBox->currentText(), ui->swControlDuct_2->checked()))
    {
        MessageDialog msg("下发方案超时！");
        msg.exec();
        return false;
    }

    bool result = false;
    MessageDialog *msg = new MessageDialog("下发方案...");
    QMetaObject::Connection conn = connect(MqttCommunication::instance(), &MqttCommunication::handleMessageInfo, [&](MessageInfo info, ENUM_CLASS::PCR_Id id) {

        if(m_id != id)
        {
            return;
        }

        if(info.result == "ok" && info.command == "set_schema")
        {
            msg->setMsg("下发成功！");
            msg->hideSpinner();
            msg->setConfirmText("确定");
            msg->setBtnEnable(true);

            result = true;

            msg->click();

            //记录此时实验名称
            m_currentSchemeName = ui->comboBox->currentText();
        }
        else if(info.result == "error"  && info.command == "set_schema")
        {
            msg->setMsg("模块出现错误！");
            msg->hideSpinner();
            msg->setConfirmText("确定");
            msg->setBtnEnable(true);

            result = false;
        }
    });
    msg->setConfirmText("");
    msg->showSpinner();
    msg->setBtnEnable(false);
    msg->exec();
    disconnect(conn);

    return result;
}

//执行方案
bool PCRTemperatureAccuracyWidget::runScheme()
{
    if(!MqttExecutor::instance()->runScheme(m_id, 1))
    {
        MessageDialog msg("执行方案超时！");
        msg.exec();
        return false;
    }

    bool result = false;
    MessageDialog *msg = new MessageDialog("启动中...");
    QMetaObject::Connection conn = connect(MqttCommunication::instance(), &MqttCommunication::handleMessageInfo, [&](MessageInfo info, ENUM_CLASS::PCR_Id id) {

        if(m_id != id)
        {
            return;
        }

        if(info.result == "ok" && info.command == "run_status")
        {
            msg->setMsg("启动成功！");
            msg->hideSpinner();
            msg->setConfirmText("确定");
            msg->setBtnEnable(true);

            result = true;
        }
        else if(info.result == "error" && info.command == "run_status")
        {
            msg->setMsg("模块出现错误！");
            msg->hideSpinner();
            msg->setConfirmText("确定");
            msg->setBtnEnable(true);

            result = false;
        }
    });

    msg->setConfirmText("");
    msg->showSpinner();
    msg->setBtnEnable(false);
    msg->exec();
    disconnect(conn);

    return result;
}

void PCRTemperatureAccuracyWidget::updateTableWidget(QVector<PcrSteps> steps)
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    if(steps.isEmpty())
    {
        return;
    }

    ui->tableWidget->setRowCount(steps.size());

    for(int i = 0; i < steps.size(); ++i)
    {
        //第一列 - 不可编辑
        QTableWidgetItem *textItem1 = new QTableWidgetItem(QString::number(steps[i].index));
        textItem1->setFlags(textItem1->flags() & ~Qt::ItemIsEditable);
        textItem1->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 0, textItem1);

        //第二列 - 不可编辑
        QTableWidgetItem *textItem2 = new QTableWidgetItem(QString::number(steps[i].temperature));
        textItem2->setFlags(textItem2->flags() & ~Qt::ItemIsEditable);
        textItem2->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 1, textItem2);

        //第三列 - 不可编辑
        QTableWidgetItem *textItem3 = new QTableWidgetItem(QString::number(steps[i].duration));
        textItem3->setFlags(textItem3->flags() & ~Qt::ItemIsEditable);
        textItem3->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 2, textItem3);

        //第四列 - 可编辑
        QTableWidgetItem *textItem4 = new QTableWidgetItem("");
        ui->tableWidget->setItem(i, 3, textItem4);
    }
}

void PCRTemperatureAccuracyWidget::showTemperatureWidget()
{
    QSize mainWidgetSize = this->size();
    QSize temperWidgetSize(mainWidgetSize.width() * 0.85, mainWidgetSize.height() * 0.85);
    m_temperatureWidget->resize(temperWidgetSize);

    //获取中心点
    QPoint mainWidgetCenter = this->rect().center();

    //计算左上角位置
    QPoint temperWidgetTopLeft(mainWidgetCenter.x() - temperWidgetSize.width() / 2, mainWidgetCenter.y() - temperWidgetSize.height() / 2);
    m_temperatureWidget->move(temperWidgetTopLeft);

    m_temperatureWidget->show();
}

//托盘推入
void PCRTemperatureAccuracyWidget::blockIn()
{
    //获取拍照位
    BlockParams blockParam = DBManager::instance()->getBlockPosition(m_id);

    //托盘推入到原点位置
    blockToOriginPoint();

    //托盘运动到拍照位
    blockToPhotoPosition(blockParam.getPhotoPosition.toFloat());
}

//托盘推入到原点位置
void PCRTemperatureAccuracyWidget::blockToOriginPoint()
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

//托盘运动到拍照位
void PCRTemperatureAccuracyWidget::blockToPhotoPosition(float position)
{
    if(!MqttExecutor::instance()->motorMove(m_id, ENUM_CLASS::Block, position))
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

//热盖下压
void PCRTemperatureAccuracyWidget::heatLidDown()
{
    //获取顶升位参数
    float position = DBManager::instance()->getCapUpPosition(m_id);

    //热盖下压
    if(!MqttExecutor::instance()->motorMove(m_id, ENUM_CLASS::Cap, position))
    {
        MessageDialog msg("热盖下压超时！");
        msg.exec();
        return;
    }

    MessageDialog *msg2 = new MessageDialog("运动至顶升位...");
    QMetaObject::Connection conn2 = connect(MqttCommunication::instance(), &MqttCommunication::handleMessageInfo, [&](MessageInfo info, ENUM_CLASS::PCR_Id id){

        if(m_id != id)
        {
            return;
        }

        if(info.result == "ok" && info.command == "move_cap")
        {
            msg2->setMsg("执行成功！");
            msg2->hideSpinner();
            msg2->setConfirmText("确定");
            msg2->setBtnEnable(true);
        }
        else if(info.result == "error" && info.command == "move_cap")
        {
            msg2->setMsg("模块出现错误！");
            msg2->hideSpinner();
            msg2->setConfirmText("确定");
            msg2->setBtnEnable(true);
        }
    });
    msg2->setConfirmText("");
    msg2->showSpinner();
    msg2->setBtnEnable(false);
    msg2->exec();
    disconnect(conn2);
}

//保存热盖补偿值
bool PCRTemperatureAccuracyWidget::saveCapTemOffset()
{
    if(!MqttExecutor::instance()->setCapTemOffset(m_id, ui->lineEdit_6->text()))
    {
        return false;
    }

    return true;
}

//保存传感器补偿值
bool PCRTemperatureAccuracyWidget::saveBlockTemOffset()
{
    BlockTemOffset a = DBManager::instance()->getBlockTemOffset(m_id);

    if(a.error)
    {
        return false;
    }

    a.temp1 = ui->lineEdit_7->text();
    a.temp2 = ui->lineEdit_9->text();

    if(!MqttExecutor::instance()->setConsistencyOffset(m_id, a))
    {
        return false;
    }

    return true;
}

//保存KB
bool PCRTemperatureAccuracyWidget::saveBlockCompensate()
{
    BlockCompenSate a;
    a.k = ui->editK->text();
    a.b = ui->editB->text();

    if(!MqttExecutor::instance()->setAccuracyOffset(m_id, a))
    {
        return false;
    }

    return true;
}

bool PCRTemperatureAccuracyWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->scrollArea->viewport() && event->type() == QEvent::Wheel)
    {
        ui->scrollArea->verticalScrollBar()->show();
        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

        if(m_timer)
        {
            m_timer->start(1000);
        }
    }
    return QWidget::eventFilter(watched, event);
}

//处理设置参数的中位机返回
void PCRTemperatureAccuracyWidget::onHandleSetParams(const MessageInfo &info, ENUM_CLASS::PCR_Id id)
{
    if(m_id != id)
    {
        return;
    }

    if(info.result == "ok")
    {
        if(info.uuid == "set_cap_offset")
        {
            m_setCapTemOffsetStatus = ENUM_CLASS::SetParams_Status::Success;
        }
        else if(info.uuid == "set_consistency_offset")
        {
            m_setBlockTemOffsetStatus = ENUM_CLASS::SetParams_Status::Success;
        }
        else if(info.uuid == "set_accuracy_offset")
        {
            m_setBlockCompensateStatus = ENUM_CLASS::SetParams_Status::Success;
        }
    }
    else
    {
        if(info.uuid == "set_cap_offset")
        {
            m_setCapTemOffsetStatus = ENUM_CLASS::SetParams_Status::failed;
        }
        else if(info.uuid == "set_consistency_offset")
        {
            m_setBlockTemOffsetStatus = ENUM_CLASS::SetParams_Status::failed;
        }
        else if(info.uuid == "set_accuracy_offset")
        {
            m_setBlockCompensateStatus = ENUM_CLASS::SetParams_Status::failed;
        }
    }

    int overallStatus = std::min({m_setCapTemOffsetStatus, m_setBlockTemOffsetStatus, m_setBlockCompensateStatus});

    if(overallStatus == ENUM_CLASS::SetParams_Status::Success)
    {
        MessageDialog msg("保存成功！");
        msg.exec();

        m_setCapTemOffsetStatus = ENUM_CLASS::SetParams_Status::Uninit;
        m_setBlockTemOffsetStatus = ENUM_CLASS::SetParams_Status::Uninit;
        m_setBlockCompensateStatus = ENUM_CLASS::SetParams_Status::Uninit;
    }
    else if(overallStatus == ENUM_CLASS::SetParams_Status::failed)
    {
        MessageDialog msg("保存失败！");
        msg.exec();

        m_setCapTemOffsetStatus = ENUM_CLASS::SetParams_Status::Uninit;
        m_setBlockTemOffsetStatus = ENUM_CLASS::SetParams_Status::Uninit;
        m_setBlockCompensateStatus = ENUM_CLASS::SetParams_Status::Uninit;
    }
}

void PCRTemperatureAccuracyWidget::onHandleFinished(const MessageInfo &info, ENUM_CLASS::PCR_Id id)
{
    if(m_id != id || info.status.isEmpty() || m_currentSchemeName != info.experiment_name)
    {
        return;
    }

    QString result = "";
    if(info.status == "complete")
    {
        result = "已完成";
    }
    else if(info.status == "abnormal")
    {
        result = "发生异常";
    }
    else
    {
        result = "发生未知错误";
    }

    MessageDialog msg(m_currentSchemeName + result);
    msg.exec();

    m_currentSchemeName.clear();
}

//执行金属浴方案
void PCRTemperatureAccuracyWidget::on_pushButton_13_clicked()
{
    //发送Sample参数
    if(ui->swControlDuct_2->checked())
    {
        if(!setSampleParams())
        {
            return;
        }
    }

    //发送方案
    if(!setScheme())
    {
        return;
    }

    //开启实验
    runScheme();
}

//停止金属浴方案
void PCRTemperatureAccuracyWidget::on_pushButton_12_clicked()
{
    if(!MqttExecutor::instance()->runScheme(m_id, 0))
    {
        MessageDialog msg("停止失败！");
        msg.exec();
        return;
    }

    //等待中位机返回
    MessageDialog *msg = new MessageDialog("正在停止");
    QMetaObject::Connection conn = connect(MqttCommunication::instance(), &MqttCommunication::handleMessageInfo,[=](MessageInfo info, ENUM_CLASS::PCR_Id id){

        if(m_id != id)
        {
            return;
        }

        if(info.command == "run_status" && info.result == "ok")
        {
            msg->setMsg("停止成功！");
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

//热盖加热
void PCRTemperatureAccuracyWidget::onStatusChanged(bool startHeat)
{
    //先判断是否为空
    if(ui->lineEdit_5->text().isEmpty())
    {
        MessageDialog msg("输入温度不能为空！");
        msg.exec();
        ui->swControlDuct->setChecked(false);
        return;
    }

    int isRun = -1;

    if(startHeat)
    {
        isRun = 1;      //0：停止，1开始
    }
    else
    {
        isRun = 0;
    }

    if(!MqttExecutor::instance()->runCap(m_id, isRun, ui->lineEdit_5->text().toInt()))
    {
        MessageDialog msg("模块连接超时！");
        msg.exec();
        return;
    }

    //等待中位机返回
    MessageDialog *msg = new MessageDialog("正在执行...");
    QMetaObject::Connection conn = connect(MqttCommunication::instance(), &MqttCommunication::handleMessageInfo,[=](MessageInfo info, ENUM_CLASS::PCR_Id id){

        if(m_id != id)
        {
            return;
        }

        if(info.command == "cap_status" && info.result == "ok")
        {
            msg->setMsg("执行成功！");
            msg->hideSpinner();
            msg->setConfirmText("确定");
            msg->setBtnEnable(true);
        }
        else
        {
            msg->setMsg("执行失败！");
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

void PCRTemperatureAccuracyWidget::onStatusChanged_2(bool enableSample)
{
    if(enableSample)
    {
        //开启sample模式
        ui->labK_2->show();
        ui->doubleSpinBox->show();

        ui->labK_3->show();
        ui->doubleSpinBox_2->show();

        ui->labK_4->show();
        ui->spinBox->show();
        ui->labK_5->show();
    }
    else
    {
        //关闭sample模式
        ui->labK_2->hide();
        ui->doubleSpinBox->hide();

        ui->labK_3->hide();
        ui->doubleSpinBox_2->hide();

        ui->labK_4->hide();
        ui->spinBox->hide();
        ui->labK_5->hide();
    }
}

//读取传感器补偿值、KB
void PCRTemperatureAccuracyWidget::on_pushButton_17_clicked()
{
    QString a = DBManager::instance()->getCapTemOffset(m_id);
    if(a.isEmpty())
    {
        MessageDialog msg("获取热盖补偿值失败！");
        msg.exec();
        return;
    }

    ui->lineEdit_6->setText(a);


    BlockTemOffset b = DBManager::instance()->getBlockTemOffset(m_id);
    if(b.error)
    {
        MessageDialog msg("获取传感器补偿值失败！");
        msg.exec();
        return;
    }

    ui->lineEdit_7->setText(b.temp1);
    ui->lineEdit_9->setText(b.temp2);

    //获取K、B
    BlockCompenSate c = DBManager::instance()->getBlockCompenSate(m_id);
    if(c.error)
    {
        MessageDialog msg("获取K、B参数失败！");
        msg.exec();
        return;
    }

    ui->editK->setText(c.k);
    ui->editB->setText(c.b);
}

//保存
void PCRTemperatureAccuracyWidget::on_pushButton_9_clicked()
{
    m_setCapTemOffsetStatus = ENUM_CLASS::SetParams_Status::Uninit;
    m_setBlockTemOffsetStatus = ENUM_CLASS::SetParams_Status::Uninit;
    m_setBlockCompensateStatus = ENUM_CLASS::SetParams_Status::Uninit;

    //保存热盖补偿值
    if(!saveCapTemOffset())
    {
        MessageDialog msg("热盖补偿值保存失败，模块未连接！");
        msg.exec();
        return;
    }

    //保存传感器补偿值
    if(!saveBlockTemOffset())
    {
        MessageDialog msg("传感器补偿值保存失败，模块未连接！");
        msg.exec();
        return;
    }

    //保存KB
    if(!saveBlockCompensate())
    {
        MessageDialog msg("KB保存失败，模块未连接！");
        msg.exec();
        return;
    }
}

//运动至校准位（托盘推入、热盖下压）
void PCRTemperatureAccuracyWidget::on_pushButton_clicked()
{
    //托盘推入
    blockIn();

    //热盖下压
    heatLidDown();
}

//模块复位
void PCRTemperatureAccuracyWidget::on_pushButton_2_clicked()
{
    if(!MqttExecutor::instance()->moduleReset(m_id))
    {
        MessageDialog msg("模块连接超时！");
        msg.exec();
        return;
    }

    //等待中位机返回
    MessageDialog *msg = new MessageDialog("正在复位");
    QMetaObject::Connection conn = connect(MqttCommunication::instance(), &MqttCommunication::handleMessageInfo, [=](MessageInfo info, ENUM_CLASS::PCR_Id id){

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

void PCRTemperatureAccuracyWidget::on_comboBox_currentTextChanged(const QString &arg1)
{
    m_pcrSteps.clear();
    m_currentPcrConfig.clear();

    //更新当前方案
    if(arg1 == "温度K、B补偿方案")
    {
        m_currentPcrConfig = ParseMessageInfo::readJsonFromFile(JSON_FILE_PATH("temperatureCompensation.json"));
    }
    else if(arg1 == "均一性方案")
    {
        m_currentPcrConfig = ParseMessageInfo::readJsonFromFile(JSON_FILE_PATH("Homogeneity.json"));
    }
    else if(arg1 == "升降温速率方案")
    {
        m_currentPcrConfig = ParseMessageInfo::readJsonFromFile(JSON_FILE_PATH("UpDownTemperature.json"));
    }

    //更新表格内容
    m_pcrSteps = ParseMessageInfo::parsePcrConfigParams(m_currentPcrConfig);
    updateTableWidget(m_pcrSteps);
}

//查看温度曲线
void PCRTemperatureAccuracyWidget::on_pushButton_3_clicked()
{
    if(!m_temperatureWidget)
    {
        return;
    }

    if(!m_temperatureWidget->isVisible())
    {
        showTemperatureWidget();
    }
    else if(m_temperatureWidget->isVisible())
    {
        m_temperatureWidget->hide();
    }
}



