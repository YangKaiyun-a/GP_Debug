#include "mainwindow.h"
#include "application.h"

#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QThread>
#include <QMessageBox>

#ifdef __cplusplus
extern "C" {
#endif

#include "canfestival.h"

#ifdef __cplusplus
};
#endif

#include "Driver/canmgr.h"

void setStyle()
{
    QFile file(":/winJk.qss");
    if(file.open(QFile::ReadOnly))
    {
        qDebug() << "QSS文件打开成功";
        QTextStream filetext(&file);
        QString stylesheet = filetext.readAll();
        qApp->setStyleSheet(stylesheet);
        file.close();
    }
    else
    {
        qDebug() << "QSS文件打开失败" << file.errorString();
    }
}

void init()
{
    //初始化资源
    CanMgr::instance()->start(QThread::NormalPriority);
    CanOpenStart();

    //设置can
    auto can = CanMgr::instance();
    can->close();
    can->open(1);
    if(can->isFinished())
        can->start();

    //加载qss文件
    setStyle();
}

int main(int argc, char *argv[])
{
    Application::instance(argc, argv);

    if(!Application::instance()->init())
    {
        QMessageBox::critical(nullptr, "Error", "初始化失败");
    }
    else
    {
        QMessageBox::information(nullptr, "Successfully", "初始化成功");
    }

    init();

    MainWindow w;
    w.showFullScreen();
    return Application::instance()->exec();
}
