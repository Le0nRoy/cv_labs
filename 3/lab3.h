///
/// Created by Vadim Trishin
///

#ifndef LAB3_LAB3_H
#define LAB3_LAB3_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <stdio.h>
#include <vector>
#include <iostream>

std::vector<std::string> getFilesLab3();

void lab3();

void findHouse(int *thresh, std::vector<string> images);
void findEngine(std::vector<std::string> images);
void findRobots(std::vector<std::string> images);
void findLamp(std::vector<std::string> images);
void findNearestRobot(std::vector<std::string> images);
void findWrench(std::vector<std::string> images);

void callbackHouse(int thresh, void*);

#endif //LAB3_LAB3_H
