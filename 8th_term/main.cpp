#include <iostream>
#include <opencv2/highgui.hpp>
#include "6/lab6_class.h"

using namespace std;
using namespace cv;

const string lab6Coins = "../6/coins.jpg";
const string lab6Lines = "";

int main ( )
{
    cout << endl
         << "Compiled under OpenCV ver. 3.3.1" << endl
         << "Current OpenCV ver. - " << CV_VERSION << endl << endl;

    // here wrapping for lab num
    int taskNum = 1;
    lab6_class lab6 ( lab6Coins, lab6Lines );
    while ( taskNum )
    {
        cout << endl
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
                break;
            }
            default:
            {
                cout << "Wrong task number" << endl;
            }
        }
    }
    destroyAllWindows ();
    return 0;
}