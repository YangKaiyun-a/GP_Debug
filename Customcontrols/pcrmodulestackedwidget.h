#ifndef PCRMODULESTACKEDWIDGET_H
#define PCRMODULESTACKEDWIDGET_H
#include "Function/data/global.h"

#include <QWidget>

namespace Ui {
class PCRModuleStackedWidget;
}

class PCRModuleStackedWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PCRModuleStackedWidget(QWidget *parent = nullptr);
    ~PCRModuleStackedWidget();
    void init();
    void insertStackedWidget(ENUM_CLASS::PCR_Fuction_Type type);

private slots:
    void on_btnM1_clicked();

    void on_btnM2_clicked();

private:
    Ui::PCRModuleStackedWidget *ui;
};

#endif // PCRMODULESTACKEDWIDGET_H
