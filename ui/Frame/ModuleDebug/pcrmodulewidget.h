#ifndef PCRMODULEWIDGET_H
#define PCRMODULEWIDGET_H

#include <QWidget>

namespace Ui {
class PCRModuleWidget;
}

class PCRModuleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PCRModuleWidget(QWidget *parent = nullptr);
    ~PCRModuleWidget();
    void init();
    void initStackedWidget();

private slots:
    void on_btnPosition_clicked();

    void on_btnTemperature_clicked();

    void on_btnLight_clicked();

private:
    Ui::PCRModuleWidget *ui;
};

#endif // PCRMODULEWIDGET_H
