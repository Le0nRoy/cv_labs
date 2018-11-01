///
/// Created by Vadim Trishin
///

#include "3/lab3.h"


using namespace std;
using namespace cv;

void getFiles();
void versionOpenCV();

/// *****************************************************
/// OpenCV version   : 3.2.0
/// Major version    : 3
/// Minor version    : 2
/// Subminor version : 0
/// *****************************************************
int main() {
    //vector<string> filesList =
    lab3();
    return 0;
}

/// *****************************************************
/// Loads files with images. (Not ready)
/// *****************************************************
void getFiles() {
    vector<string> images(15);

    /*
    FILE *popen_result;
    char buff[512];
    popen_result = popen("ls ~/CV_labs/labs/3/img_zadan/", "r");

    if(!popen_result) {
        printf("Can't execute command\n");
        return 0;
    }

    while(fgets(buff, sizeof(buff), popen_result)!=NULL) {
        printf("%s", buff);
    }

    pclose(popen_result);
     */
}

/// *****************************************************
/// Use this to get your OpenCV version.
/// *****************************************************
void versionOpenCV() {
    cout << "OpenCV version : " << CV_VERSION << endl;
    cout << "Major version : " << CV_MAJOR_VERSION << endl;
    cout << "Minor version : " << CV_MINOR_VERSION << endl;
    cout << "Subminor version : " << CV_SUBMINOR_VERSION << endl;
}