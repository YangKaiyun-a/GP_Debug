#include "workflowemptyrunwidget.h"
#include "ui_workflowemptyrunwidget.h"

WorkflowEmptyRunWidget::WorkflowEmptyRunWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WorkflowEmptyRunWidget)
{
    ui->setupUi(this);
    init();
}

WorkflowEmptyRunWidget::~WorkflowEmptyRunWidget()
{
    delete ui;
}

void WorkflowEmptyRunWidget::init()
{
    initStyleSheet();
}

void WorkflowEmptyRunWidget::initStyleSheet()
{

}
