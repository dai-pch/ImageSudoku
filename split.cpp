#include "stdafx.h"

using namespace cv;
using namespace std;

vector<Mat> splitImage(Mat const SourceImage)
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
	vector<Mat> splited(81);
	int subimageWidth = SourceImage.cols / 9, subimageHight = SourceImage.rows / 9;
	for (index = hierarchy.at(index)(2); index >= 0; index = hierarchy.at(index)(0))
	{
		Point2d	contourCenter = center(contours.at(index));
		int number = (int)(contourCenter.x / subimageWidth) + 9 * (int)(contourCenter.y / subimageHight);
		Rect region = ContentsRegion(contours, index);
		splited.at(number) = SourceImage(region);
	}

	return splited;
}

//Calculate the center of a set of points.
Point2d center(const vector<Point> SetOfPoint)
{
	Point2d centerPoint;
	for (vector<Point>::const_iterator it = SetOfPoint.begin(); it != SetOfPoint.end(); it++)
	{
		centerPoint.x += it->x;
		centerPoint.y += it->y;
	}
	centerPoint.x /= SetOfPoint.size();
	centerPoint.y /= SetOfPoint.size();

	return centerPoint;
}

Rect ContentsRegion(vector<vector<Point>> const SourceContours, int const Target)
{
	vector<Point> const &contents = SourceContours.at(Target);
	Point2d centerPosition = center(contents);
	int radio = 0;
	vector<Point>::const_iterator it = contents.begin();
	double radio_ = fmax(fabs(it->x - centerPosition.x), fabs(it->y - centerPosition.y));
	radio = (int)radio_;
	for (it++; it != contents.end(); it++)
	{
		radio_ = fmax(fabs(it->x - centerPosition.x), fabs(it->y - centerPosition.y));
		radio = (int)fmin(radio_, radio);
	}
	radio -= 1;
	
	return Rect((int)centerPosition.x - radio, (int)centerPosition.y - radio, 2 * radio + 1, 2 * radio + 1);
}
