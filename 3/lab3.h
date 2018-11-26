#ifndef LAB3_LAB3_H
#define LAB3_LAB3_H

#include <vector>
#include <string>

std::vector<std::string> getFilesLab3();

void lab3();

void findHouse(std::string image);
void findEngine(std::string image);
void findRobots(std::string image);
void findLamp(std::string image);
void findNearestRobot(std::string image);
void findWrench(std::string image, std::string objectTemplate);

#endif //LAB3_LAB3_H
