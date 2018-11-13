//
// Created by bledgharm on 05.11.18.
//

#include "lab4.h"

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <thread>

using namespace std;
using namespace cv;

const float pi = 3.141592;

void lab4()
{
    vector<string> images(3);
    images = getFilesLab4();
//    Mat img = imread(images.at(0), CV_LOAD_IMAGE_COLOR);
    custom_DFT( images.at(0) );
//    waitKey(0);
}

/// *****************************************************
/// Loads files with images.
/// *****************************************************
vector<string> getFilesLab4()
{
    vector<string> images(3); // 12 pictures for lab3, but allocate more memory as stock

    string wayToFolder = "/home/bledgharm/CV_labs/labs/4/"; // Way from home to folder with folders that contain images

    /// allababah
    images.at(0)  = wayToFolder+"ig_0.jpg";
    images.at(1)  = wayToFolder+"ig_1.jpg";

    return images;
}

/**
 * @brief - Counts coefficient W for direct fourier transformation
 * (make Mat with 2 channels with W for all n1, n2, k1 and k2)
 * @param signalSize - size of signal (for 2D image number of cols or rows, depends on what for coefficient)
 * @param elemNum - number of element for which coefficient counted
 * @param inverse - if true, than counts coefficients for inverse fourier transformation
 * @return - vector with all counted coefficients
 *      .at(0) - Real part
 *      .at(1) - Imaginary part
 */
vector<vector<float>> countW(int signalSize, int elemNum, bool inverse = false)
{
    int angleSign = -1;
    if (inverse)
    {
        angleSign = 1;
    }
    const float angle = 2 * angleSign * elemNum * pi / signalSize;

    float currentAngle = 0;
    vector<float> RealW(signalSize);
    vector<float> ImageW(signalSize);

    for (int n = 0; n <= signalSize - 1; n++)
    {
        currentAngle = angle * n;
        RealW.at(n) = cos(currentAngle);
        ImageW.at(n) = sin(currentAngle);
    }
    vector<vector<float>> W(2);
    W.at(0) = RealW;
    W.at(1) = ImageW;
    return W;
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
    Mat img = imread(imgname.data(), CV_LOAD_IMAGE_GRAYSCALE);
    Mat img1 = img.clone();
    cout << endl << "img.channels() : " << img.channels() << endl;
    int numOfCols = img.cols;
    int numOfRows = img.rows;

//    Mat img1(numOfRows, numOfCols, CV_8UC1);
//    img.copyTo(img1);
//    imshow("img1", img);
    waitKey(0);

    TickMeter msec_timer;

    vector<vector<float>> W1(numOfCols);
    vector<vector<float>> W2(numOfRows);

    // .at(0) - reals, .at(1) - imaginary
    Mat transformedImg(numOfRows, numOfCols, CV_8UC2);
    cout << "transformedImg.channels() : " << transformedImg.channels() << endl;

    msec_timer.reset();
    msec_timer.start();
    for (int newCol = 0; newCol <= numOfCols - 1; newCol++)
    {
        W1 = countW(numOfCols, newCol);
        for (int newRow = 0; newRow <= numOfRows - 1; newRow++)
        {
            W2 = countW(numOfRows, newRow);
            // Formula for ONE element
            int sumRe1 = 0;
            int sumIm1 = 0;
            for (int defaultCol = 0; defaultCol <= numOfCols - 1; defaultCol++)
            {
                float sumRe2 = 0;
                float sumIm2 = 0;

                for (int defaultRow = 0; defaultRow <= numOfRows - 1; defaultRow++)
                {
                    // take values of channel as x
                    // First sum of real parts for element (newCol, newRow)
                    sumRe2 += img.at<float>(defaultCol, defaultRow) * W2.at(0).at(defaultRow);
                    // First sum of imaginary parts for element (newCol, newRow)
                    sumIm2 += img.at<float>(defaultCol, defaultRow) * W2.at(1).at(defaultRow);
                }

                sumRe1 += sumRe2 * W1.at(0).at(defaultCol); // Final sum of real parts for element (newCol, newRow)
                sumIm1 += sumIm2 * W1.at(1).at(defaultCol); // Final sum of imaginary parts for element (newCol, newRow)
            }
            // Need to write element to Mat somehow
            transformedImg.at<Vec2b>(newRow, newCol)[0] = sumRe1;
            transformedImg.at<Vec2b>(newRow, newCol)[1] = sumIm1;
            img1.at<float>(newCol, newRow) = sqrt(sumRe1*sumRe1 + sumIm1*sumIm1);
        }
    }
    msec_timer.stop();
    std::cout << "OpenCV time = " << msec_timer.getTimeMilli() << " msec" << std::endl;
    imshow("original", img);
    imshow("fourier", img1);
    waitKey(0);
//    thread countW2(); // Need to make countW() with OutputArray
}