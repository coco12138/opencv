#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;
#define INV		100 
bool range(Mat &im, int x, int y)//judge the point(i,j) is in the range of image
{
	if(x >= 0 && x < im.cols && y >= 0 && y < im.rows)
		return true;
	return false;
}

template<typename T>
T min_(T n1, T n2)//get the min number between two number
{
	return n1 < n2 ? n1 : n2;

}
/******************************
function:
DistTransfer_AL and DistTransfer_BR

which both use D4(city block) distance method 

else:
DE means : Euclidean distance
D8 means : chessboard distance
******************************/
void DistTransfer_AL(Mat &src, int i, int j)//distance transfer by left matrix 
{
	uchar NW,W,SW,N;
	uchar *p_pcolor = src.ptr<uchar>(j);

	if(range(src, i - 1, j - 1))
	{
		NW = src.at<uchar>(j - 1,i - 1);
		p_pcolor[i] = min_(p_pcolor[i],  (uchar)(NW + 2));
	}
	if(range(src, i - 1, j))
	{
		W = src.at<uchar>(j ,i - 1);
		p_pcolor[i] = min_(p_pcolor[i],  (uchar)(W + 1));
	}
	if(range(src, i - 1, j + 1))
	{
		SW = src.at<uchar>(j + 1,i - 1);
		p_pcolor[i] = min_(p_pcolor[i],  (uchar)(SW + 2));
	}
	if(range(src, i, j - 1))
	{
		N = src.at<uchar>(j - 1, i);
		p_pcolor[i] = min_(p_pcolor[i],  (uchar)(N + 1));
	}

}

void DistTransfer_BR(Mat &src, int i, int j)//distance transfer by right matrix 
{
	uchar NE,E,SE,S;
	uchar *p_pcolor = src.ptr<uchar>(j);

	if(range(src, i + 1, j - 1))
	{
		NE = src.at<uchar>(j - 1,i + 1);
		p_pcolor[i] = min_(p_pcolor[i],  (uchar)(NE + 2));
	}
	if(range(src, i + 1, j))
	{
		E = src.at<uchar>(j ,i + 1);
		p_pcolor[i] = min_(p_pcolor[i],  (uchar)(E + 1));
	}
	if(range(src, i + 1, j + 1))
	{
		SE = src.at<uchar>(j + 1,i + 1);
		p_pcolor[i] = min_(p_pcolor[i],  (uchar)(SE + 2));
	}
	if(range(src, i, j + 1))
	{
		S = src.at<uchar>(j + 1, i);
		p_pcolor[i] = min_(p_pcolor[i],  (uchar)(S + 1));
	}


}
void DistTransfer(Mat &image)
{
	Mat D = Mat::zeros(image.size(), CV_8UC1);
	for(int i = 0; i < image.cols; i++)
	{
		for(int j = 0; j < image.rows; j++)
		{
			uchar * color_ptemp = D.ptr<uchar>(j);
			if(image.at<uchar>(j, i) == 1)// set front ground to zero 
			{			
				color_ptemp[i] = 0;
			}
			else
			{
				color_ptemp[i] = INV; // set background to INV 
			}

		}
	}

	for(int i = 0; i < image.cols; i++) 
	{
		for(int j = 0; j < image.rows; j++)
		{
			DistTransfer_AL(D,i,j);
		}
	}

	for(int i = image.cols - 1; i >= 0; i--) 
	{
		for(int j = image.rows - 1; j >= 0; j--)
		{
			DistTransfer_BR(D,i,j);//behind DistTransfet_AL function
		}
	}
	cout << D << endl;
}


void main()
{
	Mat src = (Mat_<uchar>(8,8) << 0, 0, 0, 0, 0, 0, 1, 0,
								   0, 0, 0, 0, 0, 1, 0, 0,
								   0, 0, 0, 0, 0, 1, 0, 0,
								   0, 0, 0, 0, 0, 1, 0, 0,
								   0, 1, 1, 0, 0, 0, 1, 0,
								   0, 1, 0, 0, 0, 0, 0, 1,
								   0, 1, 0, 0, 0, 0, 0, 0,
								   0, 1, 0, 0, 0, 0, 0, 0);
	cout << src << endl << endl;
	DistTransfer(src);

	system("pause");
}
