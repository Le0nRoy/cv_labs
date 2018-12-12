//
// Created by Vadim Trishin
//

#ifndef LAB3_LAB2_H
#define LAB3_LAB2_H
#include <opencv2/core.hpp>

void lab2();
void lab_box_filter(const cv::Mat orig, cv::Mat res, cv::Size filter_size = cv::Size(3, 3));
float compare_blurs(cv::Mat compare1, cv::Mat compare2);

#endif //LAB3_LAB2_H
