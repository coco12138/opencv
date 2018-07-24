#include<opencv2\opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;
typedef struct SmaxValue
{
	int nValue;
	int nDist;
	int nAngle;
}SMaxValue;
void Hough(Mat & src, int ** result, int nLineRet)
{
	int nMaxDist = sqrt(src.cols * src.cols * 1.0 + src.rows * src.rows);
	int nMaxAngle = 90;
	//为极坐标分配空间
	int nAreaNum = nMaxAngle * (nMaxDist + 1) * 2;
	int * pTransArea = new int[nAreaNum];
	memset(pTransArea, 0, nAreaNum * sizeof(int));

	//转化到极坐标域
	int nAngle, nDist;
	double fRad;
	for(int i = 0; i < src.rows; i++)
	{
		int val;
		for (int j = 0; j < src.cols; j++)
		{
			val = src.at<uchar>(i, j);
			if (val == 255)
			{
				for (nAngle = 0; nAngle < nMaxAngle; nAngle++)
				{
					fRad = nAngle * 2 * CV_PI / 180.0;//转为0~180°
					nDist = (j * cos(fRad) + i * sin(fRad));

					if (nDist >= 0)
						pTransArea[nDist * nMaxAngle + nAngle]++;
					else
					{
						nDist = -nDist;
						pTransArea[(nMaxDist + nDist + 1) * nMaxAngle + nAngle]++;
					}
				}//for nAngle
			}//if
		}//for j
	}//for i
	//清零时角度和极径的范围
	int nMaxDistAllow = 20;
	int nMaxAngleAllow = 5;
	SMaxValue MaxValue1;
	
	(*result) = new int[3 * nLineRet];//存储距离，角度，灰度值

	for (int nLine = 0; nLine < nLineRet; nLine++)
	{
		MaxValue1.nValue = 0;
		for (int i = 0; i < nAreaNum; i++)
		{
			if (pTransArea[i] > MaxValue1.nValue)
			{
				MaxValue1.nValue = pTransArea[i];
				MaxValue1.nAngle = i;
			}
		}
		if (MaxValue1.nValue == 0)
			return;
		if (MaxValue1.nAngle < nMaxAngle * (nMaxDist + 1))
		{
			MaxValue1.nDist = MaxValue1.nAngle / nMaxAngle;
			MaxValue1.nAngle = MaxValue1.nAngle % nMaxAngle;
		}
		else
		{
			MaxValue1.nAngle -= nMaxAngle * (nMaxDist + 1);
			MaxValue1.nDist *= -MaxValue1.nAngle / nMaxAngle;
			MaxValue1.nAngle = MaxValue1.nAngle % nMaxAngle;
		}
		(*result)[nLine * 3] = MaxValue1.nAngle * 2;
		(*result)[nLine * 3 + 1] = MaxValue1.nDist;
		(*result)[nLine * 3 + 2] = MaxValue1.nValue;
		if ((*result)[nLine * 3 + 1] < 0)//角度大于180的时候 距离<0
		{
			(*result)[nLine * 3] = (*result)[nLine * 3] - 180;
			(*result)[nLine * 3 + 1] = (*result)[nLine * 3 + 1] * (-1);
		}
		for (nDist = (-1) * nMaxDistAllow; nDist < nMaxDistAllow; nDist++)//清除这条线 正负范围nMaxAngleAllow内的角度的线
		{
			for (nAngle = (-1) * nMaxAngleAllow; nAngle < nMaxAngleAllow; nAngle++)
			{
				int nThisDist = MaxValue1.nDist + nDist;
				int nThisAngle = MaxValue1.nAngle + nAngle;
				nThisAngle *= 2;//转化为0~360°
				if (nThisAngle < 0 && nThisAngle >= -180)
				{
					nThisAngle += 180;
					nThisDist *= -1;
				}
				if (nThisAngle >= 180 && nThisAngle < 360)
				{
					nThisAngle -= 180;
					nThisDist *= -1;
				}
				int tmpDist = nThisDist;
				if (tmpDist < 0)
					tmpDist = -tmpDist;
				if (tmpDist <= nMaxDist && nThisAngle >= 0 && nThisAngle <= nMaxAngle * 2)
				{
					nThisAngle /= 2;//转化为极坐标-90~90°
					if (nThisDist >= 0)
					{
						pTransArea[nThisDist * nMaxAngle + nThisAngle] = 0;
					}
					else
					{
						nThisDist = tmpDist;
						pTransArea[(nMaxDist + nThisDist + 1) * nMaxAngle + nThisAngle] = 0;
					}

				}
			}//for nAngle
		}//for nDist
	}//for nLine

	delete []pTransArea;
}
void drawHoughLine(Mat &src,int ** result, int nLineRet)
{
	for (int k = 0; k < nLineRet; k++)
	{
		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				int nDist;
				nDist = (int)(j * cos((*result)[k * 3] * CV_PI / 180.0) + i * sin((*result)[k * 3] * CV_PI / 180.0));
				if (nDist == (*result)[k * 3 + 1])
					src.at<uchar>(i, j) = 255;
			}
		}
	}
}
void main()
{
	Mat src = 255-imread("line.jpg", IMREAD_GRAYSCALE);
	Mat dst = Mat::zeros(src.size(), CV_8UC1);
	Mat hou = dst.clone();
	int (*result) = NULL;
	Hough(src, &result, 3);
	drawHoughLine(dst, &result, 3);
	vector<Vec4i> lines;
	HoughLinesP(src,lines,1.0,	CV_PI/180,255,5,5);
	for(int i = 0; i < lines.size(); i++)
	{
		line(hou,Point(lines[i][0],lines[i][1]),Point(lines[i][2],lines[i][3]),Scalar(255),2);
	}		
	imshow("src", src);
	imshow("dst", dst);
	imshow("hou", hou);
	waitKey();
}
