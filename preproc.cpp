#include "stdafx.h"

using namespace cv;
using namespace std;

Mat FindROI(Mat const SourceImage)
{
	//二值化
	Mat binaryImage;
	threshold(SourceImage, binaryImage, 191, 255, CV_THRESH_TRIANGLE);

	/*//显示二值化图像
	namedWindow("二值化图像");
	imshow("二值化图像", binaryImage);*/

	//检测轮廓
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(binaryImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(FIND_ROI_CUT, FIND_ROI_CUT));

	//寻找最大面积、有足够子边界和四边形元素
	int index = 0; //hierarchy.at(0)(2);
	index = InterestContour(contours, hierarchy, 75);
	if (index < 0)
	{
		throw logic_error("Can't find right contours.");
	}

	//多边形拟合
	vector<Point> approxContours;
	approxPolyDP(contours.at(index), approxContours, arcLength(contours.at(index), true)*0.02, true);
	
	//角点排序
	int distance = 0, maxDistance = 0;
	index = 0;
	//寻找左上角元素
	maxDistance = approxContours.at(0).x + approxContours.at(0).y;
	for (int ii = 1; ii < 4; ii++)
	{
		distance = approxContours.at(ii).x + approxContours.at(ii).y;
		if (distance < maxDistance)
		{
			maxDistance = distance;
			index = ii;
		}
	}
	//进行排序
	Point2f vertices[4];
	for (int ii = 0; ii < 4; ii++)
	{
		vertices[ii] = approxContours.at((ii + index) % 4);
	}

	/*
	//画出轮廓
	Mat contoursImage = Mat::zeros(binaryImage.size(), CV_8UC3);
	//drawContours(contoursImage, contours, maxIndex, Scalar(128, 255, 255), 1, LINE_AA, hierarchy, 0);
	for (int i = 0; i < 4; i++)
		line(contoursImage, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0));
	//显示轮廓
	namedWindow("轮廓检测");
	imshow("轮廓检测", contoursImage);
	waitKey();
	*/

	//仿射变换
	Point2f transedVertices[4];
	transedVertices[0] = Point2f(TRANSFORMED_MARGIN, TRANSFORMED_MARGIN);
	transedVertices[1] = Point2f(TRANSFORMED_SIZE + TRANSFORMED_MARGIN - 1, TRANSFORMED_MARGIN);
	transedVertices[2] = Point2f(TRANSFORMED_SIZE + TRANSFORMED_MARGIN - 1, TRANSFORMED_SIZE + TRANSFORMED_MARGIN - 1);
	transedVertices[3] = Point2f(TRANSFORMED_MARGIN, TRANSFORMED_SIZE + TRANSFORMED_MARGIN - 1);
	Mat transformMatrix = getPerspectiveTransform(vertices, transedVertices);
	Mat warpedImage = Mat::zeros(Size(TRANSFORMED_SIZE + 2*TRANSFORMED_MARGIN, TRANSFORMED_SIZE + 2*TRANSFORMED_MARGIN), CV_8UC1);
	warpPerspective(SourceImage, warpedImage, transformMatrix, warpedImage.size(), INTER_CUBIC);

	return warpedImage;
}
