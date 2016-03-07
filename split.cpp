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
	for (index = hierarchy.at(index)(3); index >= 0; index = hierarchy.at(index)(0))
	{
		Point2d	contourCenter = center(contours.at(index));
		int number = (int)(contourCenter.x / subimageWidth) + 9 * (int)(contourCenter.y / subimageHight);
		splited.at(number) = SourceImage(ContentsRegion(contours, hierarchy, index));
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

Rect ContentsRegion(vector<vector<Point>> const SourceContours, vector<Vec4i, allocator<Vec4i>> const SourceHierarchy, int const Target)
{
	int index = 0;

	vector<Point> contents;
	for (index = SourceHierarchy.at(Target)(3); index >= 0; index = SourceHierarchy.at(index)(0))
	{
		contents.insert(contents.end(), SourceContours.at(index).begin(), SourceContours.at(index).end());
	}

	return boundingRect(contents);
}
