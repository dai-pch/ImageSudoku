// ImageSudoku.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

using namespace std;
using namespace cv;

int _tmain(int argc, _TCHAR* argv[])
{
	string fileName;
	Mat image, image0;

	//cout << "Please input path and name of the image file:" << endl;
	//cin >> filename;
	fileName = "testimage3.png";//"D:\\sudopic\\pic4.jpg";
	image0 = imread(fileName, 0);

	if (image0.empty())
	{
		cout << "Can't open the file." << endl;
		system("pause");
		return 0;
	}
	resize(image0, image, Size(1200, (int)(image0.cols * 1200) / image0.rows), 0, 0, INTER_CUBIC);
	//��ʾԭʼ�Ҷ�ͼ��
	namedWindow("ԭʼͼ��");
	imshow("ԭʼͼ��", image);
	waitKey();
	destroyAllWindows();

	//Ѱ����������
	Mat targetImage;
	targetImage = FindROI(image);

	//��ʾѰ�ҵ�������
	namedWindow("ROI");
	imshow("ROI", targetImage);
	waitKey();
	destroyAllWindows();

	vector<Mat> splitedImage = splitImage(targetImage);
	for (int ii = 0; ii < 81; ii++)
	{
		namedWindow("splitedImage");
		imshow("splitedImage", splitedImage.at(ii));
		waitKey();
		destroyWindow("splitedImage");
	}

	return 0;
}
