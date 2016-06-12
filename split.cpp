#include "stdafx.h"

using namespace cv;
using namespace std;

vector<Mat> splitImage(Mat const SourceImage)
{
	//��ֵ��
	Mat binaryImage;
	threshold(SourceImage, binaryImage, 191, 255, CV_THRESH_OTSU | CV_THRESH_BINARY_INV);

	//��ֵ�˲�
	Mat medianImage;
	medianBlur(binaryImage, medianImage, 3);

	//��̬ѧ
	//morphologyEx(medianImage, binaryImage, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(10, 10)));

	binaryImage = medianImage;

	//��ʾ��ֵ��ͼ��
	namedWindow("��ֵ��ͼ��");
	imshow("��ֵ��ͼ��", binaryImage);
	waitKey();

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

			//ȥ���߿�
			Mat temp;

			//��̬ѧ�����㣬��ͨ���ܶϿ�������
			morphologyEx(*it, temp, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(5, 5)));

			/*imshow("00", temp);
			waitKey();*/
			/*if (ii == 19)
			{
			imshow("00", temp2);
			waitKey();
			}*/

			//��ͨ����
 			vector<vector<Point>> contours;
			//vector<Vec4i> hierarchy;
			findContours(temp, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);//CHAIN_APPROX_TC89_L1);//
			double x, y, xm = TRANSFORMED_SIZE / 9 - 2 * SPLIT_CUT, ym = TRANSFORMED_SIZE / 9 - 2 * SPLIT_CUT;
			int index = -1;
			for (int jj = 0; jj < contours.size(); jj++)
			{
				Moments mom = moments(contours[jj]);
				if (mom.m00 > 0)
				{
					x = mom.m10 / mom.m00 - TRANSFORMED_SIZE / 18 + SPLIT_CUT;
					y = mom.m01 / mom.m00 - TRANSFORMED_SIZE / 18 + SPLIT_CUT;
				}
				else
				{
					x = 500;
					y = 500;
				}
				//cout << ii << ":" << x << ", " << y << endl;
				if (sqrt(xm*xm + ym*ym) > sqrt(x*x + y*y) && max(abs(x), abs(y)) < 20)
				{
					xm = x;
					ym = y;
					index = jj;
				}
			}
			temp = Mat::zeros((TRANSFORMED_SIZE / 9 - 2 * SPLIT_CUT) * 2, (TRANSFORMED_SIZE / 9 - 2 * SPLIT_CUT) * 2, CV_8UC1);
			if (index >= 0)
			{
				Rect roi = boundingRect(contours.at(index));
				it->operator()(roi).copyTo(temp(Rect(roi.x - xm + TRANSFORMED_SIZE / 18 - SPLIT_CUT,
					roi.y - ym + TRANSFORMED_SIZE / 18 - SPLIT_CUT, roi.width, roi.height)));
			}

			/*imshow("10", temp);
			waitKey();*/

			temp(Rect(TRANSFORMED_SIZE / 18 - SPLIT_CUT, TRANSFORMED_SIZE / 18 - SPLIT_CUT,
				TRANSFORMED_SIZE / 9 - 2 * SPLIT_CUT, TRANSFORMED_SIZE / 9 - 2 * SPLIT_CUT)).copyTo(*it);

			//cout << *it << endl;
			it++;
		}
	}
	return imageVec;
}
