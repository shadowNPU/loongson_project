#pragma once

#include <chrono>
#include <thread>
#include <atomic>

#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "TimeChart.hpp"


class VideoChart : public QWidget {
    Q_OBJECT

public:
    VideoChart(QWidget *parent = nullptr) : QWidget(parent) {
        label = new QLabel;
        chart = new TimeChart;
        auto vLayout = new QVBoxLayout;
        vLayout->addWidget(label);
        vLayout->addWidget(chart);
        this->setLayout(vLayout);
    }

    ~VideoChart() {
        finish();
        thd.join();
        qDebug("视频线程退出");
    }

    template <typename T, typename F>
    void open(T filename, F&&func) {
        thd = std::thread([=, this] {
            using namespace std::literals;

            cv::VideoCapture cap;
            if (!cap.open(filename)) {
                qDebug("Cannot open camera");
            }

            cv::Mat frame;
            cv::Mat rgbFrame;
            while (!done) {
                while (!ready) {
                    std::this_thread::yield();
                    continue;
                }
                if (cap.read(frame)) {
                    // BGR -> RGB
                    cv::cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB);

                    // opencv BGR -> QPixmap
                    unsigned char *data = rgbFrame.data;
                    int width = rgbFrame.cols;
                    int height = rgbFrame.rows;
                    auto pixmap = QPixmap::fromImage(QImage(data, width, height, QImage::Format_RGB888));

                    this->label->setPixmap(pixmap);

                    func(frame);

                    std::this_thread::sleep_for(200ms); // 每帧间隔
                }
            }
            cap.release();
            emit finished();
        });
    }

    void start() {
        ready = true;
    }

    void stop() {
        ready = false;
    }

    void finish() {
        done = true;
    }

    bool isFinished() {
        return done;
    }

signals:
    void pixmapChanged(cv::Mat const &);
    void finished();

// private:
public:
    QLabel *label;
    TimeChart *chart;
    std::thread thd;
    std::atomic<bool> ready = ATOMIC_VAR_INIT(false);
    std::atomic<bool> done = ATOMIC_VAR_INIT(false);
};
