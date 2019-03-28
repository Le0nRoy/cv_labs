#include "lab_aruco.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/aruco.hpp>
#include <iostream>

static const std::string windowCapture = "capture";
static const std::string windowMarker = "marker";

lab_aruco::lab_aruco ( ) :
    captureIsOpened ( false )
{
}

void lab_aruco::loadVideo ( int videoNum, std::string pathToVideo )
{
    if ( pathToVideo == "\0" )
    {
        capture.open ( videoNum );
    }
    else
    {
        capture.open ( pathToVideo );
    }

    if ( !capture.isOpened ( ) )
    {
        captureIsOpened = false;
        std::cout << "loadVideo () : failed to open capture !" << std::endl;
        return;
    }

    captureIsOpened = true;
}

void lab_aruco::run_lab ( )
{
    if ( !captureIsOpened )
    {
        std::cout << "run_lab ( ) : no videoCapture opened !" << std::endl;
    }
    cv::Mat frame;
//    capture.read ( frame );

    cv::namedWindow ( windowCapture, cv::WINDOW_AUTOSIZE );

//    save_marker ( );

    bool exitCondition = true;
    while ( !frame.empty ( ) && exitCondition )
    {
        frame = cv::imread ( "../Aruco/" );
        cv::Mat detectedMarkers;
        find_markers ( frame, detectedMarkers );

        cv::imshow ( windowCapture, frame );
        cv::imshow ( windowMarker, detectedMarkers );
        if ( cv::waitKey ( 0 ) == 27 )
        {
            exitCondition = false;
        }
        capture.read ( frame );
    }
    cv::destroyWindow ( windowCapture );
}

void lab_aruco::find_markers ( cv::InputArray src, cv::OutputArray outImg )
{
    try
    {
        cv::Mat inputImage = src.getMat ( );
        if ( inputImage.empty ( ) )
        {
            std::cout << "find_markers ( ) : no image given !" << std::endl;
            return;
        }

        outImg.create ( inputImage.size ( ), inputImage.type ( ) );
        cv::Mat outputImage;
        outputImage = outImg.getMat ( );
        inputImage.convertTo ( outputImage, -1 );

        std::vector < int > markerIds;
        std::vector < std::vector < cv::Point2f > > markerCorners;
        std::vector < std::vector < cv::Point2f > > rejectedCandidates;

        cv::Ptr < cv::aruco::Dictionary > dict = cv::aruco::getPredefinedDictionary ( cv::aruco::DICT_6X6_250 );

        cv::Ptr < cv::aruco::DetectorParameters > parameters = cv::aruco::DetectorParameters::create();

        cv::aruco::detectMarkers ( inputImage, dict, markerCorners, markerIds, parameters, rejectedCandidates );

        cv::aruco::drawDetectedMarkers ( outputImage, markerCorners, markerIds );
    }
    catch ( std::exception &e )
    {
        std::cout << std::endl << std::endl << "find_markers ( ) : EXCEPTION !!!" << std::endl;
        std::cout << e.what ( ) << std::endl;
    }
}

void lab_aruco::save_marker ( )
{
    try
    {
        cv::Mat markerImage;
        cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary ( cv::aruco::DICT_6X6_250 );

        dictionary.drawMarker ( 23, 200, markerImage, 1 );

        cv::imshow ( windowMarker, markerImage );
//        cv::cvtColor ( markerImage, markerImage, CV_GRAY2BGR );
        cv::imwrite ( "../Aruco/marker.jpg", markerImage );
        cv::waitKey ( 0 );
    }
    catch ( std::exception &e )
    {
        std::cout << std::endl << std::endl << "save_marker ( ) : EXCEPTION !!!" << std::endl;
        std::cout << e.what ( ) << std::endl;
    }
}
