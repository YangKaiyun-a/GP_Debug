#include "pcrlightaccuracywidget.h"
#include "ui_pcrlightaccuracywidget.h"
#include "Function/Data/mqttcommunication.h"
#include "hcscitoolkits/dialog/messagedialog.h"
#include "Controller/mqttexecutor.h"
#include "Function/data/dbmanager.h"
#include "Function/data/socketserver.h"
#include "Function/data/parsemessageinfo.h"
#include "Function/data/dbmanager.h"

#include <QScrollBar>
#include <QTimer>
#include <QButtonGroup>
#include <QJsonObject>
#include <QJsonDocument>
#include <QRegExp>
#include <QRegExpValidator>
#include <QStandardPaths>
#include <QProcess>
#include <QDir>

//分析软件exe启动路径
const QString ANALYSE_SOFT_PATH = "384FulCal/FluCal.exe";


PCRLightAccuracyWidget::PCRLightAccuracyWidget(ENUM_CLASS::PCR_Id id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PCRLightAccuracyWidget), m_id(id)
{
    ui->setupUi(this);
    init();
}

PCRLightAccuracyWidget::~PCRLightAccuracyWidget()
{
    delete ui;
}

void PCRLightAccuracyWidget::init()
{
    initStyleSheet();
    initScrollBar();
    initTemperatureWidget();
    initConnect();
}

void PCRLightAccuracyWidget::initConnect()
{
    connect(MqttCommunication::instance(), &MqttCommunication::handleMessageInfo, this, &PCRLightAccuracyWidget::onHandleSetParams);
    connect(ui->swControlDuct_2, &SwitchButton::statusChanged, this, &PCRLightAccuracyWidget::onStatusChanged_2);
    connect(MqttCommunication::instance(), &MqttCommunication::handleExperimentFinished, this, &PCRLightAccuracyWidget::onHandleFinished);

    //为了使SwitchButton::statusChanged生效，特意放在connect后
    ui->swControlDuct_2->setChecked(false);
}

void PCRLightAccuracyWidget::initStyleSheet()
{
    m_currentChannel = ENUM_CLASS::NONE;
    m_channelOnClose = ENUM_CLASS::NONE;

    ui->label_2->setObjectName("labModuleDataAction");
    ui->line_2->setObjectName("blueDashLine");

    ui->label_3->setObjectName("labModuleDataAction");
    ui->line_3->setObjectName("blueDashLine");

    ui->label_4->setObjectName("labModuleDataAction");
    ui->line_4->setObjectName("blueDashLine");

    ui->btnFAM->setObjectName("btnChannel");
    ui->btnHEX->setObjectName("btnChannel");
    ui->btnTAMRA->setObjectName("btnChannel");
    ui->btnROX->setObjectName("btnChannel");
    ui->btnCY5->setObjectName("btnChannel");

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->btnFAM);
    buttonGroup->addButton(ui->btnHEX);
    buttonGroup->addButton(ui->btnTAMRA);
    buttonGroup->addButton(ui->btnROX);
    buttonGroup->addButton(ui->btnCY5);
    buttonGroup->setExclusive(true);

    connect(buttonGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this, &PCRLightAccuracyWidget::onButtonClicked);

    //正则表达式：只能输入整数/小数
    QRegExp regExp("^-?\\d+(\\.\\d+)?$");
    ui->lineEdit_8->setValidator(new QRegExpValidator(regExp, this));
}

void PCRLightAccuracyWidget::initScrollBar()
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

void PCRLightAccuracyWidget::initTemperatureWidget()
{
    if(!m_temperatureWidget)
    {
        m_temperatureWidget = new TemperatureWidget(m_id, this);
    }
    m_temperatureWidget->hide();
}

//托盘推出
bool PCRLightAccuracyWidget::blockMoveOut()
{
    bool result = false;

    if(!MqttExecutor::instance()->blockMove(m_id, 1))
    {
        MessageDialog msg("模块连接超时！");
        msg.exec();
        return result;
    }

    //等待中位机返回
    MessageDialog *msg = new MessageDialog("托盘推出中...");
    QMetaObject::Connection conn = connect(MqttCommunication::instance(), &MqttCommunication::handleMessageInfo,[&](MessageInfo info, ENUM_CLASS::PCR_Id id){

        if(m_id != id || info.command != "move_block")
        {
            return;
        }

        if(info.result == "ok")
        {
            msg->setMsg("执行成功！");
            msg->hideSpinner();
            msg->setConfirmText("确定");
            msg->setBtnEnable(true);
            result = true;
        }
        else
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

//通道复位
bool PCRLightAccuracyWidget::channelReset(bool isUsedForSwitchChannel)
{
    if(m_currentChannel == ENUM_CLASS::NONE)
    {
        MessageDialog msg("请选择通道");
        msg.exec();
        return false;
    }

    if(!MqttExecutor::instance()->resetChannel(m_id))
    {
        MessageDialog msg("模块连接超时！");
        msg.exec();
        return false;
    }

    bool result = false;

    //等待中位机返回
    MessageDialog *msg = new MessageDialog("正在复位");
    QMetaObject::Connection conn = connect(MqttCommunication::instance(), &MqttCommunication::handleMessageInfo,[&](MessageInfo info, ENUM_CLASS::PCR_Id id){

        if(m_id != id)
        {
            return;
        }

        if("reset_mechanism" == info.command && info.result == "ok" && info.resetObject == "filter")
        {
            msg->setMsg("复位成功！");
            msg->hideSpinner();
            msg->setConfirmText("确定");
            msg->setBtnEnable(true);

            //若与切换通道组合，则复位成功后自动点击
            if(isUsedForSwitchChannel)
            {
                msg->click();
            }

            result = true;
        }
        else
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

//切换通道
bool PCRLightAccuracyWidget::channelSwitch()
{
    if(m_currentChannel == ENUM_CLASS::NONE)
    {
        MessageDialog msg("请选择通道");
        msg.exec();
        return false;
    }

    //切换通道
    if(!MqttExecutor::instance()->changeChannel(m_id, m_currentChannel))
    {
        MessageDialog msg("模块连接超时！");
        msg.exec();
        return false;
    }

    bool result = false;

    //等待中位机返回
    MessageDialog *msg = new MessageDialog("正在执行");
    QMetaObject::Connection conn = connect(MqttCommunication::instance(), &MqttCommunication::handleMessageInfo,[&](MessageInfo info, ENUM_CLASS::PCR_Id id){

        if(m_id != id)
        {
            return;
        }

        if(info.command == "move_filter" && info.result == "ok")
        {
            msg->setMsg("执行成功！");
            msg->hideSpinner();
            msg->setConfirmText("确定");
            msg->setBtnEnable(true);

            result = true;
        }
        else
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

//处理拍照完成后的逻辑
void PCRLightAccuracyWidget::handlePhotoCapture()
{

/*
*    {
*        "ip": "192.168.137.101/102",
*        "user": "tc384",
*        "password": "1",
*        "ftp":"",
*        "port":"21",
*        "path":"可执行程序目录/base_map"
*    }
*/

    //获取校准矩阵并写入配置文件
    if(!getAdjustMatrix())
    {
        MessageDialog msg("获取校准矩阵失败！");
        msg.exec();
        return;
    }

    //获取测试底图
    if(!getTestPhoto(QCoreApplication::applicationDirPath() + "/"))
    {
        MessageDialog msg("获取测试底图失败！");
        msg.exec();
        return;
    }

    //启动分析软件
    if(!startAnalyseSoft(QCoreApplication::applicationDirPath() + QDir::separator() + ANALYSE_SOFT_PATH))
    {
        return;
    }

    //组合json
    QJsonObject obj;

    if(m_id == ENUM_CLASS::PCR_Id::M1)
    {
        obj["ip"] = "192.168.137.101";
    }
    else
    {
        obj["ip"] = "192.168.137.102";
    }

    obj["user"] = "tc384";
    obj["password"] = "1";
    obj["ftp"] = "";
    obj["port"] = "21";
    obj["path"] = QCoreApplication::applicationDirPath() + "/base_map";

    QJsonDocument doc(obj);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    SocketServer::instance()->buildData(data);
}

//获取测试底图
bool PCRLightAccuracyWidget::getTestPhoto(const QString &path)
{
    //删除base_map文件夹
    QString deletePath = path + "/base_map";

    QDir dir(deletePath);
    if(dir.exists())
    {
        if(dir.removeRecursively())
        {
            qDebug() << "删除文件成功！" << deletePath;
        }
        else
        {
            qDebug() << "删除文件失败：" << deletePath;
            return false;
        }
    }
    else
    {
        qDebug() << "base_map不存在，不进行删除";
    }

    QProcess process;

    connect(&process, &QProcess::readyReadStandardOutput, this, [=]() {
        QProcess *process = qobject_cast<QProcess *>(sender());
        if(process)
        {
            QString output = process->readAllStandardOutput();
            qDebug() << "Output:" << output;
        }
    });
    connect(&process, &QProcess::readyReadStandardError, this, [=]() {
        QProcess *process = qobject_cast<QProcess *>(sender());
        if(process)
        {
            QString errorOutput = process->readAllStandardError();
            qDebug() << "Error Output:" << errorOutput;

            if(errorOutput.contains("(y/n, Return cancels connection, i for more info)"))
            {
                qDebug() << "自动输入y以确认SSH密钥";
                process->write("y\n");
            }
        }

    });

    QString scriptPath = QCoreApplication::applicationDirPath() + "/CopyFiles.ps1";

    QString ip;
    if(m_id == ENUM_CLASS::PCR_Id::M1)
    {
        ip = "192.168.137.101";
    }
    else
    {
        ip = "192.168.137.102";
    }

    QStringList arguments;
    arguments << "-ExecutionPolicy" << "Bypass" << "-File" << scriptPath << path << ip;

    // 调用 PowerShell 执行脚本
    process.start("powershell", arguments);

    // 等待命令执行结束
    if(!process.waitForFinished(-1))
    {
        qDebug() << "执行获取测试底图脚本失败:" << process.errorString();
        return false;
    }

    QString output = process.readAllStandardOutput();
    QString errorOutput = process.readAllStandardError();

    qDebug() << "Output:" << output;
    if(!errorOutput.isEmpty())
    {
        qDebug() << "Error Output:" << errorOutput;
        return false;
    }

    return true;
}

//启动分析软件
bool PCRLightAccuracyWidget::startAnalyseSoft(const QString &path)
{
    if(!m_process)
    {
        m_process = new QProcess(this);
    }

    QString nativePath = QDir::toNativeSeparators(path);

    if(!m_process->startDetached(nativePath))
    {
        qDebug() << "启动分析软件失败！";
        return false;
    }

    qDebug() << "启动分析软件成功！";
    qDebug() << "启动路径为：" << nativePath;

    return true;
}

//获取方案
void PCRLightAccuracyWidget::getScheme()
{
    QString currentPcrConfig = ParseMessageInfo::readJsonFromFile(JSON_FILE_PATH("old.json"));

    m_currentPcrConfig.clear();

    m_currentPcrConfig = ParseMessageInfo::modifyScheme(currentPcrConfig, ui->editLidTemperature->text().toInt(), ui->editBlockTemperature->text().toInt(), ui->editCount->text().toInt());
}

//下发方案
bool PCRLightAccuracyWidget::setScheme()
{
    if(!MqttExecutor::instance()->setCurrentScheme(m_id, m_currentPcrConfig, "荧光校准老化方案", ui->swControlDuct_2->checked()))
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
            m_currentSchemeName = "荧光校准老化方案";
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

bool PCRLightAccuracyWidget::runScheme()
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
            //读取温度
            m_temperatureWidget->startGetTemperature();

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

void PCRLightAccuracyWidget::showTemperatureWidget()
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

//获取校准矩阵
bool PCRLightAccuracyWidget::getAdjustMatrix()
{
    QString result = DBManager::instance()->getAdjustMatrix(m_id);

    if(result.isEmpty())
    {
        return false;
    }

    //写入"可执行目录/384FulCal/res/debug/calibrationConfig.json"中
    QString calibrationConfigPath = QCoreApplication::applicationDirPath() + "/384FulCal/res/debug/calibrationConfig.json";

    QFile file(calibrationConfigPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return false;
    }

    if(file.write(result.toUtf8()) == -1)
    {
        file.close();
        return false;
    }

    file.close();

    return true;
}

//保存第一通道位置参数
bool PCRLightAccuracyWidget::saveFirstChannelPosition(const ChannelParams &params)
{
    if(!MqttExecutor::instance()->setFirstChannelPosition(m_id, params))
    {
        return false;
    }

    return true;
}

//保存曝光时间
bool PCRLightAccuracyWidget::saveChannelExposureTime(const ChannelParams &params)
{
    if(!MqttExecutor::instance()->setChannelExposureTime(m_id, params))
    {
        return false;
    }

    return true;
}

//保存浓度系数
bool PCRLightAccuracyWidget::saveChannelPotency(const ChannelParams &params)
{
    if(!MqttExecutor::instance()->setChannelPotency(m_id, params))
    {
        return false;
    }

    return true;
}

//发送sample参数
bool PCRLightAccuracyWidget::setSampleParams()
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

bool PCRLightAccuracyWidget::eventFilter(QObject *watched, QEvent *event)
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

void PCRLightAccuracyWidget::onHandleFinished(const MessageInfo &info, ENUM_CLASS::PCR_Id id)
{
    qDebug() << m_id << id << info.status << m_currentSchemeName << info.experiment_name;

    if(m_id != id || info.status.isEmpty() || m_currentSchemeName != info.experiment_name)
    {
        return;
    }

    qDebug() << info.status << m_currentSchemeName << info.experiment_name;

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

void PCRLightAccuracyWidget::onStatusChanged_2(bool enableSample)
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

void PCRLightAccuracyWidget::onHandleSetParams(const MessageInfo &info, ENUM_CLASS::PCR_Id id)
{
    if(m_id != id)
    {
        return;
    }

    if(info.result == "ok")
    {
        if(info.uuid == "set_channel_position")
        {
            m_setFirstChannelStatus = ENUM_CLASS::SetParams_Status::Success;
        }
        else if(info.uuid == "set_channel_exposure")
        {
            m_setExposureTimeStatus = ENUM_CLASS::SetParams_Status::Success;
        }
        else if(info.uuid == "set_channel_potency")
        {
            m_setPotencyStatus = ENUM_CLASS::SetParams_Status::Success;
        }
    }
    else
    {
        if(info.uuid == "set_channel_position")
        {
            m_setFirstChannelStatus = ENUM_CLASS::SetParams_Status::failed;
        }
        else if(info.uuid == "set_channel_exposure")
        {
            m_setExposureTimeStatus = ENUM_CLASS::SetParams_Status::failed;
        }
        else if(info.uuid == "set_channel_potency")
        {
            m_setPotencyStatus = ENUM_CLASS::SetParams_Status::failed;
        }
    }

    int overallStatus = std::min({m_setFirstChannelStatus, m_setExposureTimeStatus, m_setPotencyStatus});

    if(overallStatus == ENUM_CLASS::SetParams_Status::Success)
    {
        MessageDialog msg("保存成功！");
        msg.exec();

        m_setFirstChannelStatus = ENUM_CLASS::SetParams_Status::Uninit;
        m_setExposureTimeStatus = ENUM_CLASS::SetParams_Status::Uninit;
        m_setPotencyStatus = ENUM_CLASS::SetParams_Status::Uninit;
    }
    else if(overallStatus == ENUM_CLASS::SetParams_Status::failed)
    {
        MessageDialog msg("保存失败！");
        msg.exec();

        m_setFirstChannelStatus = ENUM_CLASS::SetParams_Status::Uninit;
        m_setExposureTimeStatus = ENUM_CLASS::SetParams_Status::Uninit;
        m_setPotencyStatus = ENUM_CLASS::SetParams_Status::Uninit;
    }
}

void PCRLightAccuracyWidget::onButtonClicked(QAbstractButton *button)
{
    if (button == ui->btnFAM)
    {
        m_currentChannel = ENUM_CLASS::FAM;
    }
    else if (button == ui->btnHEX)
    {
        m_currentChannel = ENUM_CLASS::HEX;
    }
    else if (button == ui->btnTAMRA)
    {
        m_currentChannel = ENUM_CLASS::TAMRA;
    }
    else if (button == ui->btnROX)
    {
        m_currentChannel = ENUM_CLASS::ROX;
    }
    else if (button == ui->btnCY5)
    {
        m_currentChannel = ENUM_CLASS::CY5;
    }
}

//模块复位
void PCRLightAccuracyWidget::on_pushButton_2_clicked()
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

//切换通道
void PCRLightAccuracyWidget::on_pushButton_23_clicked()
{
    //通道复位
    if(!channelReset(true))
    {
        return;
    }

    //切换通道
    channelSwitch();
}

//通道复位
void PCRLightAccuracyWidget::on_pushButton_24_clicked()
{
    channelReset(false);
}

//基础性能测试-拍照
void PCRLightAccuracyWidget::on_pushButton_30_clicked()
{
    //发送Sample参数
    if(ui->swControlDuct_2->checked())
    {
        if(!setSampleParams())
        {
            return;
        }
    }

    //修改方案
    getScheme();

    //设置方案
    if(!setScheme())
    {
        return;
    }

    //运行方案
    runScheme();
}

//基础性能测试-停止
void PCRLightAccuracyWidget::on_pushButton_29_clicked()
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

//读取参数
void PCRLightAccuracyWidget::on_pushButton_32_clicked()
{
    ChannelParams params;

    //获取第一通道位置
    ParseMessageInfo::parseFirstChannelPosition(DBManager::instance()->getFirstChannelPosition(m_id), params);

    //获取曝光时间
    ParseMessageInfo::parseExposureTimeJson(DBManager::instance()->getExposureTime(m_id), params);

    //获取浓度系数
    ParseMessageInfo::parsePotency(DBManager::instance()->getPotency(m_id), params);

    if(params.error)
    {
        MessageDialog msg("获取参数失败！");
        msg.exec();
        return;
    }

    //曝光时间
    ui->channel_1_time->setText(QString::number(params.channel_1_exposure_time));
    ui->channel_2_time->setText(QString::number(params.channel_2_exposure_time));
    ui->channel_3_time->setText(QString::number(params.channel_3_exposure_time));
    ui->channel_4_time->setText(QString::number(params.channel_4_exposure_time));
    ui->channel_5_time->setText(QString::number(params.channel_5_exposure_time));

    //浓度系数
    ui->channel_1_potency->setText(params.channel_1_potency);
    ui->channel_2_potency->setText(params.channel_2_potency);
    ui->channel_3_potency->setText(params.channel_3_potency);
    ui->channel_4_potency->setText(params.channel_4_potency);
    ui->channel_5_potency->setText(params.channel_5_potency);

    //第一通道位置
    ui->lineEdit_8->setText(params.channel1_position);

    //通道间隔
    ui->lineEdit_10->setText(params.channel_gap);

    ui->pushButton_31->setEnabled(true);
}

//保存参数
void PCRLightAccuracyWidget::on_pushButton_31_clicked()
{
    m_setFirstChannelStatus = ENUM_CLASS::SetParams_Status::Uninit;
    m_setExposureTimeStatus = ENUM_CLASS::SetParams_Status::Uninit;
    m_setPotencyStatus = ENUM_CLASS::SetParams_Status::Uninit;

    ChannelParams params;
    params.channel1_position = ui->lineEdit_8->text();                      //第一通道位置
    params.channel_gap = ui->lineEdit_10->text();                           //通道间隔
    params.channel_1_exposure_time = ui->channel_1_time->text().toInt();
    params.channel_2_exposure_time = ui->channel_2_time->text().toInt();
    params.channel_3_exposure_time = ui->channel_3_time->text().toInt();
    params.channel_4_exposure_time = ui->channel_4_time->text().toInt();
    params.channel_5_exposure_time = ui->channel_5_time->text().toInt();
    params.channel_1_potency = ui->channel_1_potency->text();
    params.channel_2_potency = ui->channel_2_potency->text();
    params.channel_3_potency = ui->channel_3_potency->text();
    params.channel_4_potency = ui->channel_4_potency->text();
    params.channel_5_potency = ui->channel_5_potency->text();

    //保存第一通道位置参数和通道间隔
    if(!saveFirstChannelPosition(params))
    {
        MessageDialog msg("模块断开连接！");
        msg.exec();
        return;
    }

    //保存曝光时间
    if(!saveChannelExposureTime(params))
    {
        MessageDialog msg("模块断开连接！");
        msg.exec();
        return;
    }

    //保存浓度系数
    if(!saveChannelPotency(params))
    {
        MessageDialog msg("模块断开连接！");
        msg.exec();
        return;
    }
}


void PCRLightAccuracyWidget::on_pushButton_clicked()
{
    handlePhotoCapture();
}

void PCRLightAccuracyWidget::on_pushButton_3_clicked()
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
