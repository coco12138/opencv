#include <opencv2\opencv.hpp>
#include <iostream>
#include <time.h>
using namespace std;
using namespace cv;

int myOtsu(Mat & src)
{
	int th;
	const int GrayScale = 256;	//单通道图像总灰度256级
	int pixCount[GrayScale] = {0};//每个灰度值所占像素个数
	int pixSum = src.cols * src.rows;//图像总像素点
	float pixPro[GrayScale] = {0};//每个灰度值所占总像素比例
	float w0, w1, u0tmp, u1tmp, u0, u1, deltaTmp, deltaMax = 0; 

	for(int i = 0; i < src.cols; i++)
	{
		for(int j = 0; j < src.rows; j++)
		{
			pixCount[src.at<uchar>(j,i)]++;//统计每个灰度级中像素的个数  
		}
	}

	for(int i = 0; i < GrayScale; i++)
	{
		pixPro[i] = pixCount[i] * 1.0 / pixSum;//计算每个灰度级的像素数目占整幅图像的比例  
	}

	for(int i = 0; i < GrayScale; i++)//遍历所有从0到255灰度级的阈值分割条件，测试哪一个的类间方差最大
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = deltaTmp = 0;  
		for(int j = 0; j < GrayScale; j++)
		{
			if(j <= i)//背景
			{
				w0 += pixPro[j];
				u0tmp += j * pixPro[j]; 
			}
			else//前景
			{
				w1 += pixPro[j];
				u1tmp += j * pixPro[j];
			}
		}
		u0 = u0tmp / w0;
		u1 = u1tmp / w1;
		deltaTmp = (float)(w0 *w1* pow((u0 - u1), 2)); //类间方差公式 g = w1 * w2 * (u1 - u2) ^ 2
		if(deltaTmp > deltaMax)  
		{  
             deltaMax = deltaTmp;  
             th = i;  
        }  
	}
	return th;
}

int main()
{
	Mat src = imread("lena.jpg",IMREAD_GRAYSCALE);//单通道读取图像
	/*my_dst: 自己实现的大津法 得到的处理图像
	otsu_dst：opencv自带的大津法 得到的处理图像
	sub：两个处理图像相差图
	*/
	Mat my_dst, otsu_dst, sub;							
	/*my_th: 自己实现的大津法 得到的最大类件方差 即阈值
	th：opencv自带的大津法 得到的最大类件方差 即阈值
	*/
	int my_th, th;

	/*计算开销时间，对比两个算法效率*/
	long my_start = clock();  //开始时间
    {
		my_th = myOtsu(src);
		threshold(src,my_dst,my_th,255,CV_THRESH_BINARY);
    }
    long my_finish = clock();   //结束时间
    long my_t = my_finish-my_start;
    printf("The run time is:%9.3lf\n", my_t, "ms!\n"); //输出时间
	cout << "myOtsu threshold >> " << my_th << endl;
	
	long otsu_start = clock();  //开始时间
    {
		th = threshold(src,otsu_dst,0,255,CV_THRESH_OTSU);
    }
    long otsu_finish = clock();   //结束时间
    long t = my_finish-my_start;
    printf("The run time is:%9.3lf\n", t, "ms!\n"); //输出时间
	cout << "Otsu threshold >> " << th << endl;

	subtract(otsu_dst,my_dst,sub);//两图像相减
	imshow("src",src);
	imshow("myOtsu",my_dst);
	imshow("Otsu",otsu_dst);
	imshow("Sub",sub);
	waitKey();
	system("pause");
	return 0;
}
