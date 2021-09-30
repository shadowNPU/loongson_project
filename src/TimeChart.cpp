#include <QtCore/QDateTime>

#include <QtCharts/QChartView>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>

#include "TimeChart.hpp"


TimeChart::TimeChart(QWidget *parent) : QChartView(parent) {
    QPen penY(Qt::darkBlue, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    QDateTimeAxis *axisX = new QDateTimeAxis;  // X轴
    axisX->setTickCount(10);  // 设置坐标轴格数
    axisX->setFormat("ss"); // 设置时间显示格式
    axisX->setTitleText("时间");
    axisX->setLinePen(penY);

    QValueAxis *axisY = new QValueAxis; // Y轴
    axisY->setTickCount(5);
    axisY->setMin(-50.0);             // 设置Y轴范围
    axisY->setMax(50.0);
    axisY->setLinePenColor(QColor(Qt::darkBlue)); // 设置坐标轴颜色样式
    axisY->setGridLineColor(QColor(Qt::darkBlue));
    axisY->setGridLineVisible(false); // 设置Y轴网格不显示
    axisY->setLinePen(penY);
    axisY->setTitleText("位移");

    chart = new QChart;
    chart->addAxis(axisX, Qt::AlignBottom); // 设置坐标轴位于chart中的位置
    chart->addAxis(axisY, Qt::AlignLeft);
    chart->legend()->hide();  // 隐藏图例

    series = new QSplineSeries;
    chart->addSeries(series); // 把线添加到chart
    series->attachAxis(axisX); // 把数据添加到坐标轴上
    series->attachAxis(axisY);

    this->setChart(chart);
    this->setRenderHint(QPainter::Antialiasing); //设置抗锯齿
}

void TimeChart::setValue(QDateTime l, QDateTime r, qreal x, qreal y) {
    series->append(x, y);
    chart->axisX()->setMin(l);
    chart->axisX()->setMax(r);
}

void TimeChart::clearValue() {
    qDebug("清空曲线");
    series->clear();
}
