#include <algorithm>

#include <opencv2/opencv.hpp>

#include "algo.hpp"


/**
 * 基于Zernike矩的亚像素边缘检测
 */
std::vector<cv::Point2d> subpixelEdgeDetect(cv::Mat const &grayImage) {
    cv::Mat NewSmoothImage;
    cv::medianBlur(grayImage, NewSmoothImage, 13);
    cv::Mat NewAdaThresImage;
    cv::adaptiveThreshold(NewSmoothImage, NewAdaThresImage, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 7, 4);

    cv::Mat ZerImageM00;
    cv::filter2D(NewAdaThresImage, ZerImageM00, CV_64F, M00, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
    // filter2D( cannyImage, zerImageM00, CV_64F, M00, Point(-1,-1), 0, BORDER_DEFAULT);

    cv::Mat ZerImageM11R;
    cv::filter2D(NewAdaThresImage, ZerImageM11R, CV_64F, M11R, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
    // filter2D( cannyImage, zerImageM11R, CV_64F, M11R, Point(-1, -1), 0, BORDER_DEFAULT);

    cv::Mat ZerImageM11I;
    cv::filter2D(NewAdaThresImage, ZerImageM11I, CV_64F, M11I, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
    // filter2D( cannyImage, zerImageM11I, CV_64F, M11I, Point(-1, -1), 0, BORDER_DEFAULT);

    cv::Mat ZerImageM20;
    cv::filter2D(NewAdaThresImage, ZerImageM20, CV_64F, M20, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
    // filter2D( cannyImage, zerImageM20, CV_64F, M20, Point(-1, -1), 0, BORDER_DEFAULT);

    cv::Mat ZerImageM31R;
    cv::filter2D(NewAdaThresImage, ZerImageM31R, CV_64F, M31R, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
    // filter2D(cannyImage, zerImageM31R, CV_64F, M31R, Point(-1, -1), 0, BORDER_DEFAULT);

    cv::Mat ZerImageM31I;
    cv::filter2D(NewAdaThresImage, ZerImageM31I, CV_64F, M31I, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
    // filter2D(cannyImage, zerImageM31I, CV_64F, M31I, Point(-1, -1), 0, BORDER_DEFAULT);

    cv::Mat ZerImageM40;
    cv::filter2D(NewAdaThresImage, ZerImageM40, CV_64F, M40, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
    // filter2D(cannyImage, zerImageM40, CV_64F, M40, Point(-1, -1), 0, BORDER_DEFAULT);

    /**
     * 使用7个的7*7的Zernike模板（矩阵）M00、M11R、M11I、M20、M31R、M31I、M40，
     * 分别与图像进行卷积，得到每个像素点的七个Zernike矩Z00、Z11R、Z11I、Z20、Z31R、Z31I、Z40
     * 对于每个点，根据它的七个Zernike矩，求得距离参数l和灰度差参数k，当l和k都满足设定的条件时，
     * 则判读该点为边缘点，并进一步利用上述7个Zernike矩求出该点的亚像素级坐标
     * 如果l或k不满足设定的条件，则该点不是边缘点，转到下一个点求解距离参数l和灰度差参数k
     */
    std::vector<cv::Point2d> points;
    int row_number = NewAdaThresImage.rows;
    int col_number = NewAdaThresImage.cols;
    for (int i = 0; i < row_number; i++) {
        for (int j = 0; j < col_number; j++) {
            if (ZerImageM00.at<double>(i, j) == 0) {
                continue;
            }

            // 计算 theta 值
            double thetaTmp = atan2(ZerImageM31I.at<double>(i, j), ZerImageM31R.at<double>(i, j));

            // 计算 Z11'/Z31'
            double rotated_z11 = 0.0;
            rotated_z11 = sin(thetaTmp) * (ZerImageM11I.at<double>(i, j)) + cos(thetaTmp) * (ZerImageM11R.at<double>(i, j));
            double rotated_z31 = 0.0;
            rotated_z31 = sin(thetaTmp) * (ZerImageM31I.at<double>(i, j)) + cos(thetaTmp) * (ZerImageM31R.at<double>(i, j));

            // 计算 l
            double l_method1 = sqrt((5 * ZerImageM40.at<double>(i, j) + 3 * ZerImageM20.at<double>(i, j)) / (8 * ZerImageM20.at<double>(i, j)));
            double l_method2 = sqrt((5 * rotated_z31 + rotated_z11) / (6 * rotated_z11));
            double l = (l_method1 + l_method2) / 2;

            // 计算 k / h
            double k = 3 * rotated_z11 / 2 / pow((1 - l_method2 * l_method2), 1.5);
            // double h = (ZerImageM00.at<double>(i, j) - k * PI / 2 + k * asin(l_method2) + k * l_method2 * sqrt(1 - l_method2 * l_method2)) / PI;

            // 判断边缘
            double k_value = 20.0;
            double l_value = sqrt(2) / g_N;
            double absl = abs(l_method2 - l_method1);
            if (k >= k_value && absl <= l_value) {
                cv::Point2d point_temporary;
                point_temporary.x = j + g_N * l * cos(thetaTmp) / 2;
                point_temporary.y = i + g_N * l * sin(thetaTmp) / 2;
                points.push_back(point_temporary);
            } else {
                continue;
            }
        }
    }
    return points;
}


cv::Mat binaryThresholdByPoint(std::vector<cv::Point2d> const &points, int rows, int cols) {
    cv::Mat image(rows, cols, CV_8UC1);
    for (auto [x, y] : points) {
        image.at<unsigned char>(y, x) = 255;
    }
    return image;
}


double computeX(cv::Mat const &image,
                int closingSize,
                int gaussSize,
                double threshold,
                double maxAera,
                double minAera) {
    cv::Mat img;
    cv::cvtColor(image, img, cv::COLOR_BGR2GRAY);

    cv::Mat tmp;
    cv::equalizeHist(img, tmp);
    img = tmp;

    cv::threshold(img, img, threshold, 255, cv::THRESH_BINARY);
    // img = binaryThresholdByPoint(img);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size{closingSize, closingSize});
    cv::morphologyEx(img, img, cv::MORPH_CLOSE, kernel);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(img, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    for (auto &contour : contours) {
        double area = cv::contourArea(contour);
        if (minAera <= area && area <= maxAera) {
            cv::RotatedRect rect = cv::minAreaRect(contour);
            auto [center, size, angle] = rect;
            auto [x, y] = center;
            return x;
        }
    }
    return NaN;
}


/**
 * 相位相关法
 */
int phaseCorrelation(cv::Mat const &image1, cv::Mat const &image2) {
    // 通过 getOptimalDFTSize 创建两个频域变换后的图片(fft1 和 fft2)
    int width = cv::getOptimalDFTSize(std::max(image1.cols, image2.cols));
    int height = cv::getOptimalDFTSize(std::max(image1.rows, image2.rows));
    cv::Mat fft1(cv::Size(width, height), CV_32F, cv::Scalar(0));
    cv::Mat fft2(cv::Size(width, height), CV_32F, cv::Scalar(0));

    // 根据原图像复制
    for (int j = 0; j < image1.rows; j++) {
        for (int i = 0; i < image1.cols; i++) {
            fft1.at<float>(j, i) = image1.at<unsigned char>(j, i);
        }
    }
    for (int j = 0; j < image2.rows; j++) {
        for (int i = 0; i < image2.cols; i++) {
            fft2.at<float>(j, i) = image2.at<unsigned char>(j, i);
        }
    }

    // 傅里叶计算
    cv::dft(fft1, fft1, 0, image1.rows);
    cv::dft(fft2, fft2, 0, image2.rows);
    cv::mulSpectrums(fft1, fft2, fft1, 0, true);
    cv::idft(fft1, fft1);
    double maxVal;
    cv::Point maxLoc;
    cv::minMaxLoc(fft1, NULL, &maxVal, NULL, &maxLoc);
    int resX = (maxLoc.x < width / 2) ? (maxLoc.x) : (maxLoc.x - width);
    // int resY = (maxLoc.y < height / 2) ? (maxLoc.y) : (maxLoc.y - height);
    return resX;
}
