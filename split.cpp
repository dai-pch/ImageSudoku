#include "stdafx.h"

using namespace cv;
using namespace std;

/*vector<Mat>*/void splitImage(Mat const SourceImage)
{
	//二值化
	Mat binaryImage;
	threshold(SourceImage, binaryImage, 191, 255, CV_THRESH_OTSU);

	/*//显示二值化图像
	namedWindow("二值化图像");
	imshow("二值化图像", binaryImage);*/

	//检测轮廓
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(binaryImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	//寻找矩形外框
	int index = -1; //hierarchy.at(0)(2);
	index = InterestContour(contours, hierarchy, 81);
	if (index < 0)
	{
		throw logic_error("Can't find right contours.");
	}

	//提取出子边界


}
