#include "stdafx.h"

using namespace cv;
using namespace std;

/*vector<Mat>*/void splitImage(Mat const SourceImage)
{
	//��ֵ��
	Mat binaryImage;
	threshold(SourceImage, binaryImage, 191, 255, CV_THRESH_OTSU);

	/*//��ʾ��ֵ��ͼ��
	namedWindow("��ֵ��ͼ��");
	imshow("��ֵ��ͼ��", binaryImage);*/

	//�������
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(binaryImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	//Ѱ�Ҿ������
	int index = -1; //hierarchy.at(0)(2);
	index = InterestContour(contours, hierarchy, 81);
	if (index < 0)
	{
		throw logic_error("Can't find right contours.");
	}

	//��ȡ���ӱ߽�


}
