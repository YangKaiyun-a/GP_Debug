#ifndef DEBUGWIDGET_H
#define DEBUGWIDGET_H

#include <QWidget>

namespace Ui {
class DebugWidget;
}

class DebugWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DebugWidget(QWidget *parent = nullptr);
    ~DebugWidget();
    void init();
    void initStackedWidget();

private slots:
    void on_btnDJ1_clicked();

    void on_btnDJ2_clicked();

    void on_btnDP1_clicked();

    void on_btnDP8_clicked();

private:
    Ui::DebugWidget *ui;
};

#endif // DEBUGWIDGET_H
