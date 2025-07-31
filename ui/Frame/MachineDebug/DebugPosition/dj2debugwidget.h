#ifndef DJ2DEBUGWIDGET_H
#define DJ2DEBUGWIDGET_H

#include <QWidget>

namespace Ui {
class DJ2DebugWidget;
}

class DJ2DebugWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DJ2DebugWidget(QWidget *parent = nullptr);
    ~DJ2DebugWidget();
    void init();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_15_clicked();

private:
    Ui::DJ2DebugWidget *ui;
};

#endif // DJ2DEBUGWIDGET_H
