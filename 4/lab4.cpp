//
// Created by bledgharm on 05.11.18.
//

#include "lab4.h"

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <thread>
#include <mutex>
#include <fstream>

using namespace std;
using namespace cv;

const double pi = 3.14159265359;
//mutex readOrWriteW2;
//mutex readOrWriteW1;

void lab4()
{
    vector<string> images(3);
    images = getFilesLab4();

//    Mat img = imread(images.at(1), CV_LOAD_IMAGE_GRAYSCALE);
//    Mat imgConverted = img.clone();
//    Mat img1 = img.clone();
//    imshow("img", img);
//    img.convertTo(imgConverted, CV_32FC1);
//    dft(imgConverted, img1);
//    imshow("img1", img1);
//    waitKey(0);
//
    custom_DFT( images.at(1) );
//    waitKey(0);
}

/// *****************************************************
/// Loads files with images.
/// *****************************************************
vector<string> getFilesLab4()
{
    vector<string> images(3); // 12 pictures for lab3, but allocate more memory as stock

    string wayToFolder = "/home/bledgharm/CV_labs/labs/4/"; // Way from home to folder with folders that contain images

    images.at(0)  = wayToFolder+"ig_0.jpg";
    images.at(1)  = wayToFolder+"ig_1.jpg";

    return images;
}

/// Throw away return value and make function, that is reading from file
/**
 * @brief - Counts coefficient W for fourier transformation
 * @param signalSize - number of cols of image
 * @param inverse - if true, than counts coefficients for inverse fourier transformation
 * @return - Mat with all counted coefficients
 *      [0] - Real part
 *      [1] - Imaginary part
 */
Mat count_W(int signalSize, bool inverse)
{
    int angleSign = inverse ? 1 : -1;

    double angle = 2 * angleSign * pi / signalSize;

    // [0] - Real
    // [1] - Imaginary
    Mat W(signalSize, signalSize, CV_64FC2);

    // If k or n are zero then:
    // cos(0) = 1
    // sin(0) = 0
    for (int i = 0; i < signalSize; i++)
    {
        // Fill first column
        W.at<Vec2d>(i,0)[0] = 1.0;
        W.at<Vec2d>(i,0)[1] = 0.0;
        // Fill first row
        W.at<Vec2d>(0,i)[0] = 1.0;
        W.at<Vec2d>(0,i)[1] = 0.0;
    }

    //C_[k] = C*C_[k-1] - S*S_[k-1]
    //S_[k] = S*C_[k-1] + C*S_[k-1]
    double cos_ = cos(angle);
    double sin_ = sin(angle);

    double cos_k_1 = 1.0;
    double sin_k_1 = 0.0;
    for (int k = 1; k < signalSize; k++)
    {
        double cos_k = cos_ * cos_k_1 - sin_ * sin_k_1;
        double sin_k = sin_ * cos_k_1 + cos_ * sin_k_1;

        double cos_n_1 = 1.0;
        double sin_n_1 = 0.0;
        for (int n = 1; n < signalSize; n++)
        {
            double cos_n = cos_k * cos_n_1 - sin_k * sin_n_1;
            double sin_n = sin_k * cos_n_1 + cos_k * sin_n_1;

            W.at<Vec2d>(k, n)[0] = cos_n;
            W.at<Vec2d>(k, n)[1] = sin_n;

            cos_n_1 = cos_n;
            sin_n_1 = sin_n;
        }
        cos_k_1 = cos_k;
        sin_k_1 = sin_k;
    }
    return W;
}

/**
 * @brief - counts first (for all n2 and k2 elements) sum of DFT
 * @param img - image for which to count sum
 * @param W - coefficients
 * @return - new image
 */
Mat count_DFT_first_sum(const Mat img, const Mat W)
{
    const int numOfRows = img.rows;
    const int numOfCols = img.cols;

    Mat transformedImage(numOfRows, numOfCols, CV_64FC1);

    for (int n1 = 0; n1 < numOfRows; n1++)
    {
        for (int k = 0; k < numOfCols; k++)
        {
            double sumRe2 = 0;
            double sumIm2 = 0;
            double s = 0;
            for (int n2 = 0; n2 < numOfCols; n2++)
            {
                // First sum of real parts for element (k1, k2)
                sumRe2 += img.at<uint8_t>(n1, n2) * W.at<Vec2d>(k,n2)[0];
                // First sum of imaginary parts for element (k1, k2)
                sumIm2 += img.at<uint8_t>(n1, n2) * W.at<Vec2d>(k,n2)[1];
            }
            s = sqrt(sumRe2*sumRe2 + sumIm2*sumIm2);
            transformedImage.at<double>(n1, k) = sqrt(sumRe2*sumRe2 + sumIm2*sumIm2);
        }
    }
    return transformedImage;
}

/**
 * @brief - counts second (for all n1 and k1 elements) sum of DFT
 * @param img - image for which to count sum
 * @param W - coefficients
 * @return - new image
 */
Mat count_DFT_second_sum(const Mat img, const Mat W)
{
    const int numOfRows = img.rows;
    const int numOfCols = img.cols;

    Mat transformedImage(numOfRows, numOfCols, CV_64FC1);

    for (int n2 = 0; n2 < numOfCols; n2++)
    {
        for (int k = 0; k < numOfRows; k++)
        {
            double sumRe1 = 0;
            double sumIm1 = 0;
            for (int n1 = 0; n1 < numOfRows; n1++)
            {
                // First sum of real parts for element (k1, k2)
                sumRe1 += img.at<double>(n1, n2) * W.at<Vec2d>(k,n1)[0];
                // First sum of imaginary parts for element (k1, k2)
                sumIm1 += img.at<double>(n1, n2) * W.at<Vec2d>(k,n1)[1];
            }
            transformedImage.at<double>(k, n2) = sqrt(sumRe1*sumRe1 + sumIm1*sumIm1);
        }
    }
    return transformedImage;
}

/// *****************************************************
/// Discrete fouurier transformation (make with InputArray and OutputArray)
/// *****************************************************
/**
 * @brief Discrete fouurier transformation (need vodka to understand)
 * (make with InputArray and OutputArray)
 * (make threads)
 * @param image - image to transform
 */
void custom_DFT(const string imgname)
{
    // Need to expand to optimum size
    Mat img = imread(imgname.data(), CV_LOAD_IMAGE_GRAYSCALE);
    const int numOfCols = img.cols;
    const int numOfRows = img.rows;

    TickMeter rows_timer;
    TickMeter matrix_timer;

    int signalSize = (numOfCols < numOfRows) ? numOfRows : numOfCols;
    Mat W(signalSize, signalSize, CV_64FC2);
    W = count_W(signalSize);

    Mat img1(numOfRows, numOfCols, CV_64FC1);

    matrix_timer.reset();
    matrix_timer.start();
    img1 = count_DFT_first_sum(img, W);
    matrix_timer.stop();
    cout << "First sum time: " << matrix_timer.getTimeSec() << "sec" << endl;

    matrix_timer.reset();
    matrix_timer.start();
    img1 = count_DFT_second_sum(img1, W);
    matrix_timer.stop();
    cout << "Second sum time: " << matrix_timer.getTimeSec() << "sec" << endl;

    Mat imgConverted;// = img.clone();
    img.convertTo(imgConverted, CV_64FC1);
    dft(imgConverted, imgConverted);
    imshow("img_at_finish", img);
    imshow("OpenCV_fourier", imgConverted);
    imshow("My_fourier", img1);
    waitKey(0);
//    thread countW2(); // Need to make count_W() with OutputArray
}
