#ifndef PRE_PROC_H
#define PRE_PROC_H

#include "stdafx.h"

using namespace cv;
using namespace std;

#define FIND_ROI_CUT 5 //��ԭͼ����Ѱ�Ҿ���ʱ�������ǵı߽粿�ֿ��
#define TRANSFORMED_SIZE 540 //�任Ϊ�����εı߳�
#define TRANSFORMED_MARGIN 0 //�任��߽����ӵ�(��)�߿�

int FindROI(Mat const SourceImage, Point2f *vertices);

//���߽綥�㰴˳ʱ������Ҫ��߽�Ϊ͹��
void SortVertex(vector<Point> &SourceVertex);

#endif
