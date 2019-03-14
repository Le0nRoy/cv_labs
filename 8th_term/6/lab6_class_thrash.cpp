#include "lab6_class.h"
#include <opencv2/core.hpp>
#include "opencv2/core/matx.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/video/tracking.hpp"
#include <iostream>

using namespace cv;
using namespace std;

static string windowLinesOriginal = "lines_original";
static string windowLinesThreshed = "lines_threshed";
static string windowLinesSkeleted = "lines_skeleted";
static const int threshMaxVal = 255;

static int trackbarValueWaitTimeVideo = 10;
static const int trackbarLimitWaitTimeVideo = 25;

bool lab6_class::task_lines_video ( )
{
    if ( !linesLoaded )
    {
        cout << "taskLines () : Video is not loaded !" << endl;
        return false;
    }

    Mat frame;
    linesVideo.read ( frame );
    if ( frame.empty ( ) )
    {
        cout << "taskLines () : Frame is not loaded !" << endl;
        return false;
    }

    makeWindowsLines ( );

    createTrackbar ( "Time_skelet", windowLinesSkeleted, &trackbarValueWaitTimeSkelet, trackbarLimitWaitTimeSkelet );

    imshow ( windowLinesOriginal, frame );
    waitKey ( 0 );

    while ( !frame.empty ( ) )
    {
        Mat skelet;
        skeletezation ( frame, skelet );
//    imshow ( windowLinesSkeleted, skelet );
//    waitKey ( 1 );

        vector < Vec2f > lines;
        find_lines ( skelet, lines );
        if ( lines.empty ( ) )
        {
            cout << "No lines found" << endl;
            destroyWindowsLines ( );
            return false;

        }

        merge_lines ( lines );

        draw_lines ( frame, lines );
        imshow ( windowLinesOriginal, frame );

        waitKey ( trackbarValueWaitTimeVideo );
        linesVideo.read ( frame );
    }
    waitKey ( 0 );

    destroyWindowsLines ( );
    return true;
}
