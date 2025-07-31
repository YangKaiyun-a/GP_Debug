#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>

class SocketServer : public QLocalServer
{
    Q_OBJECT
public:
    explicit SocketServer(QObject *parent = nullptr);
    static SocketServer* instance();
    bool initConnect(const QString &serverName);
    void write(const QByteArray &data);
    void buildData(const QByteArray &data);

private slots:
    void onNewConnection();
    void onReadyRead();

signals:

private:
    QLocalSocket *m_clientSocket = nullptr;
    static SocketServer *m_server;              //实例化对象
    QByteArray m_data;
};

#endif // SOCKETSERVER_H
