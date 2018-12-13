#include <iostream>
#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;

void main()
{
	Mat image = imread("lena.png", IMREAD_GRAYSCALE);
	Mat dst = Mat::zeros(image.size(), CV_8UC1);
	for(int j = 1; j < image.rows - 1; j++)
	{
		uchar *color_ptemp = dst.ptr<uchar>(j);
		for(int i = 1; i < image.cols - 1; i++)
		{
			uchar color = image.at<uchar>(j,i);
			int mo = 0;
			for(int k = -1; k <= 1; k++)
			{
				for(int l = -1; l <= 1; l++)
				{
					mo += abs((double)(image.at<uchar>(j + l, i + k) - color));
				}

			}
			color_ptemp[i] = mo / 8;

		}

	}
	imshow("dst",dst);
	waitKey();
}
