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

	namedWindow("原始图像");
	imshow("原始图像",image);

	Mat targetImage;
	FindROI(image, targetImage);

	return 0;
}
