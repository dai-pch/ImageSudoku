#ifndef OTHERS_H
#define OTHERS_H

#include "stdafx.h"

using namespace cv;
using namespace std;

//Ѱ��ĳһ��߽�����
int contoursNumber(vector<Vec4i, allocator<Vec4i>> const SourceHierarchy,
					int Index, int Level);

//Ѱ�������������㹻�ӱ߽���ı���Ԫ��
int InterestContour(vector<vector<Point>> const SourceContours, 
					vector<Vec4i, allocator<Vec4i>> const SourceHierarchy,
					int const Number_Threshold);

//�Աȶ�����
Mat ContrastStretch(Mat const SourceImage, double min);

#endif
