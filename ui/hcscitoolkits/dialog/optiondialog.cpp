#include "optiondialog.h"
#include "ui_optiondialog.h"

#include <QPainter>
#include <QStyleOption>

OptionDialog::OptionDialog(const QString& msg,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionDialog), m_msg(msg)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_StyledBackground);
    setMsg(msg);
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

void OptionDialog::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void OptionDialog::on_btnConfirm_clicked()
{
    QDialog::accept();
}

void OptionDialog::on_btnCancel_clicked()
{
    QDialog::reject();
}

QString OptionDialog::msg() const
{
    return m_msg;
}

void OptionDialog::setMsg(const QString &msg)
{
    ui->labMessage->setText(msg);
    m_msg = msg;
}

void OptionDialog::setOkMessage(const QString &msg)
{
    ui->btnConfirm->setText(msg);
}

void OptionDialog::setCancelMessage(const QString &msg)
{
    ui->btnCancel->setText(msg);
}

void OptionDialog::setOkEnable(bool enable)
{
    ui->btnConfirm->setEnabled(enable);
}

void OptionDialog::setCancelEnable(bool enable)
{
    ui->btnCancel->setEnabled(enable);
}

void OptionDialog::showSpinner()
{
    m_spinner = new HSpinner(ui->btnConfirm);
    m_spinner->setGeometry(ui->btnConfirm->geometry());
    m_spinner->start();
}

void OptionDialog::hideSpinner()
{
    if(m_spinner)
        m_spinner->hide();
}


