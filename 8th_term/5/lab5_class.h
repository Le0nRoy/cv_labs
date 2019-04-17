#ifndef LAB5_CLASS_H
#define LAB5_CLASS_H

#include <string>
#include <opencv2/videoio.hpp>

class lab5_class
{
public:
    lab5_class();
    ~lab5_class();

    void loadVideo ( std::string pathToVideo );

    void run_lab ( );

private:
    int width_map;
    int height_map;
    float d;
    double cx, cy, fx, fy;

    bool videoIsLoaded;
    cv::VideoCapture video;

    cv::Mat draw_map ( cv::Mat &map );

    cv::Point2d map_pixel(int col, int row);

};

#endif // LAB5_CLASS_H
