#ifndef WORKFLOWEMPTYRUNWIDGET_H
#define WORKFLOWEMPTYRUNWIDGET_H

#include <QWidget>

namespace Ui {
class WorkflowEmptyRunWidget;
}

class WorkflowEmptyRunWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WorkflowEmptyRunWidget(QWidget *parent = nullptr);
    ~WorkflowEmptyRunWidget();
    void init();
    void initStyleSheet();

private:
    Ui::WorkflowEmptyRunWidget *ui;
};

#endif // WORKFLOWEMPTYRUNWIDGET_H
