#include "stdafx.h"

using namespace cv;
using namespace std;

int FindROI(Mat const SourceImage, Point2f *vertices)
{
	Mat tempImage1;
	Mat targetImage;
	//形态学底帽均匀背景
	int eigenlen = SourceImage.cols / 70;
	morphologyEx(SourceImage, targetImage, MORPH_BLACKHAT, getStructuringElement(MORPH_ELLIPSE, Size(eigenlen, eigenlen)));

	//形态学膨胀运算
	morphologyEx(targetImage, tempImage1, MORPH_DILATE, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));

	//二值化
	threshold(tempImage1, targetImage, 191, 255, CV_THRESH_OTSU);

	//显示二值化和形态学膨胀图像
	/*namedWindow("二值化图像", WINDOW_NORMAL);
	imshow("二值化图像", binaryImage);*/
	/*namedWindow("膨胀图像", WINDOW_NORMAL);
	imshow("膨胀图像", targetImage);
	waitKey();*/

	//检测轮廓
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(targetImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	/*//绘制轮廓
	Scalar color(0, 255, 0);
	namedWindow("轮廓图像", WINDOW_NORMAL);
	for (int idx=0; idx >= 0; idx = hierarchy[idx][0])
	{
	Mat dst = Mat::zeros(SourceImage.rows, SourceImage.cols, CV_8UC3);
	drawContours(dst, contours, idx, color, FILLED, 8, hierarchy);
	imshow("轮廓图像", dst);
	//waitKey();
	}*/



	//寻找最大面积的四边形元素
	int index = InterestContour(contours, hierarchy, 75);
	if (index < 0)
	{
		throw logic_error("Can't find right contours.");
	}

	//多边形拟合
	vector<Point> approxContours;
	vector<Point> temp;
	convexHull(contours.at(index), temp);
	approxPolyDP(temp, approxContours, arcLength(contours.at(index), true)*0.02, true);

	//角点排序
	SortVertex(approxContours);
	/*int distance = 0, maxDistance = 0;
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
	}*/
	vertices[0] = approxContours.at(0);// -Point(FIND_ROI_CUT, FIND_ROI_CUT);
	vertices[1] = approxContours.at(1);// - Point(FIND_ROI_CUT, FIND_ROI_CUT);
	vertices[2] = approxContours.at(2);// - Point(FIND_ROI_CUT, FIND_ROI_CUT);
	vertices[3] = approxContours.at(3);// - Point(FIND_ROI_CUT, FIND_ROI_CUT);

	
	/*//画出轮廓
	Mat blimg[3];
	blimg[0] = SourceImage;
	blimg[1] = SourceImage;
	blimg[2] = SourceImage;
	Mat contoursImage = Mat::zeros(targetImage.size(), CV_8UC3);
	merge(blimg, 3, contoursImage);
	//drawContours(contoursImage, contours, maxIndex, Scalar(128, 255, 255), 1, LINE_AA, hierarchy, 0);
	for (int i = 0; i < 4; i++)
		line(contoursImage, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0));
	//显示轮廓
	namedWindow("轮廓检测", CV_WINDOW_NORMAL);
	imshow("轮廓检测", contoursImage);
	waitKey();*/

	return 0;
}

//从开始时针排序
void SortVertex(vector<Point> &SourceVertex)
{
	double center_x = 0, center_y = 0;
	for (vector<Point>::iterator it = SourceVertex.begin(); it != SourceVertex.end(); it++)
	{
		center_x += it->x;
		center_y += it->y;
	}
	center_x /= SourceVertex.size();
	center_y /= SourceVertex.size();

	sort(SourceVertex.begin(), SourceVertex.end(), 
		[center_x, center_y](Point a, Point b)
	{
		double a_x = a.x - center_x;
		double a_y = a.y - center_y;
		double b_x = b.x - center_x;
		double b_y = b.y - center_y;
		double a_len = sqrt(a_x*a_x + a_y*a_y);
		double b_len = sqrt(b_x*b_x + b_y*b_y);
		a_x = (a_x / a_len - 1.5) * (a_y > 0 ? -1 : 1);
		b_x = (b_x / b_len - 1.5) * (b_y > 0 ? -1 : 1);
		return (a_x < b_x);
	});
}
