#ifndef PRE_PROC_H
#define PRE_PROC_H

#include "stdafx.h"

using namespace cv;
using namespace std;

#define FIND_ROI_CUT 10 //��ԭͼ����Ѱ�Ҿ���ʱ�������ǵı߽粿�ֿ��
#define TRANSFORMED_CUT 6 //ROI����õ��ı߿�
#define TRANSFORMED_SIZE 540 //�任Ϊ�����εı߳�
#define TRANSFORMED_MARGIN 8 //�任��߽����ӵ�(��)�߿�

Mat FindROI(Mat const SourceImage);

//���߽綥�㰴˳ʱ������Ҫ��߽�Ϊ͹��
void SortVertex(vector<Point> &SourceVertex);

#endif
