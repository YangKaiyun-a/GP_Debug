#include "fluorescencecurve.h"

#include <QChartView>
#include <QVBoxLayout>
#include <QValueAxis>

FluorescenceCurve::FluorescenceCurve(QWidget *parent)
    : QWidget{parent}
{
    init();
}

void FluorescenceCurve::init()
{
    chart = new QChart();
    chart->legend()->setLabelColor("#FFFFFF");
    QFont font;
    font.setPixelSize(18);
    chart->legend()->setFont(font);
    chart->setBackgroundBrush(QBrush("#3F4242"));

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(chartView);
    setLayout(layout);

    initSeries();
}

void FluorescenceCurve::initSeries()
{
    chart->removeAllSeries();
    m_lineSeriesLst.clear();
    m_textItemLst.clear();  // 初始化文本项列表

    QList<QColor> colorLst{Qt::blue, Qt::green, Qt::yellow, Qt::red};
    for (int i = 0; i < 4; ++i)
    {
        QSplineSeries *series = new QSplineSeries;
        QPen pen;
        pen.setWidth(2);
        pen.setColor(colorLst.at(i));
        series->setPen(pen);
        m_lineSeriesLst.append(series);
        chart->addSeries(series);

        // 创建文本项，并添加到图表的场景中
        QGraphicsTextItem *textItem = new QGraphicsTextItem();
        textItem->setDefaultTextColor(colorLst.at(i));
        m_textItemLst.append(textItem);
        chart->scene()->addItem(textItem);
    }
    m_lineSeriesLst.at(0)->setName("Block1");
    m_lineSeriesLst.at(1)->setName("Block2");
    m_lineSeriesLst.at(2)->setName("Block3");
    m_lineSeriesLst.at(3)->setName("Lid");

    chart->createDefaultAxes();
    auto hAxes = chart->axes(Qt::Horizontal).first();
    auto horizontalAxes = qobject_cast<QValueAxis*>(hAxes);
    horizontalAxes->setLabelsColor(Qt::white);
    horizontalAxes->setLabelFormat("%d");
    horizontalAxes->setRange(0,5);
    QFont font;
    font.setPixelSize(18);
    horizontalAxes->setLabelsFont(font);
    horizontalAxes->setLinePenColor("#8A8888");
    horizontalAxes->setGridLineVisible(false);

    auto vAxes = chart->axes(Qt::Vertical).first();
    auto verticalAxes = qobject_cast<QValueAxis*>(vAxes);
    verticalAxes->setLabelsColor(Qt::white);
    verticalAxes->setRange(0,120);
    verticalAxes->setTickCount(5);
    verticalAxes->setLabelsFont(font);
    verticalAxes->setLinePenColor("#8A8888");
    QPen pen;
    pen.setColor("#8A8888");
    pen.setStyle(Qt::DashLine);
    verticalAxes->setGridLinePen(pen);
}

void FluorescenceCurve::appendLineData(const RealTimeTemperature &temp)
{
    QList<double> data;
    data << temp.block1 << temp.block2 << temp.block3 << temp.heatCap;

    double max = 0;
    for (int i = 0; i < data.count() && i < m_lineSeriesLst.count(); ++i)
    {
        auto series = m_lineSeriesLst.at(i);
        series->append(series->count() + 1, data.at(i));
        if(data.at(i) > max)
        {
            max = data.at(i);
        }

        // 更新文本项的位置和内容
        QPointF lastPoint = series->points().last();
        auto textItem = m_textItemLst.at(i);
        textItem->setPos(chart->mapToPosition(lastPoint, series));
        textItem->setPlainText(QString::number(data.at(i)));
    }

    if(max > maxY)
    {
        while (max > maxY)
        {
            maxY = maxY + 40.0;
        }
        chart->axes(Qt::Vertical).first()->setRange(0, maxY);
    }
    chart->axes(Qt::Horizontal).first()->setRange(0, m_lineSeriesLst.at(0)->count() + 5);
}

void FluorescenceCurve::resetChart()
{
    for(auto series : m_lineSeriesLst)
    {
        series->clear();
    }

    auto hAxes = chart->axes(Qt::Horizontal).first();
    auto horizontalAxes = qobject_cast<QValueAxis*>(hAxes);
    horizontalAxes->setRange(0, 5);

    auto vAxes = chart->axes(Qt::Vertical).first();
    auto verticalAxes = qobject_cast<QValueAxis*>(vAxes);
    verticalAxes->setRange(0, 120);

    maxY = 120.0;

    // 清除所有文本项
    for(auto textItem : m_textItemLst)
    {
        textItem->setPlainText("");
    }
}
