#include "stdafx.h"

using namespace cv;
using namespace std;

int RecognizeSudoku(int* Result, vector<Mat> const Image)
{
	//初始化神经网络
	CreateInfo info;
	info.__numHiddenLayers = 1;
	info.__numInputsNodes = 60;
	info.__numLayerNodes.push_back(20);
	info.__numLayerNodes.push_back(10);
	Bpnn &bp = *new Bpnn(info);
	ifstream fs("train1.txt");
	bp.Import(fs);

	for (int ii = 0; ii < 81; ii++)
	{
		//用神经网络识别
		Vecd projection(60, 1);
		Mat temp;

		//检测轮廓
		Mat temp2;
		Image[ii].copyTo(temp);
		//形态学
		morphologyEx(temp, temp2, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(5, 5)));

		/*if (ii == 19)
		{
			imshow("00", temp2);
			waitKey();
		}*/

		//连通域检测
		vector<vector<Point>> contours;
		//vector<Vec4i> hierarchy;
		findContours(temp2, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);//CHAIN_APPROX_TC89_L1);//
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
				x = 0;
				y = 0;
			}
			//cout << ii << ":" << x << ", " << y << endl;
			if (sqrt(xm*xm + ym*ym) > sqrt(x*x + y*y) && max(abs(x), abs(y)) < 20)
			{
				xm = x;
				ym = y;
				index = jj;
			}
		}
		temp = Mat::zeros((TRANSFORMED_SIZE / 9 - 2 * SPLIT_CUT)*2, (TRANSFORMED_SIZE / 9 - 2 * SPLIT_CUT)*2, CV_8UC1);
		if (index >= 0)
		{
			Rect roi = boundingRect(contours.at(index));
			Image[ii](roi).copyTo(temp(Rect(roi.x - xm + TRANSFORMED_SIZE / 18 - SPLIT_CUT,
				roi.y - ym + TRANSFORMED_SIZE / 18 - SPLIT_CUT, roi.width, roi.height)));
		}
		/*imshow("10", temp2);
		waitKey();*/

		//减采样
		resize(temp(Rect(TRANSFORMED_SIZE / 18 - SPLIT_CUT, TRANSFORMED_SIZE / 18 - SPLIT_CUT,
			TRANSFORMED_SIZE / 9 - 2 * SPLIT_CUT, TRANSFORMED_SIZE / 9 - 2 * SPLIT_CUT)),
			temp2, Size(30, 30), 0, 0, INTER_LINEAR);

		/*if (ii == 57)
		{
			imshow("00", Image[ii]);
			waitKey();
		}*/

		//写文件
		char name[] = "D:\\sudopic\\new\\split01-00.png";
		(char)((ii + 1) / 10 + 48);
		name[21] = NUM + 48;
		name[23] = (char)((ii+1) / 10 + 48);
		name[24] = (char)((ii+1) % 10 + 48);
		imwrite(name, temp2);

		Projection(projection, temp2);

		/*if (ii == 36)
			cout << temp << endl;*/
		//cout << projection.reshape(30, 2) << endl;
		bp.transform(projection);
		double max = 0;
		for (int jj = 0; jj < 10; jj++)
		{
			if (projection(jj) > max)
			{
				max = projection(jj);
				Result[ii] = jj;
			}
		}
	}
	delete &bp;

	return 0;
}

int Projection(Vecd &Result, Mat const Image)
{
	Mat temp;
	Vecd tempres(60, 1);
	Result = Mat::zeros(60, 1, CV_64FC1);

	//cout << Image << endl;
	temp = (Image + 127 - 50) / 255;
	//cout << temp << endl;

	int xlength = temp.cols, ylength = temp.rows;

	for (unsigned int ii = 0; ii < xlength; ii++)
	{
		tempres(ii) = sum(temp.col(ii))[0];
	}
	for (unsigned int ii = 0; ii < ylength; ii++)
	{
		tempres(ii + xlength) = sum(temp.row(ii))[0];
	}

	//cout << tempres.reshape(xlength, 2) << endl;
	
	//中心化x
	int begin = 0, maxlength = 0, length = 0;
	for (int ii = 0; ii < xlength; ii++)
	{
		if (tempres(ii) < 1)
		{
			if (maxlength < length)
			{
				maxlength = length;
				begin = ii - length;
			}
			length = 0;
		}
		else
		{
			length++;
		}
	}
	if (maxlength < length)
	{
		maxlength = length;
		begin = 30 - length;
	}
	length = 0;
	
	int offset = (xlength - maxlength) / 2;
	for (int ii = 0; ii < maxlength; ii++)
	{
		Result(offset + ii) = tempres(ii + begin);
	}

	//中心化y
	begin = xlength;
	maxlength = 0;
	length = 0;
	for (int ii = xlength; ii < xlength + ylength; ii++)
	{
		if (tempres(ii) < 1)
		{
			if (maxlength < length)
			{
				maxlength = length;
				begin = ii - length;
			}
			length = 0;
		}
		else
		{
			length++;
		}
	}
	if (maxlength < length)
	{
		maxlength = length;
		begin = 60 - length;
	}

	offset = xlength + (ylength - maxlength) / 2;
	for (int ii = 0; ii < maxlength; ii++)
	{
		Result(offset + ii) = tempres(ii + begin);
	}

	//cout << Result.reshape(xlength, 2) << endl;

	return 0;
}
