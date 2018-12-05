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

void lab4()
{
    vector<string> images(4);
    images = getFilesLab4();

    custom_DFT( images.at(3) );
//    waitKey(0);
}

vector<string> getFilesLab4()
{
    vector<string> images(4);

    images.at(0) = "../4/ig_0.jpg";
    images.at(1) = "../4/ig_1.jpg";
    images.at(2) = "../4/ig_2.jpg";
    images.at(3) = "../ig_.jpg";

    return images;
}

Mat count_W(int signalSize, bool inverse)
{
    int angleSign = inverse ? 1 : -1;

    float angle = 2 * angleSign * CV_PI / signalSize;

    // [0] - Real
    // [1] - Imaginary
    Mat W(signalSize, signalSize, CV_32FC2);

    // If k or n are zero then:
    // cos(0) = 1
    // sin(0) = 0
    for (int i = 0; i < signalSize; i++)
    {
        // Fill first column
        W.at<Vec2f>(i,0)[0] = 1.0;
        W.at<Vec2f>(i,0)[1] = 0.0;
        // Fill first row
        W.at<Vec2f>(0,i)[0] = 1.0;
        W.at<Vec2f>(0,i)[1] = 0.0;
    }

    // C_[k] = C*C_[k-1] - S*S_[k-1]
    // S_[k] = S*C_[k-1] + C*S_[k-1]
    // Iterative method for counting sin and cos
    float cos_ = cos(angle);
    float sin_ = sin(angle);

    float cos_k_1 = 1.0;
    float sin_k_1 = 0.0;
    for (int k = 1; k < signalSize; k++)
    {
        float cos_k = cos_ * cos_k_1 - sin_ * sin_k_1;
        float sin_k = sin_ * cos_k_1 + cos_ * sin_k_1;

        float cos_n_1 = 1.0;
        float sin_n_1 = 0.0;
        for (int n = 1; n < signalSize; n++)
        {
            float cos_n = cos_k * cos_n_1 - sin_k * sin_n_1;
            float sin_n = sin_k * cos_n_1 + cos_k * sin_n_1;

            W.at<Vec2f>(k, n)[0] = cos_n;
            W.at<Vec2f>(k, n)[1] = sin_n;

            cos_n_1 = cos_n;
            sin_n_1 = sin_n;
        }
        cos_k_1 = cos_k;
        sin_k_1 = sin_k;
    }
    return W;
}

Mat count_DFT_first_sum(const Mat img, const Mat W)
{
    const int numOfRows = img.rows;
    const int numOfCols = img.cols;

    Mat transformedImage(numOfRows, numOfCols, CV_32FC2);
    // Count first sum
    for (int n1 = 0; n1 < numOfRows; n1++)
    {
        for (int k2 = 0; k2 < numOfCols; k2++)
        {
            complex<float> sum2 (0.0f, 0.0f);
            for (int n2 = 0; n2 < numOfCols; n2++)
            {
                sum2 += (float)img.at<uchar>(n1, n2) *
                        complex<float>(W.at<Vec2f>(k2,n2)[0], W.at<Vec2f>(k2,n2)[1]);
            }
            /// Return matrix with complex numbers
            transformedImage.at<Vec2f>(n1, k2)[0] = sum2.real();
            transformedImage.at<Vec2f>(n1, k2)[1] = sum2.imag();
        }
    }
    // Count second sum
    for (int n2 = 0; n2 < numOfCols; n2++)
    {
        for (int k1 = 0; k1 < numOfRows; k1++)
        {
            complex<float> sum1 (0.0f, 0.0f);
            for (int n1 = 0; n1 < numOfRows; n1++)
            {
                /// Multiply first sum and W (with rules of complex numbers)
                sum1 += complex<float>(transformedImage.at<Vec2f>(n1, n2)[0], transformedImage.at<Vec2f>(n1, n2)[0] ) *
                        complex<float>(W.at<Vec2f>(k1,n1)[0], W.at<Vec2f>(k1,n1)[1] );
            }
            /// Return matrix with complex numbers
            transformedImage.at<Vec2f>(k1, n2)[0] = sum1.real();
            transformedImage.at<Vec2f>(k1, n2)[1] = sum1.imag();
        }
    }
    return transformedImage;
}

Mat count_DFT_second_sum(const Mat img, const Mat W)
{
    const int numOfRows = img.rows;
    const int numOfCols = img.cols;

    Mat transformedImage(numOfRows, numOfCols, CV_32FC2);

    for (int n2 = 0; n2 < numOfCols; n2++)
    {
        for (int k1 = 0; k1 < numOfRows; k1++)
        {
            complex<float> sum1 (0.0f, 0.0f);
            for (int n1 = 0; n1 < numOfRows; n1++)
            {
                /// Multiply first sum and W (with rules of complex numbers)
                sum1 += complex<float>(img.at<Vec2f>(n1, n2)[0], img.at<Vec2f>(n1, n2)[0] ) *
                        complex<float>(W.at<Vec2f>(k1,n1)[0], W.at<Vec2f>(k1,n1)[1] );
            }
            /// Return matrix with complex numbers
            transformedImage.at<Vec2f>(k1, n2)[0] = sum1.real();
            transformedImage.at<Vec2f>(k1, n2)[1] = sum1.imag();
        }
    }
    return transformedImage;
}

void custom_DFT(const string img_name)
{
    // Need to expand to optimum size
    Mat img = imread(img_name, CV_LOAD_IMAGE_GRAYSCALE);
    if (!img.empty() )
    {
        const int numOfCols = img.cols;
        const int numOfRows = img.rows;

        // Set timer
        TickMeter matrix_timer;

        // Get matrix with all coefficients
        int signalSize = (numOfCols < numOfRows) ? numOfRows : numOfCols;
        Mat W(signalSize, signalSize, CV_32FC2);
        matrix_timer.reset();
        matrix_timer.start();
        W = count_W(signalSize);
        matrix_timer.stop();
        cout << "W time: " << matrix_timer.getTimeSec() << "sec" << endl;

        // New matrix for DFT transformed image
        Mat fourier_sums(numOfRows, numOfCols, CV_32FC2);

        matrix_timer.reset();
        matrix_timer.start();
        fourier_sums = count_DFT_first_sum(img, W);
        matrix_timer.stop();
        cout << "First sum time: " << matrix_timer.getTimeSec() << "sec" << endl;

//        matrix_timer.reset();
//        matrix_timer.start();
//        fourier_sums = count_DFT_second_sum(fourier_sums, W);
//        matrix_timer.stop();
//        cout << "Second sum time: " << matrix_timer.getTimeSec() << "sec" << endl;

        // Split sums on real and imag parts
        Mat fourier_split[2] = {Mat(img.rows, img.cols, CV_32FC1), Mat(img.rows, img.cols, CV_32FC1)};
        split(fourier_sums, fourier_split);
        // Get magnitude of DFT
        Mat img_fourier;
        magnitude(fourier_split[0], fourier_split[1], img_fourier);
        // Switch to logarithmic scale
        img_fourier += Scalar::all(1);
        log(img_fourier, img_fourier);
        // Normalize spectrum
        normalize(img_fourier, img_fourier, 0, 1, NORM_MINMAX);
//    int m = getOptimalDFTSize(img.rows);
//    int n = getOptimalDFTSize(img.cols);
//    Mat imgConverted(m, n, CV_32FC1, Scalar::all(0));// = img.clone();
        imshow("img_at_finish", img);
        imshow("My_fourier", img_fourier);
        waitKey(0);
    }
    else
    {
        cout << "custom_DFT() : Failed to load image !" << endl;
    }
}
