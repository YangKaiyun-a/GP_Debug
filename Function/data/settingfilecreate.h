#ifndef SETTINGFILECREATE_H
#define SETTINGFILECREATE_H


#include <QObject>
#include <QDir>
#include <QVariant>
#include <QSettings>

class SettingFileCreate : public QObject
{
    Q_OBJECT
public:
    explicit SettingFileCreate(QObject *parent = nullptr);
    static SettingFileCreate* instance();
    void init();
    void CreateSettingFile();
    void SettingInit();

    QVariant sysValue(const QString& group, const QString& key, const QVariant& defaultValue = QVariant());

    //config的目录
    QDir m_configDir;
    //config的文件路径
    QString m_configFile;

private:
    static SettingFileCreate* m_settingFileCreate;
    QSettings *m_settings = nullptr;


};

#endif // SETTINGFILECREATE_H
