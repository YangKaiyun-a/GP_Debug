#include "socketserver.h"

#include <QJsonDocument>
#include <QJsonObject>

SocketServer* SocketServer::m_server = nullptr;

SocketServer::SocketServer(QObject *parent)
    : QLocalServer{parent}
{
    connect(this, &QLocalServer::newConnection, this, &SocketServer::onNewConnection);
}

SocketServer *SocketServer::instance()
{
    if(!m_server)
    {
        m_server = new SocketServer();
    }

    return m_server;
}

bool SocketServer::initConnect(const QString &serverName)
{
    QLocalServer::removeServer(serverName);

    if(!listen(serverName))
    {
        qDebug() << "QLocalSocket 服务器启动失败：" << errorString();
        return false;
    }
    qDebug() << "QLocalSocket 服务器启动成功！";
    return true;
}

void SocketServer::write(const QByteArray &data)
{
    if(m_clientSocket && m_clientSocket->state() == QLocalSocket::ConnectedState)
    {
        qDebug() << "发送数据给分析软件：" << data;
        m_clientSocket->write(data);
        m_clientSocket->flush();
    }
    else
    {
        qDebug() << "分析软件未连接。";
    }
}

void SocketServer::buildData(const QByteArray &data)
{
    m_data.clear();
    m_data = data;
}

void SocketServer::onNewConnection()
{
    m_clientSocket = nextPendingConnection();
    qDebug() << "分析软件连接成功";

    connect(m_clientSocket, &QLocalSocket::readyRead, this, &SocketServer::onReadyRead);
    connect(m_clientSocket, &QLocalSocket::disconnected, this, [=]() {
        m_clientSocket->deleteLater();
        m_clientSocket = nullptr;
        qDebug() << "分析软件断开连接";
    });

    //发送json数据给分析软件
    if(!m_data.isEmpty())
    {
        write(m_data);
    }
    else
    {
        qDebug() << "m_data为空，发送给分析软件失败！";
    }
}

void SocketServer::onReadyRead()
{
    if(!m_clientSocket)
    {
        return;
    }

    QByteArray data = m_clientSocket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject json = doc.object();

    if(json["command"].toString() == "get_update_package")
    {

    }
}
