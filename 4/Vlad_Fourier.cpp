#define pi 3.14159265358979323846
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <stdio.h>
#include <iostream>
#include <complex>
#include <cmath>

using namespace cv;
using namespace std;

void show_fourier(Mat &complexI, String name);
void shift_rect(Mat& fourier_img);
void correlation(Mat img, Mat sign, String str);

int main() {
	namedWindow("main");
	char task = waitKey(0);
	switch (task) {
	case '2': {
		Mat img = imread("Lenna.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		imshow("original", img);

		Mat dft_img(img.size(), CV_32FC2);
		img.convertTo(img, CV_32FC1);
		dft(img, dft_img, DFT_COMPLEX_OUTPUT);
		show_fourier(dft_img, "CV DFT");
		char action = waitKey(0);
		float kernel[3][3];
		switch (action)
		{
		case 'h':
		    {
			float sobel_hor[][3] = { -1, -2, -1,
									 0, 0, 0,
									1, 2, 1 };
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					kernel[i][j] = sobel_hor[i][j];
				}
			}
			break;
		}
		case 'v':
		    {
			float sobel_ver[][3] = { -1, 0, 1,
									-2, 0, 2,
									-1, 0, 1 };
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					kernel[i][j] = sobel_ver[i][j];
				}
			}
			break;
		}
		case 'l':
		    {
			float laplase[][3] = { 0, 1, 0,
								  1, -4, 1,
								  0, 1, 0 };
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					kernel[i][j] = laplase[i][j];
				}
			}
			break;
		}
		default:
			break;
		}
		Mat mat_kernel(img.size(), CV_32FC1, Scalar(0));
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				mat_kernel.at<float>(i, j) = kernel[i][j];
			}
		}

		Mat dft_kernel(img.size(), CV_32FC2);
		dft(mat_kernel, dft_kernel, DFT_COMPLEX_OUTPUT);
		show_fourier(dft_kernel, "CV DFT");

		Mat res(img.size(), CV_32FC2);
		mulSpectrums(dft_kernel, dft_img, res, 0, 0);
		show_fourier(res, "mulSpectrums"); // ������� ��������� ������ ������ ����� 

		Mat idft_img(img.size(), CV_32FC1);
		dft(res, idft_img, DFT_INVERSE | DFT_REAL_OUTPUT);
		normalize(idft_img, idft_img, 0.0, 255, CV_MINMAX);
		idft_img.convertTo(idft_img, CV_8UC1);
		imshow("res", idft_img);
		break;
	}
	case '3': {
		Mat img = imread("Lenna.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		imshow("original", img);

		Mat dft_img(img.size(), CV_32FC2);
		img.convertTo(img, CV_32FC1);
		dft(img, dft_img, DFT_COMPLEX_OUTPUT);
		show_fourier(dft_img, "CV DFT");

		Mat mat_kernel(img.size(), CV_32FC2, Scalar(0));
		circle(mat_kernel, { img.rows / 2, img.cols / 2 }, 40, Scalar(1, 0), -1);

		Mat res;
		shift_rect(dft_img);
		mulSpectrums(dft_img, mat_kernel, res, 0);
		shift_rect(res);
		show_fourier(res, "res");

		Mat idft_img(img.size(), CV_32FC1);
		dft(res, idft_img, DFT_INVERSE | DFT_REAL_OUTPUT);
		normalize(idft_img, idft_img, 0.0, 255, CV_MINMAX);
		idft_img.convertTo(idft_img, CV_8UC1);
		imshow("idft_res", idft_img);
		break;
	}
	case '4': {
		Mat img = imread("table.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		imshow("original", img);

		Mat A = imread("A.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		imshow("A", A);

		Mat seven = imread("seven.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		imshow("seven", seven);

		Mat zero = imread("zero.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		imshow("zero", zero);

		correlation(img.clone(), A.clone(), "A");
		correlation(img.clone(), seven.clone(), "seven");
		correlation(img.clone(), zero.clone(), "zero");

		break;
	}
	default: break;
	}
	waitKey();
	return 0;
}

void show_fourier(Mat &complexI, String name) {
	/*
	����������� dft �������� ������������� �����������
	�� ��� ������� � ������� ���������
	*/
	Mat planes[2];
	split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
	magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
	Mat magI = planes[0];
	Mat fourier_img = magI.clone();

	//����� ���������� � �����
	fourier_img += Scalar::all(1);
	log(fourier_img, fourier_img);
	// rearrange the quadrants of Fourier image  so that the origin is at the image center
	shift_rect(fourier_img);

	normalize(fourier_img, fourier_img, 0.0, 1.0, CV_MINMAX);
	imshow("dft" + name, fourier_img);
}

void shift_rect(Mat& fourier_img) {
	//������ ��������������� �������
	int cx = fourier_img.cols / 2;
	int cy = fourier_img.rows / 2;

	Mat q0(fourier_img, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
	Mat q1(fourier_img, Rect(cx, 0, cx, cy));  // Top-Right
	Mat q2(fourier_img, Rect(0, cy, cx, cy));  // Bottom-Left
	Mat q3(fourier_img, Rect(cx, cy, cx, cy)); // Bottom-Right

	Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);
}

void correlation(Mat img, Mat ch, String str) {
	//������� ������������� ����������� ��� dft 
	Mat dft_img(img.size(), CV_32FC2);
	img.convertTo(img, CV_32FC1);
	Mat mean_img, std_img;
	meanStdDev(img, mean_img, std_img);
	img -= mean_img;
	img /= std_img;

	dft(img, dft_img, DFT_COMPLEX_OUTPUT);
	show_fourier(dft_img, "_img");

	//��������� ������� ������ �� ������� �����������
	ch.convertTo(ch, CV_32FC1);
	Mat mean_ch, std_ch;
	meanStdDev(ch, mean_ch, std_ch);
	ch -= mean_ch;
	ch /= std_ch;

	Mat sign(img.size(), CV_32FC1, Scalar(0));
	Mat roi(sign, Rect(0, 0, ch.cols, ch.rows));
	ch.copyTo(roi);

	imshow("sign" + str, sign);

	Mat dft_sign(sign.size(), CV_32FC2);
	sign.convertTo(sign, CV_32FC1);
	dft(sign, dft_sign, DFT_COMPLEX_OUTPUT);
	show_fourier(dft_sign, "_" + str);

	//������ ���������� ���� �����������
	Mat img_sign;
	mulSpectrums(dft_img, dft_sign, img_sign, 0, true);
	show_fourier(img_sign, "_img_" + str);

	Mat idft_img(img.size(), CV_32FC1);
	dft(img_sign, idft_img, DFT_INVERSE | DFT_REAL_OUTPUT);
	normalize(idft_img, idft_img, 0.0, 255, CV_MINMAX);
	idft_img.convertTo(idft_img, CV_8UC1);
	imshow("idft_res" + str, idft_img);

	double minVal;
	double maxVal;
	minMaxLoc(idft_img, &minVal, &maxVal);
	
	//�������� ��� �������� � �������� ����� ������ �������
	Mat idft_img_bin;
	threshold(idft_img, idft_img_bin, (maxVal-5), 255, THRESH_BINARY_INV);
	imshow("res_" + str, idft_img_bin);
}