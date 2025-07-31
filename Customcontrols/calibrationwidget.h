#ifndef CALIBRATIONWIDGET_H
#define CALIBRATIONWIDGET_H
#include "Function/data/global.h"

#include <QWidget>

namespace Ui {
class CalibrationWidget;
}

class CalibrationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CalibrationWidget(ENUM_CLASS::Light_Channel channel, QWidget *parent = nullptr);
    ~CalibrationWidget();
    void init();
    void setLabMessage(ENUM_CLASS::Light_Channel channel);
    void setResultMessage(const QString &msg);

private:
    Ui::CalibrationWidget *ui;
    ENUM_CLASS::Light_Channel m_channel;
};

#endif // CALIBRATIONWIDGET_H
