//
// Created by bledgharm on 05.11.18.
//

#ifndef LAB3_LAB4_H
#define LAB3_LAB4_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <string>

std::vector<std::string> getFilesLab4();

void lab4();
cv::Mat count_W(int signalSize, bool inverse = false);
void custom_DFT(const std::string image);
cv::Mat count_DFT_first_sum(const cv::Mat img, const cv::Mat W);
cv::Mat count_DFT_second_sum(const cv::Mat img, const cv::Mat W);

#endif //LAB3_LAB4_H
