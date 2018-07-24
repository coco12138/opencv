#include<opencv2\opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;

void wallner(Mat & src, Mat & dst)
{
	/*
	* pn = 当前点的灰度值
	* s = 图片宽度/n （n = 8时效果最好）
	* t = 比例阈值
	* 公式：g(n) = g(n-1) * (1-1/s) + p(n)
	*/
	int t = 15;
	int s = src.cols >> 3;
	const int S = 9;
	const int power2S = 1 << S;//加速因子
	int factor = power2S * (100 - t) / (100 * s);
	/*使用初始值127*s *s是因为 原先算法采用均值也就是fn 是前n个像素之和 
	这次算法优化为与当前点越相邻对其影响越大的思路*/
	int gn = 127 * s;
	int q = power2S - power2S / s;
	int pn, hn;
	int *prev_gn = NULL;//前一行各点像素值
	//Mat dst = Mat::zeros(src.size(), CV_8UC1);
	prev_gn = new int[src.cols];
	for (int i = 0; i < src.cols; i++)
		prev_gn[i] = gn;
	uchar * scanline = NULL;
	for (int i = 0; i < src.rows; i++)
	{
		scanline = src.ptr<uchar>(i);
		for (int j = 0; j < src.cols; j++)//从左向右遍历
		{
			pn = scanline[j];
			gn = ((gn * q) >> S) + pn;
			hn = (gn + prev_gn[j]) >> 1;
			prev_gn[j] = gn;
			pn < (hn * factor) >> S ? dst.at<uchar>(i, j) = 0 : dst.at<uchar>(i, j) = 255;
		}
		i++;
		if (i == src.rows)
			break;
		scanline = src.ptr<uchar>(i);
		for (int j = src.cols - 1; j >= 0; j--)//从右向左遍历
		{
			pn = scanline[j];
			gn = ((gn * q) >> S) + pn;
			hn = (gn + prev_gn[j]) >> 1;
			prev_gn[j] = gn;
			pn < (hn * factor) >> S ? dst.at<uchar>(i, j) = 0 : dst.at<uchar>(i, j) = 255;
		}
	}
}
void main()
{
	Mat src = imread("test.bmp", IMREAD_GRAYSCALE);
	Mat dst = Mat::zeros(src.size(), CV_8UC1);
	wallner(src, dst);
	imshow("src", src);
	imshow("dst", dst);
	imwrite("dst.bmp",dst);
	waitKey();
}
