#if 1

#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
using namespace std;
using namespace cv;
void main()
{
	Mat image = imread("lena.png",IMREAD_GRAYSCALE);
	Mat x_d = Mat::zeros(image.size(), CV_8UC1); //x偏导
	Mat y_d = x_d.clone();			//y偏导
	int detX = 2;
	int detY = 2;
	Mat dst = Mat::zeros( image.size(), CV_32FC1 );
	Mat dst_norm,dst_norm_scaled;
	cornerHarris(image,dst,2,3,0.04);
	normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
	convertScaleAbs( dst_norm, dst_norm_scaled );
	imshow("image",image);
	GaussianBlur(image,image,Size(3,3),1.2,1.2);
	for(int j = 0; j < image.rows; j++)
	{
		uchar *color_ptemp = x_d.ptr<uchar>(j);
		for(int i = 0; i < image.cols - detX; i++)
		{
			color_ptemp[i] = abs((double)(image.at<uchar>(j, i + detX) - image.at<uchar>(j, i)));
		}
	}

	for(int i = 0; i < image.cols; i++)
	{
		
		for(int j = 0; j < image.rows - detY; j++)
		{
			uchar *color_ptemp = y_d.ptr<uchar>(j);
			color_ptemp[i] = abs((double)(image.at<uchar>(j + detY, i) - image.at<uchar>(j, i)));
		}
	}
	imshow("x_d",x_d);
	imshow("y_d",y_d);
	//waitKey();
	
	
	Mat S = Mat::zeros(image.size(), CV_64FC1);//原图和平移后图片的差值的平方和
	Mat R = Mat::zeros(image.size(),CV_64FC1);//响应函数
	double k = 0.04;
	Mat A = Mat::zeros(2,2,CV_64FC1);
	Mat det_multiply_A = Mat::zeros(1,2,CV_64FC1);
	double S_max = 0, R_max = 0;
	for(int i = 0; i < image.cols - detX; i++)
	{
		for(int j = 0; j < image.rows - detY; j++)
		{
			
			
			A.setTo(0);
			//det_multiply_A.setTo(0);
			for(int x = 0; x <= detX; x++)
			{
				for(int y = 0; y <= detY; y++)
				{
					//计算局部结构矩阵A
					A.ptr<double>(0)[0] += x_d.at<uchar>(j + y, i + x) * x_d.at<uchar>(j + y, i + x);
					A.ptr<double>(0)[1] += x_d.at<uchar>(j + y, i + x) * y_d.at<uchar>(j + y, i + x);
					A.ptr<double>(1)[0] += x_d.at<uchar>(j + y, i + x) * y_d.at<uchar>(j + y, i + x);
					A.ptr<double>(1)[1] += y_d.at<uchar>(j + y, i + x) * y_d.at<uchar>(j + y, i + x);
					//cout << A << endl;
					
				}

			}
			det_multiply_A.ptr<double>(0)[0] = detX * A.ptr<double>(0)[0] + detY * A.ptr<double>(0)[1];
			det_multiply_A.ptr<double>(0)[1] = detX * A.ptr<double>(1)[0] + detY * A.ptr<double>(1)[1];

			S.ptr<double>(j)[i] = detX * det_multiply_A.ptr<double>(0)[0] + detY * det_multiply_A.ptr<double>(0)[1];
			S_max = S_max < S.ptr<double>(j)[i] ? S.ptr<double>(j)[i] : S_max;
			double end = determinant(A);
			double trace_ = trace(A)[0];
			double s = end - k * trace_ * trace_;
			R.ptr<double>(j)[i] = s;
			R_max = R_max < R.ptr<double>(j)[i] ? R.ptr<double>(j)[i] : R_max;
			//cout << s << endl;
		}

	}
	imshow("R",R);
	// <0 为边缘 偏小为平坦区域，偏大为角点
	//进行非极大抑制
	Mat Sc = Mat::zeros(image.size(),CV_8UC1);
	Mat Rc = Mat::zeros(image.size(),CV_8UC1);
	for(int i = 1; i < image.cols - 1; i++)
	{
		for(int j = 1; j < image.rows - 1; j++)
		{
			Sc.ptr<uchar>(j)[i] = (uchar)(S.ptr<double>(j)[i] / S_max * 255);//归一化
			if(R.ptr<double>(j)[i] < 0)
			{
				Rc.ptr<uchar>(j)[i] = 0;
				continue;
			}
			else
				Rc.ptr<uchar>(j)[i] = (uchar)(R.ptr<double>(j)[i] / R_max * 255);//归一化
			int flag = 0;
			for(int x = -1; x <= 1; x++)
			{
				for(int y = -1; y <= 1; y++)
				{
					if(R.ptr<double>(j)[i] < R.ptr<double>(j + y)[i + x])
					{
						Rc.ptr<uchar>(j)[i] = 0;
						flag = 1;
						break;
					}
				}
				if(flag)
					break;

			}
			
		}
	}

	
		
	
	imshow("dst",dst_norm_scaled);
	imshow("Sc",Sc);
	imshow("Rc",Rc);
	imwrite("Rc.bmp",Rc);
	waitKey();

}

#endif
