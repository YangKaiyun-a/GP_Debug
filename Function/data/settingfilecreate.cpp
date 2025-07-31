#include "settingfilecreate.h"

#include <QSettings>
#include <QDir>
#include <QDebug>

SettingFileCreate* SettingFileCreate::m_settingFileCreate = nullptr;

SettingFileCreate::SettingFileCreate(QObject *parent)
    : QObject{parent}
{
    init();
}

SettingFileCreate *SettingFileCreate::instance()
{
    if(!m_settingFileCreate)
    {
        m_settingFileCreate = new SettingFileCreate();
    }

    return m_settingFileCreate;
}

void SettingFileCreate::init()
{
    //创建配置文件
    CreateSettingFile();

    //判断文件是否存在
    if(!QFile::exists(m_configFile))
    {
        //不存在则创建一个新文件并打开
        QFile load_file(m_configFile);
        //检查文件的打开和存在性
        if (Q_UNLIKELY(!load_file.open(QIODevice::ReadOnly | QIODevice::Append) || !load_file.exists()))
        {
            qWarning() << "--------配置文件创建失败--------";
            return;
        }
        load_file.close();

        //参数初始化
        SettingInit();
    }

}

//创建配置文件
void SettingFileCreate::CreateSettingFile()
{
    //生成config目录
    m_configDir.setPath("config");

    //创建config文件夹（如果不存在）
    if(!m_configDir.exists())
    {
        m_configDir.mkpath(".");
    }

    //配置文件路径
    m_configFile = m_configDir.absoluteFilePath("config.ini");

    //指定INI文件路径
    if(!m_settings)
    {
        m_settings = new QSettings(m_configFile, QSettings::IniFormat, this);
    }
}

//参数初始化
void SettingFileCreate::SettingInit()
{
    m_settings->beginGroup("M1");
    m_settings->setValue("mqtt_HostName", "192.168.137.101");
    m_settings->setValue("mqtt_Port", 1883);
    m_settings->setValue("db_connected", false);
    m_settings->setValue("db_Host", "192.168.137.101");
    m_settings->setValue("db_Name", "mysql");
    m_settings->setValue("db_Port", "3306");
    m_settings->setValue("db_User", "mysql");
    m_settings->setValue("db_PWD", "123456");
    m_settings->endGroup();

    m_settings->beginGroup("M2");
    m_settings->setValue("mqtt_HostName", "192.168.137.102");
    m_settings->setValue("mqtt_Port", 1883);
    m_settings->setValue("db_connected", false);
    m_settings->setValue("db_Host", "192.168.137.102");
    m_settings->setValue("db_Name", "mysql");
    m_settings->setValue("db_Port", "3306");
    m_settings->setValue("db_User", "mysql");
    m_settings->setValue("db_PWD", "123456");
    m_settings->endGroup();

    m_settings->beginGroup("Position");
    m_settings->setValue("mqtt_HostName", "192.168.137.11");
    m_settings->setValue("mqtt_Port", 1883);
    m_settings->setValue("db_connected", false);
    m_settings->setValue("db_Host", "192.168.137.11");
    m_settings->setValue("db_Name", "GP");
    m_settings->setValue("db_Port", "5432");
    m_settings->setValue("db_User", "postgres");
    m_settings->setValue("db_PWD", "hcsci123456");
    m_settings->endGroup();

    //是否打开控制台
    m_settings->beginGroup("cmd");
    m_settings->setValue("enable", false);
    m_settings->endGroup();
}

QVariant SettingFileCreate::sysValue(const QString &group, const QString &key, const QVariant &defaultValue)
{
    if(!m_settings)
    {
        return QVariant();
    }

    QVariant value;

    m_settings->beginGroup(group);
    value = m_settings->value(key, defaultValue);
    m_settings->endGroup();

    return value;
}
