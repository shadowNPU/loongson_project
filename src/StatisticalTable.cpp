#include <QtCore/QString>
#include <QtCore/QDateTime>

#include <QtCharts/QChartView>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarCategoryAxis>

#include <QtWidgets/QHBoxLayout>

#include "StatisticalTable.hpp"


StatisticalTable::StatisticalTable(QWidget *parent) : QWidget(parent) {
    axis = new QBarCategoryAxis; // 条形图横坐标
    barSeries = new QBarSeries; // 条形图系列
    pieSeries = new QPieSeries(); // 扇形图系列

    setData();

    // 条形图
    barChart = new QChart;
    barChart->setTitle("振幅统计表");
    barChart->addSeries(barSeries);
    barChart->setAxisX(axis, barSeries);
    barChart->setAnimationOptions(QChart::SeriesAnimations);
    barChart->createDefaultAxes();
    barChart->legend()->setVisible(true);
    barChart->legend()->setAlignment(Qt::AlignBottom);

    auto barView = new QChartView;
    barView->setRenderHint(QPainter::Antialiasing);
    barView->setChart(barChart);

    // 扇形图
    pieChart = new QChart;
    pieChart->setTitle("振幅占比");
    pieChart->addSeries(pieSeries);
    pieChart->setAnimationOptions(QChart::SeriesAnimations);
    pieChart->legend()->hide();

    auto pieView = new QChartView;
    pieView->setRenderHint(QPainter::Antialiasing);
    pieView->setChart(pieChart);

    // 布局
    auto mainLayout = new QHBoxLayout;
    mainLayout->addWidget(barView);
    mainLayout->addWidget(pieView);
    this->setLayout(mainLayout);
}

void StatisticalTable::setData() {
    static std::map<int, QString> monthToString = {
        {1, "一月"},
        {2, "二月"},
        {3, "三月"},
        {4, "四月"},
        {5, "五月"},
        {6, "六月"},
        {7, "七月"},
        {8, "八月"},
        {9, "九月"},
        {10, "十月"},
        {11, "十一月"},
        {12, "十二月"}
    };

    QDate currentDate = QDate::currentDate();

    int thisMonth = currentDate.month();
    int lastMonth = (thisMonth + 11) % 12;
    int monthBeforeLast = (thisMonth + 10) % 12;
    QString thisMonthStr = monthToString[thisMonth];
    QString lastMonthStr = monthToString[lastMonth];
    QString monthBeforeLastStr = monthToString[monthBeforeLast];

    QStringList categories;
    categories << monthBeforeLastStr << lastMonthStr << thisMonthStr;
    axis->append(categories);

    // 数据
    QBarSet *minValue = new QBarSet("最大值");
    QBarSet *meanValue = new QBarSet("平均值");
    QBarSet *maxValue = new QBarSet("最小值");

    *minValue << 1 << 2 << 3;
    *meanValue << 5 << 4 << 6;
    *maxValue << 3 << 5 << 2;

    barSeries->append(minValue);
    barSeries->append(meanValue);
    barSeries->append(maxValue);

    //
    pieSeries->append("1", 1);
    pieSeries->append("2", 2);
    pieSeries->append("3", 3);
    pieSeries->append("4", 4);
    pieSeries->append("5", 5);

    auto slice = pieSeries->slices().at(1);
    slice->setExploded();
    slice->setLabelVisible();
    slice->setPen(QPen(Qt::darkGreen, 2));
    slice->setBrush(Qt::green);
}

void StatisticalTable::loadData() {
}
