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



}
