#include "processsteparrow.h"
#include "ui_processsteparrow.h"
#include "application.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

#define GRAPHICALHEIGHT (height()*2)/3

const int LINE_WIDTH = 3;
const int LEFT_MARGIN = 20;
const int RIGHT_MARGIN = 20;
const int TOP_BOTTOM_MARGIN = 10;
const QColor LINE_COLOR = QColor(94,94,94);
const QColor LINE_COLOR_ACTIVE = QColor(132,132,132);
const QColor LINE_COLOR_SELECTED = QColor(0,187,187);
const int TRIANGLE_SIDE_LENGTH = 20;//箭头三角形边长
const int CIRCLE_SIZE = 30;

ProcessStepArrow::ProcessStepArrow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProcessStepArrow)
{
    ui->setupUi(this);
    init();
}

ProcessStepArrow::ProcessStepArrow(QStringList &text, QWidget *parent):
    QWidget(parent),m_stepText(text),
    ui(new Ui::ProcessStepArrow)
{
    ui->setupUi(this);
    init();
}

ProcessStepArrow::~ProcessStepArrow()
{
    delete ui;
}

void ProcessStepArrow::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing );
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    drawBackground(&painter);//画进度条背景色
    drawActive(&painter);//画进激活节点线条颜色
    drawStep(&painter);//画进度条填充色
    return QWidget::paintEvent(event);
}

void ProcessStepArrow::drawBackground(QPainter *painter)
{
    //画线
    QPen pen;
    pen.setColor(LINE_COLOR);
    pen.setWidth(LINE_WIDTH);
    painter->setPen(pen);
    QPoint startPoint,lastPoint;
    startPoint.setX(LEFT_MARGIN+RIGHT_MARGIN+TRIANGLE_SIDE_LENGTH/2+CIRCLE_SIZE/2);
    startPoint.setY(GRAPHICALHEIGHT/2);
    lastPoint.setX(width()-LEFT_MARGIN-RIGHT_MARGIN-TRIANGLE_SIDE_LENGTH/2);
    lastPoint.setY(startPoint.y());
    painter->drawLine(startPoint,lastPoint);

    //画箭头
    painter->save();
    painter->setPen(Qt::NoPen);
    QPainterPath pathBoard;
    QPolygonF pointsBoard;
    pointsBoard.append(QPointF(width()-LEFT_MARGIN-RIGHT_MARGIN-TRIANGLE_SIDE_LENGTH/2,GRAPHICALHEIGHT/2-TRIANGLE_SIDE_LENGTH/2));
    pointsBoard.append(QPointF(width()-LEFT_MARGIN-RIGHT_MARGIN,GRAPHICALHEIGHT/2));
    pointsBoard.append(QPointF(width()-LEFT_MARGIN-RIGHT_MARGIN-TRIANGLE_SIDE_LENGTH/2,GRAPHICALHEIGHT/2+TRIANGLE_SIDE_LENGTH/2));
    pathBoard.addPolygon(pointsBoard);
    painter->drawPath(pathBoard);
    painter->fillPath(pathBoard, QBrush(LINE_COLOR));
    painter->restore();
    return;
    //画圆
//    painter->setBrush(QColor("gray"));
//    QPainterPath CirclePath;
//    for (int i = 0;i<m_stepText.size();i++) {
//        CirclePath.addEllipse((width())/(m_stepText.size())*i+RIGHT_MARGIN+LEFT_MARGIN+CIRCLE_SIZE/2,GRAPHICALHEIGHT/2-CIRCLE_SIZE/2,CIRCLE_SIZE,CIRCLE_SIZE);
//    }
//    painter->drawPath(CirclePath);
//    painter->restore();
//    //绘制文本
//    for (int i = 0;i<m_stepText.size();i++) {
//        QRect rectText;
//        rectText.setX(width()/m_stepText.size()*i);
//        rectText.setY(GRAPHICALHEIGHT);
//        rectText.setWidth(width()/m_stepText.size());
//        rectText.setHeight(height()-GRAPHICALHEIGHT);
//        painter->drawText(rectText,Qt::AlignCenter,m_stepText.at(i));
//    }
}

void ProcessStepArrow::drawStep(QPainter *painter)
{
    painter->save();
    QPen pen;
    pen.setColor(LINE_COLOR_SELECTED);
    pen.setWidth(LINE_WIDTH);
    painter->setPen(pen);

    int itemWidth = 0;
    if(m_stepCount != 0)
    {
        itemWidth = width() / m_stepCount;
    }
    else
    {
        return;
    }

    QPoint startPoint,lastPoint;
    startPoint.setX(m_finishStartIndex*itemWidth + itemWidth/2);
    startPoint.setY(GRAPHICALHEIGHT/2);
    lastPoint.setX(m_finishLastIndex*itemWidth + itemWidth/2);
    lastPoint.setY(GRAPHICALHEIGHT/2);
    painter->drawLine(startPoint,lastPoint);
    painter->restore();
}

void ProcessStepArrow::drawActive(QPainter *painter)
{
    painter->save();
    QPen pen;
    pen.setColor(LINE_COLOR_ACTIVE);
    pen.setWidth(LINE_WIDTH);
    painter->setPen(pen);

    int itemWidth = 0;
    if(m_stepCount != 0)
    {
        itemWidth = width() / m_stepCount;
    }
    else
    {
        return;
    }

    QPoint startPoint,lastPoint;
    startPoint.setX(m_activeStartIndex*itemWidth + itemWidth/2);
    startPoint.setY(GRAPHICALHEIGHT/2);
    lastPoint.setX(m_activeLastIndex*itemWidth + itemWidth/2);
    lastPoint.setY(GRAPHICALHEIGHT/2);
    painter->drawLine(startPoint,lastPoint);
    painter->restore();
}

void ProcessStepArrow::init()
{
    m_stepCount = m_stepText.size();
    m_finishStartIndex = 0;
    m_finishLastIndex = 0;
    m_indexStep = 0;
    m_activeStartIndex = 0;
    m_activeLastIndex = 0;
}

QStringList ProcessStepArrow::stepText() const
{
    return m_stepText;
}

void ProcessStepArrow::setStepText(const QStringList &stepText)
{
    m_stepText = stepText;
    m_stepCount = stepText.size();
    update();
}

void ProcessStepArrow::setActiveRange(int start, int last)
{
    m_activeStartIndex = start;
    m_activeLastIndex = last;
    update();
}

void ProcessStepArrow::setFinishRange(int start, int last)
{
    m_finishStartIndex = start;
    m_finishLastIndex = last;
    update();
}

void ProcessStepArrow::reFresh()
{
    init();
    update();
}

QBrush ProcessStepArrow::fillBrush() const
{
    return m_fillBrush;
}

void ProcessStepArrow::setFillBrush(const QBrush &fillBrush)
{
    m_fillBrush = fillBrush;
    update();
}

QBrush ProcessStepArrow::defaultBrush() const
{
    return m_defaultBrush;
}

void ProcessStepArrow::setDefaultBrush(const QBrush &defaultBrush)
{
    m_defaultBrush = defaultBrush;
    update();
}

void ProcessStepArrow::setIndexStep(int indexStep)
{
    if(indexStep > m_stepCount) return;
    m_indexStep = indexStep;
    update();
}

void ProcessStepArrow::setStepCount(int stepCount)
{
    m_stepCount = stepCount;
}

int ProcessStepArrow::stepCount() const
{
    return m_stepCount;
}

int ProcessStepArrow::indexStep() const
{
    return m_indexStep;
}
