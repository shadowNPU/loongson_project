#pragma once

#include <QtCore/QDateTime>

#include <QtCharts/QChartView>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>

using namespace QtCharts;


class TimeChart : public QChartView {
    Q_OBJECT
public:
    TimeChart(QWidget *parent = nullptr);

    void setValue(QDateTime l, QDateTime r, qreal x, qreal y);
    void clearValue();

private:
    QChart *chart;
    QSplineSeries *series; // 线
};
