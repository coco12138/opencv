#include <iostream>
#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;



void main()
{
	Mat kernel = (Mat_<uchar>(5,5) << 1,4,6,4,1,
									  4,16,24,16,4,
									  6,24,36,24,6,
									  4,16,24,16,4,
									  1,4,6,4,1);
	Mat image = imread("lena.png",IMREAD_GRAYSCALE);
	Mat dst1 = Mat::zeros(image.size(), CV_8UC1);
	Mat dst2 = Mat::zeros(image.size(), CV_8UC1);
	double t = (double)getTickCount();
	for(int i = 2; i < image.cols - 2; i++)
	{
		for(int j = 2; j < image.rows - 2; j++)
		{
			uchar *color_ptemp = dst1.ptr<uchar>(j);
			for(int m = -2; m <= 2; m++)
			{
				for(int n = -2; n <= 2; n++)
				{
					color_ptemp[i] += 1.0 * image.at<uchar>(j + n,i + m) * kernel.at<uchar>(n + 2, m + 2) / 256;

				}

			}
		}
	}
	t = ((double)getTickCount() - t)/getTickFrequency();
	cout << "1 " << t << endl;
	imshow("dst1",dst1);


	
	Mat kernel1 = (Mat_<uchar>(1,5) << 1,4,6,4,1);
	Mat kernel2 = kernel1.t();//kernel1 的转置
	t = (double)getTickCount();
	for(int i = 2; i < image.cols - 2; i++)
	{
		for(int j = 2; j < image.rows - 2; j++)
		{
			uchar *color_ptemp = dst2.ptr<uchar>(j);
			
			for(int m = -2; m <= 2; m++)
			{
				uchar h1 = kernel1.at<uchar>(0, m + 2);
				int color = 0;
				for(int n = -2; n <= 2; n++)
				{
					color += image.at<uchar>(j + n,i + m) * kernel2.at<uchar>(n + 2, 0);

				}
				color_ptemp[i] += h1 * color / 256;

			}
		}
	}


	t = ((double)getTickCount() - t)/getTickFrequency();
	cout << "2 " << t << endl;
	imshow("dst2",dst2);
	waitKey();

}
