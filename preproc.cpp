#include "stdafx.h"

using namespace cv;
using namespace std;

void FindROI(Mat const SourceImage, Mat& ROIImage)
{
	//��ֵ��
	Mat binaryImage;
	threshold(SourceImage, binaryImage, 191, 255, CV_THRESH_TRIANGLE);

	namedWindow("��ֵ��ͼ��");
	imshow("��ֵ��ͼ��", binaryImage);

	//�������
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(binaryImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(FIND_ROI_CUT, FIND_ROI_CUT));

	//�ڵ�һ����Ѱ��������Ԫ��
	vector<Point> approxContours, targetApproxContours;
	int index = hierarchy.at(0)(2);
	//int maxIndex = -1;
	double area = 0, maxArea = 0;
	while (index >= 0)
	{
		area = contourArea(contours.at(index));
		if (area > maxArea)
		{
			//��������
			approxPolyDP(contours.at(index), approxContours, arcLength(contours.at(index), true)*0.01, true);
			if (approxContours.size() == 4)
			{
				maxArea = area;
				//maxIndex = index;
				targetApproxContours = approxContours;
			}
		}
		index = hierarchy.at(index)(0);
	}
	if (maxArea < 1)
	{
		throw logic_error("Can't find right contours.");
	}
	
	//����
	int distance = 0, maxDistance = 0;
	index = 0;
	//Ѱ�����Ͻ�Ԫ��
	maxDistance = targetApproxContours.at(0).x + targetApproxContours.at(0).y;
	for (int ii = 1; ii < 4; ii++)
	{
		distance = targetApproxContours.at(ii).x + targetApproxContours.at(ii).y;
		if (distance < maxDistance)
		{
			maxDistance = distance;
			index = ii;
		}
	}
	//����
	Point2f vertices[4];
	for (int ii = 0; ii < 4; ii++)
	{
		vertices[ii] = targetApproxContours.at((ii + index) % 4);
	}


	//����任
	Point2f transedVertices[4];
	transedVertices[0] = Point2f(TRANSFORMED_MARGIN, TRANSFORMED_MARGIN);
	transedVertices[1] = Point2f(TRANSFORMED_SIZE + TRANSFORMED_MARGIN - 1, TRANSFORMED_MARGIN);
	transedVertices[2] = Point2f(TRANSFORMED_SIZE + TRANSFORMED_MARGIN - 1, TRANSFORMED_SIZE + TRANSFORMED_MARGIN - 1);
	transedVertices[3] = Point2f(TRANSFORMED_MARGIN, TRANSFORMED_SIZE + TRANSFORMED_MARGIN - 1);
	Mat transformMatrix = getPerspectiveTransform(vertices, transedVertices);
	Mat warpedImage = Mat::zeros(Size(TRANSFORMED_SIZE + 2*TRANSFORMED_MARGIN, TRANSFORMED_SIZE + 2*TRANSFORMED_MARGIN), CV_8UC1);
	warpPerspective(SourceImage, warpedImage, transformMatrix, warpedImage.size(), INTER_CUBIC);


	//��������
	Mat contoursImage = Mat::zeros(binaryImage.size(), CV_8UC3);
	//drawContours(contoursImage, contours, maxIndex, Scalar(128, 255, 255), 1, LINE_AA, hierarchy, 0);
	for (int i = 0; i < 4; i++)
		line(contoursImage, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0));

	//��ʾ����
	namedWindow("�������");
	imshow("�������", warpedImage);//contoursImage);
	waitKey();

}
