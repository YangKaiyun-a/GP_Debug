#ifndef DP1DEBUGWIDGET_H
#define DP1DEBUGWIDGET_H

#include <QWidget>

namespace Ui {
class DP1DebugWidget;
}

class DP1DebugWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DP1DebugWidget(QWidget *parent = nullptr);
    ~DP1DebugWidget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_22_clicked();

private:
    Ui::DP1DebugWidget *ui;
};

#endif // DP1DEBUGWIDGET_H
