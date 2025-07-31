#ifndef LOGWIDGET_H
#define LOGWIDGET_H
#include "Function/data/global.h"


#include <QWidget>

namespace Ui {
class LogWidget;
}

class LogWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LogWidget(QWidget *parent = nullptr);
    ~LogWidget();
    void init();

private slots:
    void onShowLog(const Storage_Info &storageInfo, const PipetteInfo &pipetteInfo, const ClawInfo &clawInfo, const LaserInfo &laserInfo, float deviationX, float deviationY, float deviationZ, float x, float y, float z, float resetZ0, const SingleChannelInfo &singleInfo, const QString &str);
    void on_btnBack_clicked();

    void on_btnClear_clicked();

private:
    Ui::LogWidget *ui;
};

#endif // LOGWIDGET_H
