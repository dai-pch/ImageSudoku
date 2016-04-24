#include "stdafx.h"

using namespace cv;
using namespace std;

vector<Mat> splitImage(Mat const SourceImage)
{
	//��ֵ��
	Mat binaryImage;
	threshold(SourceImage(Rect(TRANSFORMED_MARGIN, TRANSFORMED_MARGIN, TRANSFORMED_SIZE, TRANSFORMED_SIZE)), binaryImage, 191, 255, CV_THRESH_OTSU | CV_THRESH_BINARY_INV);

	//��ֵ�˲�
	Mat medianImage;
	medianBlur(binaryImage, medianImage, 3);

	//��̬ѧ��ʴ
	//morphologyEx(medianImage, binaryImage, MORPH_ERODE, getStructuringElement(MORPH_RECT, Size(3, 3)));
	binaryImage = medianImage;

	/*//��ʾ��ֵ��ͼ��
	namedWindow("��ֵ��ͼ��");
	imshow("��ֵ��ͼ��", binaryImage);
	waitKey();*/

	vector<Mat> imageVec(81);
	int width = TRANSFORMED_SIZE / 9 - 2 * SPLIT_CUT;
	vector<Mat>::iterator it = imageVec.begin();
	for (int ii = 0; ii < 9; ii++)
	{
		for (int jj = 0; jj < 9; jj++)
		{
			int start_x = jj * TRANSFORMED_SIZE / 9 + SPLIT_CUT;
			int start_y = ii * TRANSFORMED_SIZE / 9 + SPLIT_CUT;
			Rect rect(start_x, start_y, width, width);
			*it = binaryImage(rect);
			//cout << *it << endl;
			it++;
		}
	}
	return imageVec;
}
