#ifndef TEMPERATUREWIDGET_H
#define TEMPERATUREWIDGET_H
#include "fluorescencecurve.h"
#include "Function/data/global.h"

#include <QWidget>


class TemperatureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TemperatureWidget(ENUM_CLASS::PCR_Id id, QWidget *parent = nullptr);
    void init();
    void startGetTemperature();
    void reset();

private:
    FluorescenceCurve *m_wgt = nullptr;
    ENUM_CLASS::PCR_Id m_id;
    QDateTime m_lastDateTime;
    QTimer *m_timer = nullptr;
    QMetaObject::Connection m_conn;
};

#endif // TEMPERATUREWIDGET_H
