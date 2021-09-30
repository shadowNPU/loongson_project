#include "mainwindow.h"
#include "settings.h"
#include "./ui_mainwindow.h"
#include "./ui_settings.h"

#include <QtCore/QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    this->setWindowTitle("监测主窗口");

    ui->cameraButton->setStyleSheet("QPushButton{background-color: rgb(248, 248, 255);border:2px groove gray;border-radius:6px;padding:2px 4px;border-style: outset;}"
                                    "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                    "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
    ui->resetStartPointButton->setStyleSheet("QPushButton{background-color: rgb(248, 248, 255);border:2px groove gray;border-radius:6px;padding:2px 4px;border-style: outset;}"
                                             "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                             "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
    ui->startMeasureButton->setStyleSheet("QPushButton{background-color: rgb(248, 248, 255);border:2px groove gray;border-radius:6px;padding:2px 4px;border-style: outset;}"
                                          "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                          "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
    ui->stopMeasureButton->setStyleSheet("QPushButton{background-color: rgb(248, 248, 255);border:2px groove gray;border-radius:6px;padding:2px 4px;border-style: outset;}"
                                         "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                         "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
    ui->selectTargetButton->setStyleSheet("QPushButton{background-color: rgb(248, 248, 255);border:2px groove gray;border-radius:6px;padding:2px 4px;border-style: outset;}"
                                          "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                          "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
    ui->dataChartButton->setStyleSheet("QPushButton{background-color: rgb(248, 248, 255);border:2px groove gray;border-radius:6px;padding:2px 4px;border-style: outset;}"
                                       "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                       "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");

    ui->settingButton->setStyleSheet("QPushButton{background-color: rgb(245,245,245);border:2px groove gray;border-radius:12px;padding:2px 4px;border-style: outset;}"
                                     "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
                                     "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");

    label = new VideoChart;
    label->setStyleSheet("border:2px solid blue;");
    label->open(0, [this] (auto &image) {
        static double dx = 0.0;
        static double x0 = NaN;

        if (pauseChart) {
            return;
        }
        qDebug() << pauseChart;
        double x = computeX(image, closingSize, gaussSize, threshold, maxAera, minAera);
        x *= rate;

        if (std::isnan(x0)) {
            x0 = x;
        } else {
            if (!std::isnan(x)) {
                dx = x - x0;
            }
        }

        // 修改图表
        QDateTime currentTime = QDateTime::currentDateTime();
        auto l = currentTime.addSecs(-60 * 1);
        auto r = currentTime;
        auto t = currentTime.toMSecsSinceEpoch();
        this->label->chart->setValue(l, r, t, dx);
     });

    // chart = new TimeChart;
    // ui->dataChartLayout->addWidget(chart);
    ui->camViewLayout->addWidget(label);

    // 按钮事件
    ui->cameraButton->setCheckable(true);
    connect(ui->cameraButton, &QPushButton::toggled, [this](bool checked) {
        if (checked) {
            label->start();
            ui->cameraButton->setText("关闭摄像头");
        } else {
            label->stop();
            ui->cameraButton->setText("开启摄像头");
        }
    });
    connect(ui->dataChartButton, &QPushButton::clicked, [](bool checked) {
        auto table = new StatisticalTable;
        table->show();
    });
    connect(ui->settingButton, &QPushButton::clicked, [this](bool checked) {
        auto dialog = new Settings;
        dialog->show();
        connect(dialog, &Settings::paramsChanged, this, &MainWindow::onParamsChanged);
    });
    connect(ui->startMeasureButton, &QPushButton::clicked, [this](bool checked) {
        pauseChart = false;
    });
    connect(ui->stopMeasureButton, &QPushButton::clicked, [this](bool checked) {
        pauseChart = true;
    });
    connect(ui->resetStartPointButton, &QPushButton::clicked, [this](bool checked) {
        chart->clearValue();
    });
}


MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::onParamsChanged(QString closingSizeStr, QString gaussSizeStr, QString thresholdStr, QString rateStr, QString maxAeraStr, QString minAeraStr) {
    bool flag = false;
    int closingSize = closingSizeStr.toInt(&flag);
    if (flag) {
        this->closingSize = closingSize;
        flag = false;
    }
    int gaussSize = gaussSizeStr.toInt(&flag);
    if (flag) {
        this->gaussSize = gaussSize;
        flag = false;
    }
    int threshold = thresholdStr.toInt(&flag);
    if (flag) {
        this->threshold = threshold;
        flag = false;
    }
    double rate = rateStr.toDouble(&flag);
    if (flag) {
        this->rate = rate;
        flag = false;
    }
    int minAera = minAeraStr.toInt(&flag);
    if (flag) {
        this->minAera = minAera;
        flag = false;
    }
    int maxAera = maxAeraStr.toInt(&flag);
    if (flag) {
        this->maxAera = maxAera;
        flag = false;
    }
}
