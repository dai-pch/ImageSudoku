// ImageSudoku.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

using namespace std;
using namespace cv;

int _tmain(int argc, _TCHAR* argv[])
{
	string fileName;
	Mat image;

	//cout << "Please input path and name of the image file:" << endl;
	//cin >> filename;
	fileName = "testimage.png";
	image = imread(fileName, 0);

	if (image.empty())
	{
		cout << "Can't open the file." << endl;
		return 0;
	}

	//显示原始灰度图像
	namedWindow("原始图像");
	imshow("原始图像",image);

	//寻找网格区域
	Mat targetImage;
	targetImage = FindROI(image);

	//显示寻找到的区域
	namedWindow("ROI");
	imshow("ROI", targetImage);

	splitImage(targetImage);



	waitKey();

	return 0;
}
