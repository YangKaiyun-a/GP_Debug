#include "application.h"
#include "Function/Data/settingfilecreate.h"
#include "Function/Data/mqttcommunication.h"
#include "Function/Data/dbmanager.h"
#include "Function/data/socketserver.h"
#include "Function/data/logmanager.h"
#include "Function/data/parsemessageinfo.h"

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#include <QSettings>
#include <QMessageBox>

//重定向输出
void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);

    QByteArray localMsg = msg.toLocal8Bit();
    switch (type)
    {
    case QtDebugMsg:
        printf("Debug: %s\n", localMsg.constData());
        break;
    case QtInfoMsg:
        printf("Info: %s\n", localMsg.constData());
        break;
    case QtWarningMsg:
        printf("Warning: %s\n", localMsg.constData());
        break;
    case QtCriticalMsg:
        printf("Critical: %s\n", localMsg.constData());
        break;
    case QtFatalMsg:
        printf("Fatal: %s\n", localMsg.constData());
        abort();
    }
}


Application* Application::m_singlenApplication = nullptr;

Application::Application(int &argc, char **argv)
    : QApplication(argc,argv)
{

}

Application *Application::instance()
{
    return m_singlenApplication;
}

Application *Application::instance(int &argc, char **argv)
{
    if(m_singlenApplication == nullptr)
    {
        m_singlenApplication = new Application(argc,argv);
    }
    return m_singlenApplication;
}

bool Application::init()
{
    bool res = true;

    //初始化日志
    LogManager::getInstance();

    m_sigmanager = SignalManager::instance();

    //初始化配置文件
    SettingFileCreate::instance();

    //是否开启控制台
    QSettings settings(SettingFileCreate::instance()->m_configFile, QSettings::IniFormat);
    settings.beginGroup("cmd");
    //读取配置文件信息
    bool enable = settings.value("enable").toBool();
    if(enable)
    {
        // 安装消息处理器
        qInstallMessageHandler(customMessageHandler);

#ifdef Q_OS_WIN
        // 在Windows上分配一个新的控制台
        AllocConsole();
        // 重定向标准输入输出流到新的控制台
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
#endif

        qDebug() << "已开启控制台";
    }

    //连接数据库
    if(!DBManager::instance()->connectDataBase())
    {
        res = false;
    }

    //连接mqtt
    MqttCommunication::instance();

    //开启QLocalSocket服务
    if(!SocketServer::instance()->initConnect("GP_Debug"))
    {
        res = false;
    }

    //初始化标记偏差坐标、激光相对葫芦头位置
    ParseMessageInfo::initDeviationFromJson(JSON_FILE_PATH("crossingDeviation.json"));

    return res;
}

