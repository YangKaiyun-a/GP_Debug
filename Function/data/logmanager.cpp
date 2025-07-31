#include "logmanager.h"

#include <QApplication>
#include <QDir>
#include <QDirIterator>
#include <QDateTime>
#include <QMutex>
#include <QTextStream>

int LOG_INDEX = 1;      //当天被分割的日志的索引

//重定向qdebug输出
void outputLogMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    LogManager::getInstance()->outputLog(type,context,msg);
}

static QString getFileName(QString logContext)
{
    if (logContext.isEmpty())
        return QString("");

    //输入是相对路径名,只取其中的文件名
    /**
     * windows格式："..\\UI\\src\\rpc\\uimessage.cpp"
     * ubuntu格式： "../UI/src/rpc/uimessage.cpp"
     */

    QString fileName(logContext);
    int start = 0;
#ifdef Q_OS_WIN32
    start = fileName.lastIndexOf("\\")+1;
#else
    start = fileName.lastIndexOf("/")+1;
#endif

    return fileName.mid(start);
}

static QString getFunName(QString logContext)
{
    if (logContext.isEmpty())
        return QString("");

    //有两个情况,只取其中的函数名,ubuntu下该格式不适用
    // void __cdecl MainWindow::onBootup()
    // int __cdecl main(int,char *[])
    QString funName(logContext);
    int start = 0;
#ifdef Q_OS_WIN32
    start = funName.lastIndexOf("decl")+4;
#endif
    int end   = funName.indexOf("(");

    return funName.mid(start, end-start);
}


LogManager* LogManager::m_logManager = nullptr;

LogManager::LogManager(QObject *parent)
    : QObject{parent}
{
    init();
}

LogManager::~LogManager()
{
    freeManager();
}

LogManager *LogManager::getInstance()
{
    if(!m_logManager)
    {
        m_logManager = new LogManager();
    }

    return m_logManager;
}

void LogManager::init()
{
    m_logFilePath = QApplication::applicationDirPath() + "/log/";

    QDir dir(m_logFilePath);

    if(!dir.exists())
    {
        dir.mkpath(m_logFilePath);
    }

    //删除超过30天的日志
    QDate currentDate = QDate::currentDate();
    QDirIterator dirIterator(m_logFilePath, QDir::Files);
    while(dirIterator.hasNext())
    {
        dirIterator.next();
        QFileInfo fileInfo = dirIterator.fileInfo();
        QDate fileDate = fileInfo.lastModified().date();

        int daysOld = fileDate.daysTo(currentDate);
        if(daysOld > 30)
        {
            QFile::remove(fileInfo.absoluteFilePath());
        }
    }

    mDefaultOutput = qInstallMessageHandler(outputLogMessage);
}

void LogManager::outputLog(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    //如果要写文件需要加锁，因为函数调用在debug调用线程
    QMutexLocker locker(&mLogMutex);
    QString out_text;
    QTextStream stream(&out_text);

    //打印时间和线程ID信息
    stream << QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss:zzz] ");

    //区分日志类型给文本加头
    switch (type)
    {
    case QtDebugMsg:        stream<<"[Debug   ]"; break;
    case QtInfoMsg:         stream<<"[Info    ]"; break;
    case QtWarningMsg:      stream<<"[Warning ]"; break;
    case QtCriticalMsg:     stream<<"[Critical]"; break;
    case QtFatalMsg:        stream<<"[Fatal   ]"; break;
    default:                stream<<"[Unknown ]"; break;
    }

    //加入文件信息
    QString fileName = getFileName(context.file);       ///<打印所在的源文件名称
    QString funName = getFunName(context.function);     ///<函数名称
    QString tmpStr = QString("[%1 line:%2 %3] { %4}").arg(fileName).arg(context.line).arg(funName).arg(msg);

    stream << tmpStr;

    //日志大小分割，分割大小默认为5MB，此功能默认不开启
    if(mIsFileSplit)
    {

        QFileInfo info(mFile.fileName());

        //获取文件大小并进行比较
        if(info.size() >= mFileSize * 1024 * 1024)
        {
            //超过设定值后重命名该文件，关闭文件
            mFile.rename(m_logFilePath + QDate::currentDate().toString("yyyyMMdd")+"_ui_" + QString::number(LOG_INDEX) + ".txt");
            if(mFile.isOpen())
            {
                mFile.close();
            }

            //建立并打开新的文件准备进行写入
            mFile.setFileName(m_logFilePath + QDate::currentDate().toString("yyyyMMdd")+"_ui_" + QString::number(++LOG_INDEX) + ".txt");
            if(!mFile.open(QIODevice::WriteOnly|QIODevice::Append))
            {
                emit newLog(QtDebugMsg,"Open log file error:"+mFile.errorString()+mFile.fileName());        //打开失败发送错误消息
            }
        }

    }

    if(!mFile.isOpen())
    {
        //文件未打开则按照日期设定文件的名称
        mFile.setFileName(m_logFilePath + QDate::currentDate().toString("yyyyMMdd")+"_ui" + ".log");

        //打开文件，使用Append追加模式，避免同一文件被清除
        if(!mFile.open(QIODevice::WriteOnly|QIODevice::Append))
        {
            emit newLog(QtDebugMsg,"Open log file error:"+mFile.errorString()+mFile.fileName());        //打开失败发送错误消息
        }
    }
    //文件打开则直接进行写入
    if(mFile.isOpen())
    {
        //写入文件
        stream.setDevice(&mFile);
        stream<<out_text<<"\n";
    }

    //发送信号给需要的对象，如ui上显示日志
    emit newLog(type, msg);

    //默认的输出，控制台
    //区分日志类型给文本加颜色
    //常见格式为：\e[显示方式;前景色;背景色m输出字符串\e[0m
    //其中\e=\033
    QString cmd_text;
    stream.setString(&cmd_text);
    switch (type)
    {
    //debug绿色
    case QtDebugMsg:
        stream<<"\033[34m";
        break;
    //info蓝色
    case QtInfoMsg:
        stream<<"\033[34m";
        break;
    //warning紫色
    case QtWarningMsg:
        stream<<"\033[35m";
        break;
    //critical加粗红色
    case QtCriticalMsg:
        stream<<"\033[1;31m";
        break;
    //fatal红底黑字
    //Fatal表示致命错误，默认处理会报异常的
    case QtFatalMsg:
        stream<<"\033[1;30;41m";
        break;
    //defualt默认颜色
    default:
        stream<<"\033[0m";
        break;
    }
    stream << out_text << "\033[0m";
    if(mDefaultOutput)
    {
        mDefaultOutput(type,context,cmd_text);
    }
}

void LogManager::freeManager()
{
    mFile.close();
    qInstallMessageHandler(nullptr);
}
