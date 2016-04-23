#include "stdafx.h"

using namespace cv;
using namespace std;

Mat FindROI(Mat const SourceImage)
{
	//��̬ѧ��ñ���ȱ���
	Mat targetImage;
	int eigenlen = SourceImage.cols / 70;
	morphologyEx(SourceImage, targetImage, MORPH_BLACKHAT, getStructuringElement(MORPH_ELLIPSE, Size(eigenlen, eigenlen)));

	//��ֵ��
	Mat binaryImage;
	threshold(targetImage, binaryImage, 191, 255, CV_THRESH_OTSU);

	//��̬ѧ��������
	morphologyEx(binaryImage, targetImage, MORPH_DILATE, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));

	/*//��ʾ��ֵ������̬ѧ����ͼ��
	namedWindow("��ֵ��ͼ��", WINDOW_NORMAL);
	imshow("��ֵ��ͼ��", binaryImage);
	namedWindow("����ͼ��", WINDOW_NORMAL);
	imshow("����ͼ��", targetImage);
	waitKey();*/

	//�������
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(targetImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(FIND_ROI_CUT, FIND_ROI_CUT));

	/*//��������
	Scalar color(0, 255, 0);
	namedWindow("����ͼ��", WINDOW_NORMAL);
	for (int idx=0; idx >= 0; idx = hierarchy[idx][0])
	{
	Mat dst = Mat::zeros(SourceImage.rows, SourceImage.cols, CV_8UC3);
	drawContours(dst, contours, idx, color, FILLED, 8, hierarchy);
	imshow("����ͼ��", dst);
	//waitKey();
	}*/



	//Ѱ�����������ı���Ԫ��
	int index = 0; //hierarchy.at(0)(2);
	index = InterestContour(contours, hierarchy, 75);
	if (index < 0)
	{
		throw logic_error("Can't find right contours.");
	}

	//��������
	vector<Point> approxContours;
	vector<Point> temp;
	convexHull(contours.at(index), temp);
	approxPolyDP(temp, approxContours, arcLength(contours.at(index), true)*0.02, true);

	//�ǵ�����
	SortVertex(approxContours);
	/*int distance = 0, maxDistance = 0;
	index = 0;
	//Ѱ�����Ͻ�Ԫ��
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
	Point2f vertices[4];
	vertices[0] = approxContours.at(0) - Point(FIND_ROI_CUT - TRANSFORMED_CUT, FIND_ROI_CUT - TRANSFORMED_CUT);
	vertices[1] = approxContours.at(1) - Point(FIND_ROI_CUT + TRANSFORMED_CUT, FIND_ROI_CUT - TRANSFORMED_CUT);
	vertices[2] = approxContours.at(2) - Point(FIND_ROI_CUT + TRANSFORMED_CUT, FIND_ROI_CUT + TRANSFORMED_CUT);
	vertices[3] = approxContours.at(3) - Point(FIND_ROI_CUT - TRANSFORMED_CUT, FIND_ROI_CUT + TRANSFORMED_CUT);

	/*
	//��������
	Mat contoursImage = Mat::zeros(binaryImage.size(), CV_8UC3);
	//drawContours(contoursImage, contours, maxIndex, Scalar(128, 255, 255), 1, LINE_AA, hierarchy, 0);
	for (int i = 0; i < 4; i++)
	line(contoursImage, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0));
	//��ʾ����
	namedWindow("�������");
	imshow("�������", contoursImage);
	waitKey();
	*/

	//����任
	Point2f transedVertices[4];
	transedVertices[0] = Point2f(TRANSFORMED_MARGIN, TRANSFORMED_MARGIN);
	transedVertices[1] = Point2f(TRANSFORMED_SIZE + TRANSFORMED_MARGIN - 1, TRANSFORMED_MARGIN);
	transedVertices[2] = Point2f(TRANSFORMED_SIZE + TRANSFORMED_MARGIN - 1, TRANSFORMED_SIZE + TRANSFORMED_MARGIN - 1);
	transedVertices[3] = Point2f(TRANSFORMED_MARGIN, TRANSFORMED_SIZE + TRANSFORMED_MARGIN - 1);

	Mat transformMatrix = getPerspectiveTransform(vertices, transedVertices);
	Mat warpedImage = Mat::zeros(Size(TRANSFORMED_SIZE + 2 * TRANSFORMED_MARGIN, TRANSFORMED_SIZE + 2 * TRANSFORMED_MARGIN), CV_8UC1);
	warpPerspective(SourceImage, warpedImage, transformMatrix, warpedImage.size(), INTER_CUBIC);

	return warpedImage;
}

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
