#include "messagedialog.h"
#include "ui_messagedialog.h"

#include <QPainter>
#include <QStyleOption>
#include <QDebug>

MessageDialog::MessageDialog(const QString& msg, Flags flag,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageDialog), m_msg(msg)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    setMsg(msg);
    updateIcon(flag);
}

MessageDialog::~MessageDialog()
{
    delete ui;
}

void MessageDialog::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void MessageDialog::on_btnConfirm_clicked()
{
    QDialog::accept();
}

QString MessageDialog::msg() const
{
    return m_msg;
}

void MessageDialog::setMsg(const QString &msg)
{
    ui->labMessage->setText(msg);
    m_msg = msg;
}

void MessageDialog::setConfirmText(const QString &msg)
{
    ui->btnConfirm->setText(msg);
}

void MessageDialog::updateIcon(MessageDialog::Flags flag)
{
    if(Fault == flag)
    {
        QPixmap pix(":/res/ico/error.png");
        ui->TipIcon->setPixmap(pix);
        ui->labFlag->setText("错误");
    }
    else if(Warning == flag)
    {
        QPixmap pix(":/res/ico/warning.png");
        ui->TipIcon->setPixmap(pix);
        ui->labFlag->setText("警告");
    }
}

void MessageDialog::setBtnEnable(bool enable)
{
    ui->btnConfirm->setEnabled(enable);
}

void MessageDialog::showSpinner()
{
    m_spinner = new HSpinner(ui->btnConfirm);
    m_spinner->setGeometry(ui->btnConfirm->geometry());

    m_spinner->start();
}

void MessageDialog::hideSpinner()
{
    if(m_spinner)
        m_spinner->hide();
}

void MessageDialog::click()
{
    ui->btnConfirm->click();
}
