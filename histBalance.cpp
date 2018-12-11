#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
using namespace std;
using namespace cv;

int round(double n)
{
	return (int)(n + 0.5);
}
void histBalance(Mat &image)
{
	int array[256] = {0};
	int H_c[256] = {0};
	int T_p[256] = {0};
	Mat hist = image.clone();
	for(int i = 0; i < image.cols; i++)
	{
		for(int j = 0; j < image.rows; j++)
		{
			array[image.at<uchar>(j, i)]++;
		}
	}

	for(int i = 255; i >= 0; i--)
	{
		for(int j = 0; j <= i; j++)
		{
			H_c[i] += array[j];
		}
	}

	for(int i = 0; i < 256; i++)
	{
		T_p[i] =  round( (double)(256 - 1) / (image.rows * image.cols) * H_c[i]);
	}
	for(int i = 0; i < hist.cols; i++)
	{
		for(int j = 0; j < hist.rows; j++)
		{
			uchar * color_ptemp = hist.ptr<uchar>(j);
			color_ptemp[i] = T_p[color_ptemp[i]];
		}
	}
	imshow("hist",hist);
	waitKey();
}

void main()
{
	Mat image = imread("lena.png",IMREAD_GRAYSCALE);
	histBalance(image);
	imshow("lena",image);
	waitKey();
}
