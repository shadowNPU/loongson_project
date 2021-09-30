#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <atomic>

#include <QtCore/QDateTime>

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QFrame>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>

#include "StatisticalTable.hpp"
#include "TimeChart.hpp"
#include "VideoChart.hpp"
#include "algo.hpp"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onParamsChanged(QString closingSize, QString gaussSize, QString threshold, QString rate, QString maxAera, QString minAera);

private:
    TimeChart *chart;
    VideoChart *label;
    Ui::MainWindow *ui;
    int closingSize = 60;
    int gaussSize = 3;
    double threshold = 100;
    double minAera = 10000;
    double maxAera = 100000;
    double rate = 1.0;

    std::atomic<bool> pauseChart = ATOMIC_VAR_INIT(false);
};
#endif // MAINWINDOW_H
