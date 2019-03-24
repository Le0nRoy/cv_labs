#include <iostream>
#include <opencv2/highgui.hpp>
#include "6/lab6_class.h"

using namespace std;
using namespace cv;

static const string lab6Coins = "../6/coinsImg.jpg";
static const string lab6Lines = "../6/line.avi";
static const string coinTempNickel = "../6/nickel.jpg";
static const string coinTempBrass = "../6/brass.png";

bool run_lab ( );

int main ( )
{
    cout << endl
         << "Compiled under OpenCV ver. - 3.3.1" << endl
         << "Current OpenCV ver. - " << CV_VERSION << endl << endl;
    while ( run_lab ( ) )
    {}
    return 0;
}

bool run_lab ( )
{
    bool exit_code = false;
    // here wrapping for lab num
    int taskNum = 1;
    lab6_class lab6;
    //lab6Coins, lab6Lines
    lab6.loadLines ( lab6Lines );
    lab6.loadCoins ( lab6Coins );
    lab6.loadTemplates ( coinTempNickel, coinTempBrass );

    while ( taskNum )
    {
        cout << endl
             << "-1 - to reload programm" << endl
             << "0 - to stop programm" << endl
             << "1 - for coinsImg" << endl
             << "2 - for linesVideo" << endl
             << "Enter number of task: ";
        cin >> taskNum;
        switch ( taskNum )
        {
            case 1:
            {
                if ( !lab6.task_coins ( ) )
                {
                    cout << "Failed to find coinsImg !" << endl;
                }
                break;
            }
            case 2:
            {
                if ( lab6.task_lines ( ) )
                {
                    waitKey ( 0 );
                }
                break;
            }
            case 0:
            {
                cout << endl << "Exiting programm..." << endl;
                exit_code = false;
                break;
            }
            case -1:
            {
                cout << endl << "Reloading programm..." << endl;
                exit_code = true;
                break;
            }
            default:
            {
                cout << "Wrong task number" << endl;
            }
        }
    }
    destroyAllWindows ();
    return exit_code;
}

/***
# take first frame of the video
ret,frame = cap.read()
# setup initial location of window
r,h,c,w = 250,90,400,125  # simply hardcoded the values
track_window = (c,r,w,h)
# set up the ROI for tracking
roi = frame[r:r+h, c:c+w]
hsv_roi =  cv2.cvtColor(roi, cv2.COLOR_BGR2HSV)

mask = cv2.inRange(hsv_roi, np.array((0., 60.,32.)), np.array((180.,255.,255.)))

roi_hist = cv2.calcHist([hsv_roi],[0],mask,[180],[0,180])
cv2.normalize(roi_hist,roi_hist,0,255,cv2.NORM_MINMAX)

# Setup the termination criteria, either 10 iteration or move by atleast 1 pt
term_crit = ( cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 10, 1 )
while(1):
    ret ,frame = cap.read()
    if ret == True:
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        dst = cv2.calcBackProject([hsv],[0],roi_hist,[0,180],1)

        # apply meanshift to get the new location
        ret, track_window = cv2.meanShift(dst, track_window, term_crit)

        # Draw it on image
        x,y,w,h = track_window
        img2 = cv2.rectangle(frame, (x,y), (x+w,y+h), 255,2)
        cv2.imshow('img2',img2)
*/
