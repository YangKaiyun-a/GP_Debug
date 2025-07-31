#ifndef ACCURATINGPOSITIONWIDGET_H
#define ACCURATINGPOSITIONWIDGET_H
#include "logwidget.h"

#include <QWidget>

namespace Ui {
class AccuratingPositionWidget;
}

class AccuratingPositionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AccuratingPositionWidget(QWidget *parent = nullptr);
    ~AccuratingPositionWidget();
    void init();
    void initStackedWidget();
    void showLogWidget();

private slots:
    void on_btnDJ1_clicked();

    void on_btnDJ2_clicked();

    void on_btnDP1_clicked();

    void on_btnDP8_clicked();

    void on_btnLogistics_clicked();

    void on_btnDP8_2_clicked();

    void on_btnQPCR_clicked();

    void on_btnLog_clicked();

private:
    Ui::AccuratingPositionWidget *ui;
    LogWidget *m_logWidget = nullptr;
};

#endif // ACCURATINGPOSITIONWIDGET_H
