#include "languagebutton.h"
#include "application.h"

#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>

LanguageButton::LanguageButton(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(100, 50);
    setAttribute(Qt::WA_TranslucentBackground);
}

void LanguageButton::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setColor(QColor(0, 182, 206));
    painter.setPen(pen);

    QRect rect(0, 0, width(), height());
    int side = qMin(width(), height());

    //左侧半圆
    QPainterPath path1;
    path1.addEllipse(rect.x(), rect.y(), side, side);

    //右侧半圆
    QPainterPath path2;
    path2.addEllipse(rect.width() - side, rect.y(), side, side);

    //中间的矩形
    QPainterPath path3;
    path3.addRect(rect.x() + side / 2, rect.y(), rect.width() - side, height());
    //中间矩形的一半
    QPainterPath path4;
    path4.addRect(rect.x() + side / 2, rect.y(), (rect.width() - side) / 2, height());
    QPainterPath path5;
    path5.addRect(rect.x() + side, rect.y(), (rect.width() - side) / 2, height());

    QPainterPath leftPath = path1 + path4;
    QPainterPath rightPath = path2 + path5;

    if(m_leftSideSelected)
    {
        painter.setBrush(QColor(0, 182, 206));
        painter.drawPath(leftPath);

        painter.setBrush(QColor(0, 21, 30));
        painter.drawPath(rightPath);
    }
    else
    {
        painter.setBrush(QColor(0, 182, 206));
        painter.drawPath(rightPath);

        painter.setBrush(QColor(0, 21, 30));
        painter.drawPath(leftPath);
    }

    //绘制文本
    int midPoint = width() / 2; // 找到中点
    painter.setPen(Qt::white);
    QRect leftRect(0, 0, midPoint, height());
    QRect rightRect(midPoint, 0, midPoint, height());
    painter.drawText(leftRect, Qt::AlignCenter, tr("中文"));
    painter.drawText(rightRect, Qt::AlignCenter, tr("EN"));
}

void LanguageButton::mousePressEvent(QMouseEvent *ev)
{
    m_leftSideSelected = (ev->x() < width() / 2);
    update();
    Q_EMIT hApp->m_sigmanager->languageChanged(m_leftSideSelected ? ENUM_CLASS::Chinese : ENUM_CLASS::English);
}

