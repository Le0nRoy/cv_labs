#ifndef LAB5_CLASS_H
#define LAB5_CLASS_H

#include <string>
#include <opencv2/videoio.hpp>

class lab5_class
{
public:
    lab5_class();

    void loadVideo ( std::string pathToVideo );

    void run_lab ( );

private:
    bool videoIsLoaded;
    cv::VideoCapture video;

    void threshold_lighting ( cv::InputArray src, cv::OutputArray threshed_img );

};

#endif // LAB5_CLASS_H
