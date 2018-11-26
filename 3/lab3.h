///
/// Created by Vadim Trishin
///

#ifndef LAB3_LAB3_H
#define LAB3_LAB3_H

#include <vector>
#include <string>

std::vector<std::string> getFilesLab3();

void lab3();

void findHouse(std::string image);
void findEngine(std::vector<std::string> images);
void findRobots(std::vector<std::string> images);
void findLamp(std::vector<std::string> images);
void findNearestRobot(std::vector<std::string> images);
void findWrench(std::vector<std::string> images);

void callbackHouse(int thresh, void*);

#endif //LAB3_LAB3_H
