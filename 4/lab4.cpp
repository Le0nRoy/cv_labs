//
// Created by bledgharm on 05.11.18.
//

#include "lab4.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

void lab4()
{
    vector<string> images(3);
    images = getFilesLab4();
    waitKey(0);
}

/// *****************************************************
/// Loads files with images.
/// *****************************************************
vector<string> getFilesLab4() {
    vector<string> images(3); // 12 pictures for lab3, but allocate more memory as stock

    string wayToFolder = "/home/bledgharm/CV_labs/labs/4/"; // Way from home to folder with folders that contain images

    /// allababah
    images.at(0)  = wayToFolder+"ig_0.jpg";
    images.at(1)  = wayToFolder+"ig_1.jpg";

    return images;
}