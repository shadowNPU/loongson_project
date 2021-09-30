#pragma once

#include <map>

#include <QtCore/QString>
#include <QtCore/QDateTime>

#include <QtCharts/QChartView>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>

#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QPieSeries>


QT_CHARTS_USE_NAMESPACE // 使用 QChart 命名空间


class StatisticalTable : public QWidget {
    Q_OBJECT
public:
    StatisticalTable(QWidget *parent = nullptr);

private:
    void setData();
    void loadData();

    QBarSeries *barSeries;
    QPieSeries *pieSeries;
    QBarCategoryAxis *axis;
    QChart *barChart;
    QChart *pieChart;
};
