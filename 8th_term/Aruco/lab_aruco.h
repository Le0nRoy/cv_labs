#ifndef LAB_ARUCO_H
#define LAB_ARUCO_H

#include <string>
#include <opencv2/videoio.hpp>

class lab_aruco
{
public:
    lab_aruco ( );

    void loadVideo ( int videoNum, std::string pathToVideo = "\0" );

    void run_lab ( );

private:
    bool captureIsOpened;
    cv::VideoCapture capture;

    void save_marker ( );

    void find_markers ( cv::InputArray src, cv::OutputArray outImg );

};

#endif // LAB_ARUCO_H
