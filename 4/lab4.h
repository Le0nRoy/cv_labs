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

/**
 * @brief - Loads files with images
 * @return - vector with filenames of images
 */
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
 * @brief Discrete fouurier transformation
 * also makes OpenCV DFT for comparing
 * @param image - image to transform
 * @return - Fourier transformed image
 */
cv::Mat custom_DFT(const std::string img_name);
/**
 * @brief - normalize spectrum of image and make window with this image
 * @param fourier_sums - matrix of imaginary values of spectrum
 * @param name - additional string for window name
 * win name = name + "dft"
 * @return - normalized spectrum
 */
cv::Mat normalize_fourier(cv::Mat fourier_sums, std::string name);
/**
 * @brief - filter image with one of available filters
 * uses Fourier transformation
 * @param img_name - path to image to be processed
 * @param kernel_type - type of 3x3 kernel of filter
 * 0 - horizontal Sobel
 * 1 - vertical Sobel
 * 2 - BoxFilter
 * 3 - Laplas
 */
void dftConvolution(std::string img_name, int kernel_type);
/**
 * @brief - cut low or high frequences
 * @param img_name - path to image to be processed
 * @param high - if true, cut high frequences
 */
void cutFreq(std::string img_name, bool high);

void correlation(cv::Mat img1, cv::Mat img2, std::string str);
/**
 *
 */
void numb_correlation();

void shift_rect(cv::Mat& fourier_img);
#endif //LAB3_LAB4_H
