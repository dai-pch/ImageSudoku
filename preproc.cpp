#include "stdafx.h"

using namespace cv;
using namespace std;

void FindROI(Mat const SourceImage, Mat& ROIImage)
{
	//二值化
	Mat binaryImage;
	threshold(SourceImage, binaryImage, 191, 255, CV_THRESH_TRIANGLE);

	namedWindow("二值化图像");
	imshow("二值化图像", binaryImage);

	//检测轮廓
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(binaryImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(ROI_INDENT, ROI_INDENT));

	//在第一层中寻找最大面积元素
	int index = hierarchy.at(0)(2);
	int maxIndex = index;
	double area = 0, lastArea = 0;
	while (index >= 0)
	{
		area = contourArea(contours.at(index));
		if (area > lastArea)
		{
			lastArea = area;
			maxIndex = index;
		}
		index = hierarchy.at(index)(0);
	}

	//用矩形拟合
	Point2f vertices[4];
	minAreaRect(contours.at(maxIndex)).points(vertices);
	//排序


	//仿射变换
	Point2f transedVertices[4];
	transedVertices[1] = Point2f(0, 0);
	transedVertices[2] = Point2f(TRANSFORMED_SIZE - 1, 0);
	transedVertices[3] = Point2f(TRANSFORMED_SIZE - 1, TRANSFORMED_SIZE - 1);
	transedVertices[0] = Point2f(0, TRANSFORMED_SIZE - 1);
	Mat transformMatrix = getPerspectiveTransform(vertices, transedVertices);
	Mat warpedImage = Mat::zeros(Size(TRANSFORMED_SIZE, TRANSFORMED_SIZE), CV_8UC1);
	warpPerspective(SourceImage, warpedImage, transformMatrix, warpedImage.size(), INTER_CUBIC);


	//画出轮廓
	Mat contoursImage = Mat::zeros(binaryImage.size(), CV_8UC3);
	//drawContours(contoursImage, contours, maxIndex, Scalar(128, 255, 255), 1, LINE_AA, hierarchy, 0);
	for (int i = 0; i < 4; i++)
		line(contoursImage, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0));

	//显示轮廓
	namedWindow("轮廓检测");
	imshow("轮廓检测", warpedImage);//contoursImage);
	waitKey();

}
