#include <iostream>
#include <opencv2/highgui.hpp>
#include "5/lab5_class.h"
#include "5/lab5.h"
#include "6/lab6_class.h"
#include "Aruco/lab_aruco.h"
#include "Aruco/samples.hpp"

using namespace std;
using namespace cv;

/**
 * lab5 variables block
 */
static const string lab5VideoLight = "../5/0.avi";
static const string lab5VideoDark = "../5/calib_1.avi";

/**
 * lab6 variables block
 */
static const string lab6Coins = "../6/coinsImg.jpg";
static const string lab6Lines = "../6/line.avi";
static const string lab6CoinTempNickel = "../6/nickel.jpg";
static const string lab6CoinTempBrass = "../6/brass.png";

/**
 * Aruco variables block
 */
static const int labArucoVideoNum = 0;

bool run_lab5 ( );
bool run_lab6 ( );
bool run_lab_aruco ( );

int main ( )
{
    cout << endl
         << "Compiled under OpenCV ver. - 3.3.1" << endl
         << "Current OpenCV ver. - " << CV_VERSION << endl << endl;

    int labNum = 6;
    while ( labNum )
    {
        cout << endl
             << "0 - to stop programm" << endl
             << "5 - for lab5" << endl
             << "6 - for lab6" << endl
             << "7 - for Aruco" << endl
             << "Enter number of lab: ";
        cin >> labNum;
        switch ( labNum )
        {
            case 5:
            {
                while ( run_lab5 ( ) )
                {}
                break;
            }
            case 6:
            {
                while ( run_lab6 ( ) )
                {}
                break;
            }
            case 7:
            {
                while ( run_lab_aruco ( ) )
                {}
                break;
            }
            case 0:
            {
                cout << endl << "Exiting programm..." << endl;
                break;
            }
            default:
            {
                cout << "Wrong lab number" << endl;
            }
        }
    }
    destroyAllWindows ( );
    return 0;
}

bool run_lab5 ( )
{
//    lab5_class lab5Class;
//    lab5Class.loadVideo ( lab5VideoLight );
//    lab5Class.run_lab ( );
    lab5 ( );
    destroyAllWindows ( );
    return false;
}

bool run_lab6 ( )
{
    bool exit_code = false;
    // here wrapping for lab num
    int taskNum = 1;
    lab6_class lab6;
    //lab6Coins, lab6Lines
    lab6.loadLines ( lab6Lines );
    lab6.loadCoins ( lab6Coins );
    lab6.loadTemplates ( lab6CoinTempNickel, lab6CoinTempBrass );

    while ( taskNum )
    {
        cout << endl
             << "-1 - to reload lab6" << endl
             << "0 - to stop lab6" << endl
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
                if ( !lab6.lines_lab ( ) )
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

bool run_lab_aruco ( )
{
    lab_aruco labAruco;
    labAruco.loadVideo ( labArucoVideoNum );
    aruco_lab ( );
//    labAruco.run_lab ( );
    destroyAllWindows ( );
    return false;
}
