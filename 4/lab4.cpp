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
 * @brief - Counts coefficient W2 for fourier transformation
 * @param cols - number of cols of image
 * @param inverse - if true, than counts coefficients for inverse fourier transformation
 * @return - Mat with all counted coefficients
 *      [0] - Real part
 *      [1] - Imaginary part
 */
cv::Mat countW2(int cols, bool inverse)
{
//    readOrWriteW2.lock();
    // Stream for storing W-matrices in files
//    fstream f;
    // File creation
//    ofstream fi("../4/directW2.txt");
//    fi.close();

    int angleSign = -1;
    if (inverse)
    {
        angleSign = 1;
    }

    double angle = 2 * angleSign * pi / cols;
    double currentAngle = 0;

    // [0] - Real
    // [1] - Imaginary
    Mat W(cols, cols, CV_64FC2);

    // Angle of (0,0) element is always 0
    // cos(0)
    W.at<Vec2d>(0,0)[0] = 1.0;
    // sin(0)
    W.at<Vec2d>(0,0)[1] = 0.0;

//    f.open("../4/directW2.txt", fstream::in);
//    if(!f)
//    {
//        cout << "error!";
//        return;
//    }

//    int co = 0;
//    int si = 0;
    for (int k = 1; k < cols; k++)
    {
        currentAngle = angle * k;
        for (int n = 1; n < cols; n++)
        {
//            co = cos(currentAngle);
//            si = sin(currentAngle);
//            f << co << " " << si << ", ";
            // Change on pointers
            W.at<Vec2d>(k, n)[0] = cos(currentAngle);
            W.at<Vec2d>(k, n)[1] = sin(currentAngle);
            // Same as currentAngle = angle * k * n
            currentAngle++;
        }
//        f << endl;
    }
//    &output = W;
//    f.close();
//    readOrWriteW2.unlock();
    return W;
}

/**
 * @brief - Counts coefficient W1 for fourier transformation
 * @param cols - number of rows of image
 * @param inverse - if true, than counts coefficients for inverse fourier transformation
 * @return - Mat with all counted coefficients
 *      [0] - Real part
 *      [1] - Imaginary part
 */
cv::Mat countW1(int rows, bool inverse)
{
//    readOrWriteW1.lock();
    // Stream for storing W-matrices in files
//    fstream f;
    // File creation
    // Make another file for inverse
//    ofstream fi("../4/directW1.txt");
//    fi.close();

    int angleSign = -1;
    if (inverse)
    {
        angleSign = 1;
    }

    double angle = 2 * angleSign * pi / rows;
    double currentAngle = 0;

    // [0] - Real
    // [1] - Imaginary
    Mat W(rows, rows, CV_64FC2);

    // Angle of (0,0) element is always 0
    // cos(0)
    W.at<Vec2d>(0,0)[0] = 1.0;
    // sin(0)
    W.at<Vec2d>(0,0)[1] = 0.0;

//    f.open("../4/directW2.txt", fstream::in);
//    if(!f)
//    {
//        cout << "error!";
//        return;
//    }

//    int co = 0;
//    int si = 0;
    for (int k = 1; k < rows; k++)
    {
        currentAngle = angle * k;
        for (int n = 1; n < rows; n++)
        {
//            co = cos(currentAngle);
//            si = sin(currentAngle);
//            f << co << " " << si << ", ";
            // Change on pointers
            W.at<Vec2d>(k, n)[0] = cos(currentAngle);
            W.at<Vec2d>(k, n)[1] = sin(currentAngle);
            // Same as currentAngle = angle * k * n
            currentAngle++;
        }
//        f << endl;
    }
//    *output = W;
//    f.close();
//    readOrWriteW1.unlock();
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
    imshow("ing_at_start", img);
    cout << endl << "img.channels() : " << img.channels() << endl;
    int numOfCols = img.cols;
    int numOfRows = img.rows;

    TickMeter msec_timer;

    thread countingW2;
    thread countingW1;
    Mat W2(numOfCols, numOfCols, CV_64FC2);
//    countingW2 = thread(countW2, numOfCols, &W2);
    Mat W1(numOfRows, numOfRows, CV_64FC2);
//    countingW1 = thread(countW2, numOfCols, &W2);

    // [0] - Real
    // [1] - Imaginary
    Mat transformedImg(numOfRows, numOfCols, CV_64FC2);
//    cout << "transformedImg.channels() : " << transformedImg.channels() << endl;

    // Divide on to cycles each for 1-dimension transformation
    for (int k1 = 0; k1 <= numOfRows - 1; k1++)
    {
        cout << "k1: " << k1 << endl << "total of rows: " << numOfRows << endl;
        msec_timer.reset();
        msec_timer.start();
        for (int k2 = 0; k2 <= numOfCols - 1; k2++)
        {
            // Formula for ONE element
            double sumRe1 = 0;
            double sumIm1 = 0;
            for (int n1 = 0; n1 <= numOfRows - 1; n1++)
            {
                double sumRe2 = 0;
                double sumIm2 = 0;

                for (int n2 = 0; n2 <= numOfCols - 1; n2++)
                {
                    // take values of channel as x
                    // First sum of real parts for element (k1, k2)
                    sumRe2 += img.at<double>(n1, n2) * W2.at<Vec2d>(k2,n2)[0];
                    // First sum of imaginary parts for element (k1, k2)
                    sumIm2 += img.at<double>(n1, n2) * W2.at<Vec2d>(k2,n2)[0];
                }

                sumRe1 += sumRe2 * W1.at<Vec2d>(k1,n1)[0]; // Final sum of real parts for element (k1, k2)
                sumIm1 += sumIm2 * W1.at<Vec2d>(k1,n1)[1]; // Final sum of imaginary parts for element (k1, k2)
            }
            // Need to write element to Mat somehow
            transformedImg.at<Vec2d>(k1, k2)[0] = sumRe1;
            transformedImg.at<Vec2d>(k1, k2)[1] = sumIm1;
            img1.at<double>(k1, k2) = sqrt(sumRe1*sumRe1 + sumIm1*sumIm1);
        }
        msec_timer.stop();
        std::cout << "Col time: " << msec_timer.getTimeMilli() << " msec" << std::endl;
    }
    imshow("img_at_finish", img);
    imshow("fourier", img1);
    waitKey(0);
//    thread countW2(); // Need to make countW2() with OutputArray
}