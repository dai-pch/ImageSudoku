#include "stdafx.h"

using namespace cv;
using namespace std;

vector<Mat> splitImage(Mat const SourceImage)
{
	//二值化
	Mat binaryImage;
	threshold(SourceImage, binaryImage, 191, 255, CV_THRESH_OTSU | CV_THRESH_BINARY_INV);

	//中值滤波
	Mat medianImage;
	medianBlur(binaryImage, medianImage, 3);

	//形态学
	//morphologyEx(medianImage, binaryImage, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(10, 10)));

	binaryImage = medianImage;

	//显示二值化图像
	/*namedWindow("二值化图像");
	imshow("二值化图像", binaryImage);
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
