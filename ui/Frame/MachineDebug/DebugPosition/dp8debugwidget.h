#ifndef DP8DEBUGWIDGET_H
#define DP8DEBUGWIDGET_H

#include <QWidget>

namespace Ui {
class DP8DebugWidget;
}

class DP8DebugWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DP8DebugWidget(QWidget *parent = nullptr);
    ~DP8DebugWidget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_22_clicked();

private:
    Ui::DP8DebugWidget *ui;
};

#endif // DP8DEBUGWIDGET_H
