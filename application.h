#ifndef APPLICATION_H
#define APPLICATION_H

#include "Function/data/signalmanager.h"

#include <QObject>
#include <QApplication>

#define hApp (static_cast<Application *>(Application::instance()))




class Application : public QApplication
{
    Q_OBJECT
public:
    explicit Application(int &argc, char **argv);
    static Application* instance();
    static Application* instance(int &argc, char **argv);
    bool init();

public:
    SignalManager* m_sigmanager= nullptr;

signals:

private:
    static Application* m_singlenApplication;

};

#endif // APPLICATION_H
