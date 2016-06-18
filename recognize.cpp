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

		//减采样
		resize(Image[ii], temp, Size(30, 30), 0, 0, INTER_LINEAR);
		threshold(temp, temp, 30, 255, CV_8UC1);

		/*if (ii < 9)
			cout << temp << endl;*/

		/*if (ii == 75)
		{
			imshow("00", Image[ii]);
			waitKey();
		}*/
		/*imshow("00", Image[ii]);
		waitKey();*/

		//写文件
		char name[] = "D:\\sudopic\\new\\split01-00.png";
		(char)((ii + 1) / 10 + 48);
		name[20] = NUM / 10 + 48;
		name[21] = NUM % 10 + 48;
		name[23] = (char)((ii+1) / 10 + 48);
		name[24] = (char)((ii+1) % 10 + 48);
		imwrite(name, temp);

		Projection(projection, temp);

		/*if (ii < 9)
			cout << projection.reshape(1,2) << endl;*/
		//cout << projection.reshape(30, 2) << endl;
		bp.transform(projection);
		//cout << projection << endl;
		double max = 0;
		for (int jj = 0; jj < 10; jj++)
		{
			if (projection(jj) > max)
			{
				max = projection(jj);
				Result[ii] = (jj + 1)%10;
			}
		}
	}
	delete &bp;

	return 0;
}

int Projection(Vecd &Result, Mat const Image)
{
	unsigned int xlength = Image.cols, ylength = Image.rows;

	//Mat temp;
	Vecd tempres(xlength + ylength, 1);
	Result = Mat::zeros(xlength + ylength, 1, CV_64FC1);

	//cout << Image << endl;
	//temp = Image;
	//cout << temp << endl;

	for (unsigned int ii = 0; ii < xlength; ii++)
	{
		tempres(ii) = sum(Image.col(ii))[0] / 255.0;
	}
	for (unsigned int ii = 0; ii < ylength; ii++)
	{
		tempres(ii + xlength) = sum(Image.row(ii))[0] / 255.0;
	}

	//cout << tempres.reshape(xlength, 2) << endl;
	
	tempres.copyTo(Result);

	/*//中心化x
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
	}*/

	//cout << Result.reshape(xlength, 2) << endl;

	return 0;
}
