#ifndef DJ1DEBUGWIDGET_H
#define DJ1DEBUGWIDGET_H

#include <QWidget>

namespace Ui {
class DJ1DebugWidget;
}

class DJ1DebugWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DJ1DebugWidget(QWidget *parent = nullptr);
    ~DJ1DebugWidget();
    void init();
    void initStyleSheet();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

private:
    Ui::DJ1DebugWidget *ui;
};

#endif // DJ1DEBUGWIDGET_H
