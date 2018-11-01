//
// Created by Vadim Trishin
//

#ifndef LAB3_LAB2_H
#define LAB3_LAB2_H
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

void lab2();
void custom_blur(cv::Mat orig, cv::Mat res);
float compare_blurs(cv::Mat compare1, cv::Mat compare2);

#endif //LAB3_LAB2_H
