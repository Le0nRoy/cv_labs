#include <opencv2\opencv.hpp>
#include <complex>
#define _USE_MATH_DEFINES //??? M_PI
#include <math.h> //??? M_PI
using namespace cv;
using namespace std;

void dft_my();
void beautySpectrum(Mat &magI);

Mat img, ft_img;
double fi;

int main()
{
	img = imread("2.jpeg", CV_LOAD_IMAGE_GRAYSCALE);
	resize(img, img, Size(), 0.5, 0.5);
	imshow("original", img);

	Mat ft_img(img.cols, img.rows, CV_32FC2, Scalar(0, 0));
	//Mat out(img.cols, img.rows, CV_32FC1, 0);

	fi = 2 * M_PI / img.rows;





	int rows = img.rows;
	int cols = img.cols;

	//??????? ??? ????
	// Make 2 zeros arrays
	complex<double>** sum1 = new complex<double>*[rows];
	complex<double>** sum2 = new complex<double>*[rows];
	for (int i = 0; i < rows; i++)
	{
		sum1[i] = new complex<double>[cols];
		sum2[i] = new complex<double>[cols];
		for (int j = 0; j < cols; j++)
		{
			sum1[i][j] = 0;
			sum2[i][j] = 0;
		}
	}

	//??? ?? ???????(??????????)
	// Count first sum
	for (int i = 0; i < rows; i++)	//????? ??????
	{
		for (int k1 = 0; k1 < cols; k1++)	//????? ???????
		{
			for (int n1 = 0; n1 < cols; n1++)	//????? ?????????
			{
				sum1[i][k1] += (double)img.at<uchar>(i, n1)*complex<double>(cos(-fi*n1*k1), sin(-fi*n1*k1));
			}
		}
	}

	//??? ?? ????????(??????????)
	// Count second sum
	for (int j = 0; j < cols; j++)	//????? ???????
	{
		for (int k2 = 0; k2 < rows; k2++)	//????? ??????
		{
			for (int n2 = 0; n2 < rows; n2++)	//????? ?????????
			{
				sum2[k2][j] += sum1[n2][j] * complex<double>(cos(-fi*n2*k2), sin(-fi*n2*k2));
			}
			ft_img.at<Vec2f>(k2, j)[0] = sum2[k2][j].real();
			ft_img.at<Vec2f>(k2, j)[1] = sum2[k2][j].imag();
			//ft_img.at<float>(k2, j) = abs(sum2[k2][j]);
			//magnitude(img.at<Vec2f>(k2, j)[0], img.at<Vec2f>(k2, j)[1], out.at<cv::Vec2f>(k2, j));
		}
	}

	Mat spimg[] = { Mat(img.rows, img.cols, CV_32FC1), Mat(img.rows, img.cols, CV_32FC1) };
	split(ft_img, spimg);
	magnitude(spimg[0], spimg[1], spimg[0]);

	Mat magI = spimg[0];
	magI += Scalar::all(1);                    // switch to logarithmic scale
	log(magI, magI);

	normalize(magI, magI, 0, 1, NORM_MINMAX);

	beautySpectrum(magI);

	imshow("spectrum magnitude", magI);

	//dft_my();

	waitKey(0);
}

void dft_my()
{
	
}

void beautySpectrum(Mat &magI)
{
	// rearrange the quadrants of Fourier image  so that the origin is at the image center
	int cx = magI.cols / 2;
	int cy = magI.rows / 2;

	Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
	Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
	Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
	Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

	Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);
}