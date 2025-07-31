#ifndef MQTTCOMMUNICATION_H
#define MQTTCOMMUNICATION_H
#include "global.h"

#include <QObject>
#include <QtMqtt/QMqttClient>
#include <QMap>

class MqttCommunication : public QObject {
    Q_OBJECT

public:
    explicit MqttCommunication(QObject *parent = nullptr);
    static MqttCommunication* instance();
    void init();
    void connectToServer(QMqttClient *client, const QString &host, int port);
    void publishMessage(ENUM_CLASS::PCR_Id id, const QString &topic, const QByteArray &message = "");
    void subscribeToTopic(ENUM_CLASS::PCR_Id id, const QString &topic);
    void unsubscribeToTopic(ENUM_CLASS::PCR_Id id, const QString& topic);
    bool isClientConnected(ENUM_CLASS::PCR_Id id);

private slots:
    void onMessageReceived(const QByteArray &message, const QMqttTopicName &topic);
    void onStateChanged();

signals:
    //发送PCR回复的消息
    void handleMessageInfo(const MessageInfo &info, ENUM_CLASS::PCR_Id id);
    //发送中位机回复的消息
    void handleMiddleInfo(const MiddleMessageInfo &info, ENUM_CLASS::PCR_Id id);
    //发送中位机复位成功消息
    void handleMiddleMachineResetInfo(const MiddleMessageInfo &info, ENUM_CLASS::PCR_Id id);
    //发送PCR实验完成的消息
    void handleExperimentFinished(const MessageInfo &info, ENUM_CLASS::PCR_Id id);

private:
    QMqttClient *m_client1 = nullptr;               //PCR M1
    QMqttClient *m_client2 = nullptr;               //PCR M2
    QMqttClient *m_middleClient = nullptr;          //中位机
    QMap<ENUM_CLASS::PCR_Id, QMqttClient*> m_clientMap;
    static MqttCommunication* m_mqttCommunication;
};

#endif // MQTTCOMMUNICATION_H
