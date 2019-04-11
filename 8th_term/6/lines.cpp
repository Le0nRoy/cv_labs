#include <opencv2\opencv.hpp>
#include <cmath>
#include "lines.h"
using namespace cv;
using namespace std;

static Mat img, cimg, frame;
//static Mat dimg(img.rows, img.cols, CV_8UC1, Scalar(0));

//{ P2, P3, P4, P5, P6, P7, P8, P9, P2 }
static uchar P[9];
static vector<Vec4i> lines;
//static Vec3f c;

static int flag = 0;
static int time1 = 0;

static void together();
static int A();
static int B();
static void skeleton();

int lines_lab()
{
    // открываем видео
    VideoCapture cap("../6/line.avi");
    // проверяем успешно ли открыли источник видео
    if (!cap.isOpened())
    {
		return -1;
    }

    while ( waitKey(1) != 27 )
	{
		flag = 0;
        // получаем новый кадр с камеры
        cap >> frame;
		frame.copyTo(img);
		imshow("lines", img);
		blur(img, img, Size(5, 5));
        cvtColor(img, img, CV_BGR2GRAY);
        if (time1 > 0)
		{
			inRange(img, 70, 180, img);

			while (flag == 0)
			{
				skeleton();
			}
			imshow("skeleton", img);

			img.copyTo(cimg);

            // Ищем линии
			HoughLinesP(cimg, lines, 1, CV_PI / 180, 50, 50, 80);

            // Рисуем полученные линии на изображении
            for (size_t i = 0; i < lines.size(); i++)
            {
                Vec4i l = lines[i];
                line(frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);
            }
            together();
			imshow("lines", frame);
		}

		time1++;
	}
    destroyAllWindows ( );
    return 0;
}

void together()
{
	Point d;
	int r_min = 25;

    for (uint k = 0; k < lines.size(); k++)
	{
		Vec4i l = lines[k];
		for (size_t i = 0; i < lines.size(); i++)
		{
			Vec4i j = lines[i];
			if (i != k)
			{
				if (abs(l[0] - j[2]) < r_min && abs(l[1] - j[3]) < r_min)
				{
					line(frame, Point(l[0], l[1]), Point(j[2], j[3]), Scalar(0, 0, 255), 3, CV_AA);
				}
				if (abs(l[2] - j[0]) < r_min && abs(l[3] - j[1]) < r_min)
				{
					line(frame, Point(l[2], l[3]), Point(j[0], j[1]), Scalar(0, 0, 255), 3, CV_AA);
				}
				if (abs(l[0] - j[0]) < r_min && abs(l[1] - j[1]) < r_min)
				{
					line(frame, Point(l[0], l[1]), Point(j[0], j[1]), Scalar(0, 0, 255), 3, CV_AA);
				}
				if (abs(l[2] - j[2]) < r_min && abs(l[3] - j[3]) < r_min)
				{
					line(frame, Point(l[2], l[3]), Point(j[2], j[3]), Scalar(0, 0, 255), 3, CV_AA);
				}
			}
		}
	}
}

int A()
{
	int k = 0;

	if (P[0] == 0 && P[1] == 255) k++;
	if (P[1] == 0 && P[2] == 255) k++;
	if (P[2] == 0 && P[3] == 255) k++;
	if (P[3] == 0 && P[4] == 255) k++;
	if (P[4] == 0 && P[5] == 255) k++;
	if (P[5] == 0 && P[6] == 255) k++;
	if (P[6] == 0 && P[7] == 255) k++;
	if (P[7] == 0 && P[8] == 255) k++;
	return k;
}

int B()
{
	int l = 0;

	for (int i = 0; i < 7; i++)
	{
		if (P[i] == 255) l++;
	}

	return l;
}

void skeleton()
{
	Mat dimg(img.rows, img.cols, CV_8UC1, Scalar(0));

	int k, l;
	flag = 1;

	// Step 1
	for (int x = 1; x < (img.cols - 1); x++)
	{
		for (int y = 1; y < (img.rows - 1); y++)
		{
			if (img.at<uchar>(y, x) == 255)
			{
                ///{ P2, P3, P4, P5, P6, P7, P8, P9, P2 }
                //P2
                P[0] = img.at<uchar>(y - 1, x);
                //P3
                P[1] = img.at<uchar>(y - 1, x + 1);
                //P4
                P[2] = img.at<uchar>(y, x + 1);
                //P5
                P[3] = img.at<uchar>(y + 1, x + 1);
                //P6
                P[4] = img.at<uchar>(y + 1, x);
                //P7
                P[5] = img.at<uchar>(y + 1, x - 1);
                //P8
                P[6] = img.at<uchar>(y, x - 1);
                //P9
                P[7] = img.at<uchar>(y - 1, x - 1);
                //P2
                P[8] = img.at<uchar>(y - 1, x);

				l = B();
				k = A();
                // (1) and (2) conditions
                if ((2 <= l) && (l <= 6) && (k == 1))
				{
                    // (3) and (4) conditions
                    if ((P[0] == 0 || P[2] == 0 || P[4] == 0) && (P[2] == 0 || P[4] == 0 || P[6] == 0))
					{
                        // Pixel to delete
                        dimg.at<uchar>(y, x) = 255;
						flag = 0;
					}
				}
			}
		}
    }

	// Pixels delete
	for (int x = 1; x < (img.cols - 1); x++)
	{
		for (int y = 1; y < (img.rows - 1); y++)
		{
            if (dimg.at<uchar>(y, x) == 255)
            {
                img.at<uchar>(y, x) = 0;
            }
		}
	}
    // Vanishing before the next step
    dimg = 0;

	// Step 2
	for (int x = 1; x < (img.cols - 1); x++)
	{
		for (int y = 1; y < (img.rows - 1); y++)
		{
			if (img.at<uchar>(y, x) == 255)
			{
                ///{ P2, P3, P4, P5, P6, P7, P8, P9, P2 }
                //P2
                P[0] = img.at<uchar>(y - 1, x);
                //P3
                P[1] = img.at<uchar>(y - 1, x + 1);
                //P4
                P[2] = img.at<uchar>(y, x + 1);
                //P5
                P[3] = img.at<uchar>(y + 1, x + 1);
                //P6
                P[4] = img.at<uchar>(y + 1, x);
                //P7
                P[5] = img.at<uchar>(y + 1, x - 1);
                //P8
                P[6] = img.at<uchar>(y, x - 1);
                //P9
                P[7] = img.at<uchar>(y - 1, x - 1);
                //P2
                P[8] = img.at<uchar>(y - 1, x);

				l = B();
				k = A();
                // (1) and (2) conditions
                if ((2 <= l) && (l <= 6) && (k == 1))
				{
                    // (3) and (4) conditions
                    if ((P[0] == 0 || P[2] == 0 || P[6] == 0) && (P[0] == 0 || P[4] == 0 || P[6] == 0))
					{
                        // Pixel to delete
                        dimg.at<uchar>(y, x) = 255;
						flag = 0;
					}     
				}
			}
		}
    }

	// Pixels delete
	for (int x = 1; x < (img.cols - 1); x++)
	{
		for (int y = 1; y < (img.rows - 1); y++)
		{
			if (dimg.at<uchar>(y, x) == 255) img.at<uchar>(y, x) = 0;
		}
	}
	dimg = 0; // Vanishing before the next step
}
