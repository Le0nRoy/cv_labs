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

    int control = getchar();
    while (control != '0')
    {
        switch (control)
        {
            case '1':
            {
                custom_DFT(images.at(3));
                destroyAllWindows();
                break;
            }
            case '2':
            {
                dftConvolution(images.at(3), 0);
                dftConvolution(images.at(3), 1);
                dftConvolution(images.at(3), 2);
                dftConvolution(images.at(3), 3);
                destroyAllWindows();
                break;
            }
            case '3':
            {
                cutFreq(images.at(3), false);
                cutFreq(images.at(3), true);
                destroyAllWindows();
                break;
            }
        }
        control = getchar();
    }
}

/// Good
vector<string> getFilesLab4()
{
    vector<string> images(4);

    images.at(0) = "../4/ig_0.jpg";
    images.at(1) = "../4/ig_1.jpg";
    images.at(2) = "../4/ig_2.jpg";
    images.at(3) = "../ig_.jpg";

    return images;
}

/// Good
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

/// Good
Mat count_DFT_first_sum(const Mat img, const Mat W)
{
    if (img.empty() )
    {
        cout << "count_DFT_first_sum() : Image is empty !" << endl;
        return img;
    }
    const int numOfRows = img.rows;
    const int numOfCols = img.cols;

    Mat transformedImage(numOfRows, numOfCols, CV_32FC2);

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
    return transformedImage;
}

/// Good
Mat count_DFT_second_sum(const Mat img, const Mat W)
{
    if (img.empty() )
    {
        cout << "count_DFT_second_sum() : Image is empty !" << endl;
        return img;
    }
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

/// Good
Mat custom_DFT(const string img_name)
{
    cout << "You've entered custom_DFT(). Good luck!" << endl;

    Mat img = imread(img_name, CV_LOAD_IMAGE_GRAYSCALE);
    if (img.empty() )
    {
        cout << "custom_DFT() : Failed to load image !" << endl;
        return img;
    }
    const int numOfCols = img.cols;
    const int numOfRows = img.rows;
    /// Set timer
    TickMeter matrix_timer;
    /// Get matrix with all coefficients
    int signalSize = (numOfCols < numOfRows) ? numOfRows : numOfCols;
    Mat W(signalSize, signalSize, CV_32FC2);
    W = count_W(signalSize);
    /// New matrix for DFT sums
    Mat fourier_sums(numOfRows, numOfCols, CV_32FC2);
    /// First sum of DFT
    matrix_timer.reset();
    matrix_timer.start();
    fourier_sums = count_DFT_first_sum(img, W);
    matrix_timer.stop();
    cout << "First sum time: " << matrix_timer.getTimeSec() << "sec" << endl;
    /// Second sum of DFT
    matrix_timer.reset();
    matrix_timer.start();
    fourier_sums = count_DFT_second_sum(fourier_sums, W);
    matrix_timer.stop();
    cout << "Second sum time: " << matrix_timer.getTimeSec() << "sec" << endl;
    /// Get image with normalized spectrum
    Mat img_fourier;
    img_fourier = normalize_fourier(fourier_sums, "My");
    ///** Get lib Fourier
    /// DFT sum
    Mat fourier_lib;
    img.convertTo(fourier_lib, CV_32FC2);
    /// DFT image
    Mat img_fourier_lib;
    img.convertTo(img_fourier_lib, CV_32FC1);
    /// DFT transformation
    matrix_timer.reset();
    matrix_timer.start();
    dft(img_fourier_lib, fourier_lib, DFT_COMPLEX_OUTPUT);
    matrix_timer.stop();
    cout << "Lib sum time: " << matrix_timer.getTimeSec() << "sec" << endl;
    img_fourier_lib = normalize_fourier(fourier_lib, "lib");

    imshow("original_image", img);
    waitKey(0);
    return img_fourier;
}

/// Good
Mat normalize_fourier(Mat fourier_sums, string name)
{
    /// Split sums on real and imag parts
    Mat fourier_split[2] = {Mat(fourier_sums.rows, fourier_sums.cols, CV_32FC1),
                            Mat(fourier_sums.rows, fourier_sums.cols, CV_32FC1)};
    split(fourier_sums, fourier_split);
    /// Get magnitude of DFT
    Mat img_fourier;
    magnitude(fourier_split[0], fourier_split[1], img_fourier);
    /// Switch to logarithmic scale
    img_fourier += Scalar::all(1);
    log(img_fourier, img_fourier);
    /// Normalize spectrum
    normalize(img_fourier, img_fourier, 0, 1, NORM_MINMAX);

    imshow(name + "dft", img_fourier);
    return img_fourier;
}

/// Good
void dftConvolution(string img_name, int kernel_type)
{
    Mat img = imread(img_name, CV_LOAD_IMAGE_GRAYSCALE);
    imshow("original", img);

    Mat dft_img(img.size(), CV_32FC2);
    img.convertTo(img, CV_32FC1);
    dft(img, dft_img, DFT_COMPLEX_OUTPUT);
    normalize_fourier(dft_img, "CV_DFT");
    /// Prepare kernel for convolution
    int kernel[3][3];
    switch (kernel_type)
    {
        case 0:
        {
            int sobel_hor[][3] = { -1, -2, -1,
                                     0, 0, 0,
                                     1, 2, 1 };
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    kernel[i][j] = sobel_hor[i][j];
                }
            }
            break;
        }
        case 1:
        {
            int sobel_ver[][3] = { -1, 0, 1,
                                     -2, 0, 2,
                                     -1, 0, 1 };
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    kernel[i][j] = sobel_ver[i][j];
                }
            }
            break;
        }
        case 2:
        {
            int laplase[][3] = { 0, 1, 0,
                                   1, -4, 1,
                                   0, 1, 0 };
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    kernel[i][j] = laplase[i][j];
                }
            }
            break;
        }
        case 3:
        {
            int boxfilter[][3] = { 1, 1, 1,
                                   1, 1, 1,
                                   1, 1, 1 };
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    kernel[i][j] = boxfilter[i][j];
                }
            }
            break;
        }
        default:
        {
            cout << "dftConvolution() : Wrong kernel type !" << endl;
            break;
        }
    }

    Mat mat_kernel(img.size(), CV_32FC1, Scalar(0));
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            mat_kernel.at<float>(i, j) = (float)kernel[i][j];
        }
    }

    Mat dft_kernel(img.size(), CV_32FC2);
    dft(mat_kernel, dft_kernel, DFT_COMPLEX_OUTPUT);
    normalize_fourier(dft_kernel, "CV_DFT_kernel");

    Mat result(img.size(), CV_32FC2);
    mulSpectrums(dft_kernel, dft_img, result, 0, 0);
    normalize_fourier(result, "mulSpectrums");

    Mat idft_img(img.size(), CV_32FC1);
    dft(result, idft_img, DFT_INVERSE | DFT_REAL_OUTPUT);
    normalize(idft_img, idft_img, 0.0, 255, CV_MINMAX);
    idft_img.convertTo(idft_img, CV_8UC1);
    imshow("result", idft_img);
    waitKey(0);
}

/// Good
void cutFreq(string img_name, bool high)
{
    Mat img = imread(img_name, CV_LOAD_IMAGE_GRAYSCALE);
    imshow("original", img);

    int zone = high ? 1 : 0;
    Mat img_dft(img.size(), CV_32FC2);
    img.convertTo(img, CV_32FC1);
    dft(img, img_dft, DFT_COMPLEX_OUTPUT);
    normalize_fourier(img_dft, "CV DFT");

    Mat mat_kernel(img.size(), CV_32FC2, Scalar(zone, 0));
    int cut_radius = img_dft.cols > img_dft.rows ? img_dft.cols / 2 : img_dft.rows / 2;
    cut_radius -= 40;
    circle(mat_kernel, Point(img.rows / 2, img.cols / 2), cut_radius, Scalar(1 - zone, 0), -1);

    Mat res;
    mulSpectrums(img_dft, mat_kernel, res, 0);
    normalize_fourier(res, "res");

    Mat idft_img(img.size(), CV_32FC1);
    dft(res, idft_img, DFT_INVERSE | DFT_REAL_OUTPUT);
    normalize(idft_img, idft_img, 0.0, 255, CV_MINMAX);
    idft_img.convertTo(idft_img, CV_8UC1);
    imshow("idft_res", idft_img);
    waitKey(0);
}

///
void numb_correlation()
{
    Mat img = imread("table.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    imshow("original", img);

    Mat A = imread("A.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    imshow("A", A);

    Mat seven = imread("seven.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    imshow("seven", seven);

    Mat zero = imread("zero.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    imshow("zero", zero);

    correlation(img.clone(), A.clone(), "A");
    correlation(img.clone(), seven.clone(), "seven");
    correlation(img.clone(), zero.clone(), "zero");
}

void correlation(Mat img, Mat ch, string str)
{
    Mat dft_img(img.size(), CV_32FC2);
    img.convertTo(img, CV_32FC1);
    Mat mean_img, std_img;
    meanStdDev(img, mean_img, std_img);
    img -= mean_img;
    img /= std_img;

    dft(img, dft_img, DFT_COMPLEX_OUTPUT);
    normalize_fourier(dft_img, "_img");

    ch.convertTo(ch, CV_32FC1);
    Mat mean_ch, std_ch;
    meanStdDev(ch, mean_ch, std_ch);
    ch -= mean_ch;
    ch /= std_ch;

    Mat sign(img.size(), CV_32FC1, Scalar(0));
    Mat roi(sign, Rect(0, 0, ch.cols, ch.rows));
    ch.copyTo(roi);

    imshow("sign" + str, sign);

    Mat dft_sign(sign.size(), CV_32FC2);
    sign.convertTo(sign, CV_32FC1);
    dft(sign, dft_sign, DFT_COMPLEX_OUTPUT);
    normalize_fourier(dft_sign, "_" + str);

    Mat img_sign;
    mulSpectrums(dft_img, dft_sign, img_sign, 0, true);
    normalize_fourier(img_sign, "_img_" + str);

    Mat idft_img(img.size(), CV_32FC1);
    dft(img_sign, idft_img, DFT_INVERSE | DFT_REAL_OUTPUT);
    normalize(idft_img, idft_img, 0.0, 255, CV_MINMAX);
    idft_img.convertTo(idft_img, CV_8UC1);
    imshow("idft_res" + str, idft_img);

    double minVal;
    double maxVal;
    minMaxLoc(idft_img, &minVal, &maxVal);

    Mat idft_img_bin;
    threshold(idft_img, idft_img_bin, (maxVal-5), 255, THRESH_BINARY_INV);
    imshow("res_" + str, idft_img_bin);
}
