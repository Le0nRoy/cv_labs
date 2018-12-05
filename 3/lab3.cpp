#include "lab3.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void lab3()
{
    /// Load filenames of images
    vector<string> images = getFilesLab3();
    /// Filename of image for function checking
    string imgForFunc;
    /// First part (houses)
//    imgForFunc = images.at(0);
//    imgForFunc = images.at(1);
//    imgForFunc = images.at(2);
//    findHouse(imgForFunc);

    /// Second part (engines)
    /// may be can do smth with 9 and 11
//    imgForFunc = images.at(7);
//    imgForFunc = images.at(8);
//    imgForFunc = images.at(9);
//    imgForFunc = images.at(10);
//    imgForFunc = images.at(11);
//    findEngine(imgForFunc);

    /// Third part (robots, lamp)
//    imgForFunc = images.at(5);
    imgForFunc = images.at(6);
    findRobots(imgForFunc);

    /// Fourth part (wrench)
//    imgForFunc = images.at(3);
//    string wrenchTemplate = images.at(4);
//    findWrench(imgForFunc, wrenchTemplate);

    destroyAllWindows();
}

/// Good
vector<string> getFilesLab3()
{
    vector<string> images(15); // 12 pictures for lab3, but allocate more memory as stock

    string wayToFolder = "../3/Tasks/img_zadan"; // Way from home to folder with folders that contain images

    /// allababah
    images.at(0)  = wayToFolder+"/allababah/ig_0.jpg";
    images.at(1)  = wayToFolder+"/allababah/ig_1.jpg";
    images.at(2)  = wayToFolder+"/allababah/ig_2.jpg";

    /// gk
    images.at(3)  = wayToFolder+"/gk/gk.jpg";
    images.at(4)  = wayToFolder+"/gk/gk_tmplt.jpg";

    /// roboti
    images.at(5)  = wayToFolder+"/roboti/roi_robotov.jpg";
    images.at(6)  = wayToFolder+"/roboti/roi_robotov_1.jpg";

    /// teplovizor
    images.at(7)  = wayToFolder+"/teplovizor/21331.res.jpg";
    images.at(8)  = wayToFolder+"/teplovizor/ntcs_quest_measurement.png";
    images.at(9)  = wayToFolder+"/teplovizor/445923main_STS128_16FrAvg_color1.jpg";
    images.at(10) = wayToFolder+"/teplovizor/size0-army.mil-2008-08-28-082221.jpg";
    images.at(11) = wayToFolder+"/teplovizor/MW-AW129-measured.jpg";

    return images;
}

/// Good
void findHouse(string image)
{
    Mat img = imread(image, CV_LOAD_IMAGE_GRAYSCALE);
    if (img.empty())
    {
        cout << "findHouse() : Failed to load image" << endl;
        return;
    }
    /// All works with copy of image
    Mat imgGRAY;
    img.copyTo(imgGRAY);
    /// Set to binary mode
    threshold(imgGRAY, imgGRAY, 220, 255, THRESH_BINARY);
    /// Use morphology to throw away noises
    Mat kernel = getStructuringElement(CV_SHAPE_RECT, Size(3, 3));
	erode(imgGRAY, imgGRAY, kernel, Point(-1, -1), 1);
    dilate(imgGRAY, imgGRAY, kernel, Point(-1,-1), 4);
    /// Find contours of objects
    vector<vector<Point>> contours;
    findContours(imgGRAY, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    /// Higlight objects by it's contour
    img.copyTo(imgGRAY);
    cvtColor(imgGRAY, imgGRAY, CV_GRAY2BGR);
    polylines(imgGRAY, contours, true, Vec3b(0, 0, 255), 2, 8);
    /// Find mass center of each house
    Moments house_moments;
    for (int i = 0; i < contours.size(); i++)
    {
        /// Check if contour length is not too low
        if (contours.at(i).size() > 10)
        {
            house_moments = moments(contours.at(i) );
            int x = house_moments.m10 / house_moments.m00;
            int y = house_moments.m01 / house_moments.m00;
            circle(imgGRAY, Point(x, y), 5, Vec3b(0, 0, 255), -1);
        }
    }

    imshow("lab3.1_transformed", imgGRAY);
    waitKey(0);
}

/// Good
void findEngine(string image)
{
    Mat img = imread(image, CV_LOAD_IMAGE_COLOR);
    if (img.empty())
    {
        cout << "findEngine() : Failed to load image" << endl;
        return;
    }
    /// All works with copy of image
    // Change to imgHSV
    Mat img_copy;
    img.copyTo(img_copy);
    /// Convert to HSV
    cvtColor(img, img_copy, CV_BGR2HSV);
    cvtColor(img, img, CV_BGR2HSV);
    /// Find objects
    inRange(img_copy, Vec3b(0, 0, 0), Vec3b(35, 255, 255), img_copy);
    /// Use morphology to throw away noises
    Mat kernel = getStructuringElement(CV_SHAPE_RECT, Size(3, 3));
	erode(img_copy, img_copy, kernel, Point(-1, -1), 1);
    dilate(img_copy, img_copy, kernel, Point(-1, -1), 3);
    /// Find contours of objects
    vector<vector<Point>> contours;
    findContours(img_copy, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    /// Higlight objects by it's contour
    img.copyTo(img_copy);
    cvtColor(img_copy, img_copy, CV_HSV2BGR);
    polylines(img_copy, contours, true, Vec3b(0, 0, 255), 2, 8);
    /// Find mass center of each house
    Moments house_moments;
    for (int i = 0; i < contours.size(); i++)
    {
        /// Check if contour length is not too low
        if (contours.at(i).size() > 10)
        {
            house_moments = moments(contours.at(i) );
            int x = house_moments.m10 / house_moments.m00;
            int y = house_moments.m01 / house_moments.m00;
            circle(img_copy, Point(x, y), 5, Vec3b(0, 0, 0), -1);
        }
    }

    imshow("lab3.2_transformed", img_copy);
    waitKey(0);
}

void findRobots(string image)
{
    Mat img = imread(image, CV_LOAD_IMAGE_COLOR);
    if (img.empty())
    {
        cout << "findRobots() : Failed to load image" << endl;
        return;
    }
}

void findLamp(string image)
{
    Mat img = imread(image, CV_LOAD_IMAGE_COLOR);
    if (img.empty())
    {
        cout << "findLamp() : Failed to load image" << endl;
        return;
    }
}

void findNearestRobot(string image)
{
    Mat img = imread(image, CV_LOAD_IMAGE_COLOR);
    if (img.empty())
    {
        cout << "findNearestRobot() : Failed to load image" << endl;
        return;
    }

}

/// Good
void findWrench(string image, string objectTemplate)
{
    Mat img = imread(image, CV_LOAD_IMAGE_COLOR);
    resize(img, img, Size(), 0.75, 0.75);
    Mat temp = imread(objectTemplate, CV_LOAD_IMAGE_COLOR);
    if (img.empty() )
    {
        cout << "findWrench() : Failed to load image" << endl;
        return;
    }
    else if (temp.empty() )
    {
        cout << "findWrench() : Failed to load template" << endl;
        return;
    }
    ///*** Find contours of main image
    // Work with copy of image (resized not to deal with such a giant)
    Mat img_copy;
    cvtColor(img, img_copy, COLOR_BGR2GRAY);
    // Get binary image for simplier compare
    threshold(img_copy, img_copy, 240, 255, THRESH_BINARY_INV);
    // Find contours of objects
    vector<vector<Point>> contours;
    findContours(img_copy, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    ///*** Find contours of template
    // All works with copy of image
    Mat temp_copy;
    cvtColor(temp, temp_copy, COLOR_BGR2GRAY);
    threshold(temp_copy, temp_copy, 240, 255, THRESH_BINARY_INV);
    vector<vector<Point>> temp_contours;
    findContours(temp_copy, temp_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    /// Compare mask with each object and higlight in different color if similar or not
    for (int i = 0; i < contours.size(); i++)
    {
        float diff = (float)matchShapes(contours.at(i), temp_contours.at(0), CV_CONTOURS_MATCH_I2, 0);
        /// If not big difference, then object is similar to template
        /// (may be difference is counted as percentage?)
        if (abs(diff) > 0.9)
        {
            polylines(img, contours.at(i), true, Vec3b(0, 255, 0), 2, 8);
        }
        else
        {
            polylines(img, contours.at(i), true, Vec3b(0, 0, 255), 2, 8);
        }
    }

    imshow("img", img);
    waitKey(0);
}