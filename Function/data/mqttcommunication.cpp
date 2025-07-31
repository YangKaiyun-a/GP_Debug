#include "mqttcommunication.h"
#include "global.h"
#include "settingfilecreate.h"
#include "parsemessageinfo.h"

#include <QDebug>
#include <QThread>
#include <QMessageBox>

MqttCommunication* MqttCommunication::m_mqttCommunication = nullptr;

MqttCommunication::MqttCommunication(QObject *parent) : QObject(parent)
{
    init();
}

MqttCommunication *MqttCommunication::instance()
{
    if(!m_mqttCommunication)
    {
        m_mqttCommunication = new MqttCommunication();
    }

    return m_mqttCommunication;
}

void MqttCommunication::init()
{
    m_client1 = new QMqttClient();
    m_client1->setObjectName("M1");

    m_client2 = new QMqttClient();
    m_client2->setObjectName("M2");

    m_middleClient = new QMqttClient();
    m_middleClient->setObjectName("Middle");

    m_clientMap.insert(ENUM_CLASS::M1, m_client1);
    m_clientMap.insert(ENUM_CLASS::M2, m_client2);
    m_clientMap.insert(ENUM_CLASS::Middle, m_middleClient);

    //移入线程
    QThread* m_cilent1Thread = new QThread();
    m_client1->moveToThread(m_cilent1Thread);
    m_cilent1Thread->start();

    QThread* m_cilent2Thread = new QThread();
    m_client2->moveToThread(m_cilent2Thread);
    m_cilent2Thread->start();

    QThread* m_cilent3Thread = new QThread();
    m_middleClient->moveToThread(m_cilent3Thread);
    m_cilent3Thread->start();

    //连接M1服务端
    QString host1 = SettingFileCreate::instance()->sysValue("M1", "mqtt_HostName").toString();
    int port1 = SettingFileCreate::instance()->sysValue("M1", "mqtt_Port").toInt();
    connectToServer(m_client1, host1, port1);

    //连接M2服务端
    QString host2 = SettingFileCreate::instance()->sysValue("M2", "mqtt_HostName").toString();
    int port2 = SettingFileCreate::instance()->sysValue("M2", "mqtt_Port").toInt();
    connectToServer(m_client2, host2, port2);

    //连接中位机服务端
    QString host3 = SettingFileCreate::instance()->sysValue("Position", "mqtt_HostName").toString();
    int port3 = SettingFileCreate::instance()->sysValue("Position", "mqtt_Port").toInt();
    connectToServer(m_middleClient, host3, port3);
}

void MqttCommunication::connectToServer(QMqttClient *client, const QString &host, int port)
{
    //连接信号和槽
    connect(client, &QMqttClient::messageReceived, this, &MqttCommunication::onMessageReceived, Qt::QueuedConnection);
    connect(client, &QMqttClient::stateChanged, this, &MqttCommunication::onStateChanged, Qt::QueuedConnection);
    connect(client, &QMqttClient::errorChanged, this, [=](const QMqttClient::ClientError e){
            qCritical() << client->objectName() << "连接失败！" << e;
        }, Qt::QueuedConnection);

    client->setHostname(host);
    client->setPort(port);
    client->connectToHost();
    client->setAutoKeepAlive(true);
}

void MqttCommunication::publishMessage(ENUM_CLASS::PCR_Id id, const QString &topic, const QByteArray &message)
{
    QMqttClient *client = m_clientMap.value(id);

    if(client->state() == QMqttClient::Connected)
    {
        QMqttTopicName mqttTopic(topic);
        client->publish(mqttTopic, message);
        qDebug() << client->objectName() << "publish" << "topic:" << topic << "message:" << QString::fromUtf8(message); //将QByteArray转为QString，用于打印日志
    }
    else
    {
        qCritical() << client->objectName() << "is not connected.";
    }
}

void MqttCommunication::subscribeToTopic(ENUM_CLASS::PCR_Id id, const QString &topic)
{
    QMqttClient *client = m_clientMap.value(id);

    qDebug() << client->objectName() << "subsribeToTopic" << topic;
    if(client->state() == QMqttClient::Connected)
    {
        QMqttTopicFilter mqttTopicFilter(topic);
        client->subscribe(mqttTopicFilter);
    }
    else
    {
        qCritical() << client->objectName() << "is not connected.";
    }
}

void MqttCommunication::unsubscribeToTopic(ENUM_CLASS::PCR_Id id, const QString &topic)
{
    QMqttClient *client = m_clientMap.value(id);

    qDebug() << client->objectName() << "unsubscribeToTopic" << topic;
    if(client->state() == QMqttClient::Connected)
    {
        QMqttTopicFilter mqttTopicFilter(topic);
        client->unsubscribe(mqttTopicFilter);
    }
    else
    {
        qCritical() << client->objectName() << "is not connected.";
    }
}

bool MqttCommunication::isClientConnected(ENUM_CLASS::PCR_Id id)
{
    QMqttClient *client = m_clientMap.value(id);

    if(client->state() == QMqttClient::Connected)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//接收中位机消息
void MqttCommunication::onMessageReceived(const QByteArray &message, const QMqttTopicName &topic)
{
    QMqttClient *sourceClient = qobject_cast<QMqttClient *>(sender());
    if(!sourceClient)
    {
        qCritical() << "Message received from unknown source";
        return;
    }

    ENUM_CLASS::PCR_Id id;
    for(auto it = m_clientMap.begin(); it != m_clientMap.end(); ++it)
    {
        if(it.value() == sourceClient)
        {
            id = it.key();
            break;
        }
    }

    qDebug() << "Message from" << sourceClient->objectName() << ": " << QString(message) << " topic: " << topic.name();

    if(topic.name() == QString(TC384_CONTROL_UP) || topic.name() == QString(TC384_DEBUG_UP))
    {
        //接受qpcr上报消息
        Q_EMIT handleMessageInfo(ParseMessageInfo::parseMessageReceived(QString(message)), id);
    }
    else if(topic.name() == QString(TC384_CHANGE_UP))
    {
        //接受qpcr实验完成后上报消息
        Q_EMIT handleExperimentFinished(ParseMessageInfo::parseMessageReceived(QString(message)), id);
    }
    else if(topic.name() == QString(LOAD_CARRIERS_REPLY) || topic.name() == QString(LOAD_TEST_PROJECT_REPLY) || \
            topic.name() == QString(LOAD_PROCESS_TABLE_REPLY) || topic.name() == QString(RUN_TEST_PROJECT_REPLY))
    {
        //接受中位机上报消息
        Q_EMIT handleMiddleInfo(ParseMessageInfo::parseMiddleReceived(QString(message)), id);
    }
    else if(topic.name() == QString(MACHINE_RESET_REPLY))
    {
        //接受中位机整机复位消息
        Q_EMIT handleMiddleMachineResetInfo(ParseMessageInfo::parseMiddleReceived(QString(message)), id);
    }
}

void MqttCommunication::onStateChanged()
{
    QMqttClient *sourceClient = qobject_cast<QMqttClient *>(sender());
    if(!sourceClient)
    {
        qCritical() << "Message received from unknown source";
        return;
    }

    ENUM_CLASS::PCR_Id id;
    for(auto it = m_clientMap.begin(); it != m_clientMap.end(); ++it)
    {
        if(it.value() == sourceClient)
        {
            id = it.key();
            break;
        }
    }

    if(sourceClient->state() == QMqttClient::Connected)
    {
        qDebug() << sourceClient->objectName() << "connected!";

        if(id == ENUM_CLASS::Middle)
        {
            subscribeToTopic(id, LOAD_CARRIERS_REPLY);
            subscribeToTopic(id, LOAD_TEST_PROJECT_REPLY);
            subscribeToTopic(id, LOAD_PROCESS_TABLE_REPLY);
            subscribeToTopic(id, RUN_TEST_PROJECT_REPLY);
            subscribeToTopic(id, MACHINE_RESET_REPLY);
        }
        else
        {
            subscribeToTopic(id, TC384_CONTROL_UP);
            subscribeToTopic(id, TC384_DEBUG_UP);
            subscribeToTopic(id, TC384_CHANGE_UP);
        }
    }
    else if(sourceClient->state() == QMqttClient::Connecting)
    {
        qDebug() << sourceClient->objectName() << "connecting...";
    }
    else if(sourceClient->state() == QMqttClient::Disconnected)
    {
        qDebug() << sourceClient->objectName() << "failed to connect!";
    }
}
