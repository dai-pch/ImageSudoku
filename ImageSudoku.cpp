// ImageSudoku.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

using namespace std;
using namespace cv;

int _tmain(int argc, _TCHAR* argv[])
{
	string fileName;
	Mat image, image0;

	//cout << "Please input path and name of the image file:" << endl;
	//cin >> filename;
	fileName = "testimage3.png";//"D:\\sudopic\\pic4.jpg";
	image0 = imread(fileName, 0);

	if (image0.empty())
	{
		cout << "Can't open the file." << endl;
		system("pause");
		return 0;
	}
	resize(image0, image, Size(1200, (int)(image0.cols * 1200) / image0.rows), 0, 0, INTER_CUBIC);
	//显示原始灰度图像
	namedWindow("原始图像");
	imshow("原始图像", image);
	waitKey();
	destroyAllWindows();

	//寻找网格区域
	Mat targetImage;
	targetImage = FindROI(image);

	//显示寻找到的区域
	namedWindow("ROI");
	imshow("ROI", targetImage);
	waitKey();
	destroyAllWindows();

	vector<Mat> splitedImage = splitImage(targetImage);
	for (int ii = 0; ii < 81; ii++)
	{
		namedWindow("splitedImage");
		imshow("splitedImage", splitedImage.at(ii));
		waitKey();
		destroyWindow("splitedImage");
	}

	return 0;
}
