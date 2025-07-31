#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QObject>
#include <QMutex>
#include <QFile>
#include <QElapsedTimer>

class LogManager : public QObject
{
    Q_OBJECT
public:
    explicit LogManager(QObject *parent = nullptr);
    ~LogManager();

    static LogManager *getInstance();
    void init();
    //重定向qdebug输出
    void outputLog(QtMsgType type, const QMessageLogContext& context, const QString& msg);
    //释放
    void freeManager();

signals:
    //可以关联信号接收日志信息，如显示到ui中
    //注意，如果槽函数为lambda或者其他没有接收者的情况，需要保证槽函数中的变量有效性
    //因为static变量的生命周期更长，可能槽函数所在模块已经释放资源，最好connect加上接收者
    void newLog(int msgType, const QString &log);

private:
    static LogManager *m_logManager;
    QString m_logFilePath;
    //保留默认handle，用于输出到控制台
    QtMessageHandler mDefaultOutput = nullptr;
    //多线程操作时需要加锁
    QMutex mLogMutex;
    //是否开启文件分割
    bool mIsFileSplit = false;
    //文件分割的最小大小
    int mFileSize;          //单位 MB
    //输出到文件
    QFile mFile;
};

#endif // LOGMANAGER_H
