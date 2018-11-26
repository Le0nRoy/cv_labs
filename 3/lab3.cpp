#include "lab3.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

//#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

void lab3()
{
    vector<string> images = getFilesLab3();
    string imgForFunc;
//    imgForFunc = images.at(0);
    imgForFunc = images.at(1);
//    imgForFunc = images.at(2);
    findHouse(imgForFunc);
    // createTrackbar("findHouse", "lab3", &thresh, 255, findHouse, images);
    waitKey(0);
}

/// *****************************************************
/// Loads files with images.
/// *****************************************************
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
    images.at(7)  = wayToFolder+"/teplovizor/21331.jpg";
    images.at(8)  = wayToFolder+"/teplovizor/ntcs_quest_measurement.png";
    images.at(9)  = wayToFolder+"/teplovizor/445923main_STS128_16FrAvg_color1.jpg";
    images.at(10) = wayToFolder+"/teplovizor/size0-army.mil-2008-08-28-082221.jpg";
    images.at(11) = wayToFolder+"/teplovizor/MW-AW129-measured.jpg";

    return images;
}

/// *****************************************************
/// Houses are warmer, than anything else.
/// Need to find all of them and mark center of each
/// *****************************************************
/**
 * @brief - find objects by threshold and findContours
 * Houses are warmer, than anything else.
 * Need to find all of them and mark center of each
 * @param image
 */
void findHouse(string image)
{
    Mat ig_0 = imread(image, CV_LOAD_IMAGE_GRAYSCALE);
    /// All works with copy of image
    Mat ig_0_cp;
    ig_0.copyTo(ig_0_cp);
    /// Set to binary mode
    threshold(ig_0_cp, ig_0_cp, 220, 255, THRESH_BINARY);
    /// Use morphology to throw away noises
    Mat kernel = getStructuringElement(CV_SHAPE_RECT, Size(3, 3));
	erode(ig_0_cp, ig_0_cp, kernel, Point(-1, -1), 1);
    dilate(ig_0_cp, ig_0_cp, kernel, Point(-1,-1), 4);
    /// Find contours of object
    vector<vector<Point>> contours;
    findContours(ig_0_cp, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    /// Higlight object by it's contour
    ig_0.copyTo(ig_0_cp);
    cvtColor(ig_0_cp, ig_0_cp, CV_GRAY2BGR);
    polylines(ig_0_cp, contours, true, Vec3b(0, 0, 255), 2, 8);

    imshow("lab3.1_transformed", ig_0_cp);
    waitKey(0);
}

void callbackHouse(int thresh, void*)
{

}
/// *****************************************************
/// Engines are warmer than anything else.
/// Need to find and mark center.
/// *****************************************************
void findEngine(vector<string> images)
{
    Mat ig_7 = imread(images.at(7), CV_LOAD_IMAGE_COLOR);
    Mat ig_8 = imread(images.at(8), CV_LOAD_IMAGE_COLOR);
    Mat ig_9 = imread(images.at(9), CV_LOAD_IMAGE_COLOR);
    Mat ig_10 = imread(images.at(10), CV_LOAD_IMAGE_COLOR);
    Mat ig_11 = imread(images.at(11), CV_LOAD_IMAGE_COLOR);
}

/// *****************************************************
/// Find colorful cap and circle around it color of robots' team.
/// *****************************************************
void findRobots(vector<string> images)
{
    Mat ig_5 = imread(images.at(5), CV_LOAD_IMAGE_COLOR);
    Mat ig_6 = imread(images.at(6), CV_LOAD_IMAGE_COLOR);
}

/// *****************************************************
/// Find lamp and mark it.
/// *****************************************************
void findLamp(vector<string> images)
{
    Mat ig_5 = imread(images.at(5), CV_LOAD_IMAGE_COLOR);
    Mat ig_6 = imread(images.at(6), CV_LOAD_IMAGE_COLOR);
}

/// *****************************************************
/// Find the robot closest to the lamp and mark it's mass center.
/// *****************************************************
void findNearestRobot(vector<string> images)
{
    Mat ig_5 = imread(images.at(5), CV_LOAD_IMAGE_COLOR);
    Mat ig_6 = imread(images.at(6), CV_LOAD_IMAGE_COLOR);

}

/// *****************************************************
/// Find deffective wrenches and mark them.
/// *****************************************************
void findWrench(vector<string> images)
{
    Mat ig_3 = imread(images.at(3), CV_LOAD_IMAGE_COLOR);
    Mat ig_4 = imread(images.at(4), CV_LOAD_IMAGE_COLOR);

}