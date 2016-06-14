#include "stdafx.h"

using namespace cv;
using namespace std;

vector<Mat> splitImage(Mat const SourceImage)
{
	//形态学
	//morphologyEx(medianImage, binaryImage, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(10, 10)));

	/*//显示二值化图像
	namedWindow("二值化图像");
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

			*it = SourceImage(rect);

			//二值化
			Mat binaryImage;
			threshold(*it, binaryImage, 191, 255, CV_THRESH_OTSU | CV_THRESH_BINARY_INV);

			//中值滤波
			Mat medianImage;
			medianBlur(binaryImage, *it, 3);

			//去除边框
			Mat temp;
			//形态学开运算，断开数字与边框
			//morphologyEx(*it, temp, MORPH_OPEN, getStructuringElement(MORPH_RECT, Size(5, 5)));
			it->copyTo(temp);

			/*imshow("00", temp);
			waitKey();*/
			if (ii == 6 && jj == 6)
			{
			imshow("00", temp);
			waitKey();
			}

			//连通域检测
 			vector<vector<Point>> contours;
			//vector<Vec4i> hierarchy;
			findContours(temp, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);//CHAIN_APPROX_TC89_L1);//
			double x2, y2, xm2 = 90000, ym2 = 90000;//, xm = TRANSFORMED_SIZE / 18 - SPLIT_CUT, ym = TRANSFORMED_SIZE / 18 - SPLIT_CUT;
			int index = -1;
			temp = Mat::zeros((TRANSFORMED_SIZE / 9 - 2 * SPLIT_CUT) * 2, (TRANSFORMED_SIZE / 9 - 2 * SPLIT_CUT) * 2, CV_8UC1);
			Rect roi(TRANSFORMED_SIZE / 18 - SPLIT_CUT, TRANSFORMED_SIZE / 18 - SPLIT_CUT, 0, 0);
			for (int jj = 0; jj < contours.size(); jj++)
			{
				Moments mom = moments(contours[jj]);
				if (mom.m00 > 0)
				{
					x2 = (mom.m20 - 2 * (TRANSFORMED_SIZE / 18 - SPLIT_CUT) * mom.m10) / mom.m00 + (TRANSFORMED_SIZE / 18 - SPLIT_CUT)*(TRANSFORMED_SIZE / 18 - SPLIT_CUT);
					y2 = (mom.m02 - 2 * (TRANSFORMED_SIZE / 18 - SPLIT_CUT) * mom.m01) / mom.m00 + (TRANSFORMED_SIZE / 18 - SPLIT_CUT)*(TRANSFORMED_SIZE / 18 - SPLIT_CUT);
				}
				else
				{
					x2 = 90000;
					y2 = 90000;
				}
				//cout << ii << ":" << x << ", " << y << endl;
				//cout << sqrt(x2 + y2) << endl;
				if (((sqrt(x2 + y2) < TRANSFORMED_SIZE / 18 - SPLIT_CUT - 4)))// && (xm2 + ym2) > (x2 + y2))))
				{ 
					xm2 = x2;
					ym2 = y2;
					//xm = mom.m10 / mom.m00 - TRANSFORMED_SIZE / 18 + SPLIT_CUT;
					//ym = mom.m01 / mom.m00 - TRANSFORMED_SIZE / 18 + SPLIT_CUT;
					index = jj;
					roi |= boundingRect(contours.at(jj));
				}
			}
			if (index >= 0)
				it->operator()(roi).copyTo(temp(Rect(TRANSFORMED_SIZE / 9 - 2 * SPLIT_CUT - roi.width / 2, 
				TRANSFORMED_SIZE / 9 - 2 * SPLIT_CUT - roi.height / 2, roi.width, roi.height)));
			/*if (index >= 0)
			{
				
			}*/
			
			/*if (ii == 8 && jj == 6)
			{
				imshow("10", temp);
				waitKey();
			}*/
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
