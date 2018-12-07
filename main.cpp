#include "1/lab1.h"
#include "2/lab2.h"
#include "3/lab3.h"
#include "3/lab3_3.h"
#include "4/lab4.h"

#include <opencv2/core.hpp>
#include <iostream>

using namespace std;

void getFiles();
void versionOpenCV();

/// *****************************************************
/// OpenCV version   : 3.2.0
/// *****************************************************
int main()
{
    versionOpenCV();
    //vector<string> filesList =
//    lab1();
//    lab2();
    lab3_3();
//    lab4();

    return 0;
}
/// *****************************************************
/// Loads files with images. (Not ready)
/// *****************************************************
void getFiles()
{
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
//    lab3();

/// *****************************************************
/// Use this to get your OpenCV version.
/// *****************************************************
void versionOpenCV()
{
    cout << endl << "OpenCV version : " << CV_VERSION << endl << endl;
}