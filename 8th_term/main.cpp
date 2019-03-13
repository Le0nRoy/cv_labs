#include <iostream>
#include <opencv2/highgui.hpp>
#include "6/lab6_class_lines.h"

using namespace std;
using namespace cv;

const string lab6Coins = "../6/coins.jpg";
const string lab6Lines = "../6/line.avi";

bool run_lab ( );

int main ( )
{
    cout << endl
         << "Compiled under OpenCV ver. 3.3.1" << endl
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
    lab6_class lab6 ( lab6Coins, lab6Lines );
    while ( taskNum )
    {
        cout << endl
             << "-1 - to reload programm" << endl
             << "0 - to stop programm" << endl
             << "1 - for coins" << endl
             << "2 - for lines" << endl
             << "Enter number of task: ";
        cin >> taskNum;
        switch ( taskNum )
        {
            case 1:
            {
                if ( !lab6.findCoins ( ) )
                {
                    cout << "Failed to find coins !" << endl;
                }
                break;
            }
            case 2:
            {
                if ( lab6.findLines ( ) )
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