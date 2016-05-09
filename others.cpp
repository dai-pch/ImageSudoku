#include "stdafx.h"

using namespace cv;
using namespace std;

//寻找某一层边界数量
int contoursNumber(vector<Vec4i, allocator<Vec4i>> const SourceHierarchy, int Index, int Level)
{
	while (Level > 0 && Index >= 0)
	{
		Index = SourceHierarchy.at(Index)(2);
		Level += -1;
	}
	while (Level < 0 && Index >= 0)
	{
		Index = SourceHierarchy.at(Index)(3);
		Level += 1;
	}
	if (Index < 1)
		return 0;

	int number = -1;
	int ii = Index;
	while (ii>=0)
	{
		ii = SourceHierarchy.at(ii)(1);
		number += 1;
	}
	ii = Index;
	while (ii>=0)
	{
		ii = SourceHierarchy.at(ii)(0);
		number += 1;
	}
	return number;
}

//寻找最大面积的四边形元素
int InterestContour(vector<vector<Point>> const SourceContours, vector<Vec4i, allocator<Vec4i>> const SourceHierarchy, int const Number_Threshold)
{
	vector<Point> approxContours;
	unsigned int index = 0; //hierarchy.at(0)(2);
	int maxIndex = -1;
	double area = 0, maxArea = 0;
	for (index = 0; index < SourceContours.size(); index++)
	{
		vector<Point> convexhull;
		convexHull(SourceContours.at(index), convexhull);
		area = contourArea(convexhull);
		if (area > maxArea)//&& contoursNumber(SourceHierarchy, index, 1) >= Number_Threshold)
		{
			//多边形拟合
			approxPolyDP(convexhull, approxContours, arcLength(SourceContours.at(index), true)*0.02, true);
			if (approxContours.size() == 4)//&& isContourConvex(approxContours))
			{
				maxArea = area;
				maxIndex = index;
			}
		}
	}

	return maxIndex;
}

Mat ContrastStretch(Mat const SourceImage, double min)
{
	Mat hist;
	int histSize = 256;
	calcHist(&SourceImage, 1, 0, Mat(), hist, 1, &histSize, 0);
	//cout << hist << endl;

	int data_min = 0, data_max = 255;
	int accumulate = 0, minValue = SourceImage.rows * SourceImage.cols * min;
	for (accumulate = 0; data_min < histSize; data_min++)
	{
		accumulate += hist.at<float>(data_min);
		if (accumulate > minValue) break;
	}
	
	for (accumulate = 0; data_max >= 0; data_max--)
	{
		accumulate += hist.at<float>(data_max);
		if (accumulate > minValue) break;
	}
	//cout << data_min << " " << data_max << endl;

	Mat lookUp(1, 256, CV_8U);
	int len = data_max - data_min;

	if (len < 1) return SourceImage;

	for (int i = 0; i<256; i++) {
		if (i < data_min) lookUp.at<uchar>(i) = 0;
		else if (i > data_max) lookUp.at<uchar>(i) = 255;
		else lookUp.at<uchar>(i) = static_cast<uchar>(255.0*(i - data_min) / len);
	}

	Mat resultImage;
	LUT(SourceImage, lookUp, resultImage);
	return resultImage;
}
