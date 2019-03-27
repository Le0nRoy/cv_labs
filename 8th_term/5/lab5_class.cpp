#include "lab5_class.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

static const std::string windowVideo = "capture";

lab5_class::lab5_class() :
    videoIsLoaded ( false )
{

}

void lab5_class::loadVideo ( std::string pathToVideo )
{
    video.open ( pathToVideo );

    if ( !video.isOpened ( ) )
    {
        videoIsLoaded = false;
        std::cout << "loadVideo () : failed to load video !" << std::endl;
        return;
    }

    videoIsLoaded = true;
}

void lab5_class::run_lab (  )
{
    if ( !videoIsLoaded )
    {
        std::cout << "run_lab ( ) : no video downloaded !" << std::endl;
    }
    cv::Mat frame;
    video.read ( frame );

    int exitCondition = 1;
    cv::namedWindow ( windowVideo, cv::WINDOW_AUTOSIZE );
    cv::createTrackbar ( "turn_off", windowVideo, &exitCondition, 1 );

    while ( !frame.empty ( ) && exitCondition )
    {
        cv::imshow ( windowVideo, frame );
        video.read ( frame );
        cv::waitKey ( 5 );
    }
    cv::destroyWindow ( windowVideo );
}

void lab5_class::threshold_lighting ( cv::InputArray src, cv::OutputArray threshed_img )
{
    cv::Mat srcImg;
    srcImg = src.getMat( );
    if ( srcImg.empty ( ) )
    {
        std::cout << "threshold_lighting () : Image is empty !" << std::endl;
        return;
    }

    cv::Mat imgThreshed;
    threshed_img.create ( srcImg.size ( ), CV_8U );
    imgThreshed = threshed_img.getMat_ ( );
    //  через inRange оставить только подсветку
}
