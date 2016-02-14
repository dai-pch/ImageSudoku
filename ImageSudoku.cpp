// ImageSudoku.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

using namespace std;
using namespace cv;

int _tmain(int argc, _TCHAR* argv[])
{
	string fileName;
	Mat image;

	//cout << "Please input path and name of the image file:" << endl;
	//cin >> filename;
	fileName = "testimage.png";
	image = imread(fileName, 0);

	if (image.empty())
	{
		cout << "Can't open the file." << endl;
		return 0;
	}

	//��ʾԭʼ�Ҷ�ͼ��
	namedWindow("ԭʼͼ��");
	imshow("ԭʼͼ��",image);

	//Ѱ����������
	Mat targetImage;
	targetImage = FindROI(image);

	//��ʾѰ�ҵ�������
	namedWindow("ROI");
	imshow("ROI", targetImage);

	splitImage(targetImage);



	waitKey();

	return 0;
}
