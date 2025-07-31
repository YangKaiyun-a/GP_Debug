#ifndef LOGISTICSWIDGET_H
#define LOGISTICSWIDGET_H

#include <QWidget>

namespace Ui {
class LogisticsWidget;
}

class LogisticsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LogisticsWidget(QWidget *parent = nullptr);
    ~LogisticsWidget();
    void init();
    void initStyleSheet();
    void initData();
    void initUI();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_24_clicked();

private:
    Ui::LogisticsWidget *ui;
};

#endif // LOGISTICSWIDGET_H
