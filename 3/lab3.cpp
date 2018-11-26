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
//    imgForFunc = images.at(6);
//    findRobots(imgForFunc);

    /// Fourth part (wrench)
    imgForFunc = images.at(3);
    string wrenchTemplate = images.at(4);
    findWrench(imgForFunc, wrenchTemplate);

    destroyAllWindows();
}
/**
 * @brief - Loads files with images
 * @return - vector with filenames of images
 */
vector<string> getFilesLab3()
{
    vector<string> images(15); // 12 pictures for lab3, but allocate more memory as stock

    string wayToFolder = "../3/Task/img_zadan"; // Way from home to folder with folders that contain images

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
/**
 * @brief - find objects with binary image
 * Houses are warmer, than anything else.
 * Need to find all of them and mark center of each
 * @param image - image with object to detect
 */
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
//    createTrackbar("findHouse", "lab3", &thresh, 255, findHouse, images);
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

    imshow("lab3.1_transformed", imgGRAY);
    waitKey(0);
}
/**
 * @brief - find objects with HSV
 * Engines are warmer than anything else.
 * Need to find and mark center.
 * @param image - image with object to detect
 */
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

    imshow("lab3.2_transformed", img_copy);
    waitKey(0);
}
/**
 * @brief - Find colorful cap and circle around it color of robots' team.
 * @param image - image with object to detect
 */
void findRobots(string image)
{
    Mat img = imread(image, CV_LOAD_IMAGE_COLOR);
    if (img.empty())
    {
        cout << "findRobots() : Failed to load image" << endl;
        return;
    }
}

/// *****************************************************
/// Find lamp and mark it.
/// *****************************************************
void findLamp(string image)
{
    Mat img = imread(image, CV_LOAD_IMAGE_COLOR);
    if (img.empty())
    {
        cout << "findLamp() : Failed to load image" << endl;
        return;
    }
}

/// *****************************************************
/// Find the robot closest to the lamp and mark it's mass center.
/// *****************************************************
void findNearestRobot(string image)
{
    Mat img = imread(image, CV_LOAD_IMAGE_COLOR);
    if (img.empty())
    {
        cout << "findNearestRobot() : Failed to load image" << endl;
        return;
    }

}

/// *****************************************************
/// Find deffective wrenches and mark them.
/// *****************************************************
void findWrench(string image, string objectTemplate)
{
    Mat img = imread(image, CV_LOAD_IMAGE_COLOR);
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
    /// All works with copy of image (resized not to deal with such a giant)
    Mat img_copy;
    img.copyTo(img_copy);
    resize(img, img, Size(), 0.75, 0.75);
    /// All works with copy of image
    Mat temp_copy;
    temp.copyTo(temp_copy);

    imshow("img", img);
    imshow("template", temp);
    waitKey(0);
}