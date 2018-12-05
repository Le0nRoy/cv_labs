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

/// *****************************************************
/// Loads files with images.
/// *****************************************************
std::vector<std::string> getFilesLab4();

void lab4();
/**
 * @brief - Counts coefficient W for fourier transformation
 * @param signalSize - number of cols of image
 * @param inverse - if true, than counts coefficients for inverse fourier transformation
 * @return - Mat with all counted coefficients
 *      [0] - Real part
 *      [1] - Imaginary part
 */
cv::Mat count_W(int signalSize, bool inverse = false);
/**
 * @brief - counts first (for all n2 and k2 elements) sum of DFT
 * @param img - image for which to count sum
 * @param W - coefficients
 * @return - matrix with complex numbers
 */
cv::Mat count_DFT_first_sum(const cv::Mat img, const cv::Mat W);
/**
 * @brief - counts second (for all n1 and k1 elements) sum of DFT
 * @param img - first sum of DFT (CV_32FC2)
 * @param W - coefficients
 * @return - matrix with complex numbers (transformed by DFT )
 */
cv::Mat count_DFT_second_sum(const cv::Mat img, const cv::Mat W);
/**
 * @brief Discrete fouurier transformation (need vodka to understand)
 * (make with InputArray and OutputArray)
 * (make threads)
 * @param image - image to transform
 */
void custom_DFT(const std::string img_name);

#endif //LAB3_LAB4_H
