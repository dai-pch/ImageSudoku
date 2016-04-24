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
	image0.release();
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
	//destroyAllWindows();

	vector<Mat> splitedImage = splitImage(targetImage);
	
	/*//��ʾ�ָ������ͼ��
	for (int ii = 0; ii < 81; ii++)
	{
		namedWindow("splitedImage");
		imshow("splitedImage", splitedImage.at(ii));
		waitKey();
		destroyWindow("splitedImage");
	}*/

	//ʶ�����ɾ���
	int sudoku[81];
	//��ʼ��������
	CreateInfo info;
	info.__numHiddenLayers = 1;
	info.__numInputsNodes = 900;
	info.__numLayerNodes.push_back(10);
	info.__numLayerNodes.push_back(9);
	Bpnn &bp = *new Bpnn(info);
	ifstream fs("log-train0.txt");
	bp.Import(fs);

	int empnum = splitedImage[0].rows * splitedImage[0].cols / 10.0;
	for (int ii = 0; ii < 81; ii++)
	{
		//����ǿո�
		if (sum(splitedImage[ii])(0) / 255 < empnum)
			sudoku[ii] = 0;
		else //������������ʶ��
		{
			Vecd temp;
			//��Ϊdouble
			splitedImage[ii].convertTo(temp, CV_64F, 1.0 / 255, 0);
			//������
			resize(temp, temp, Size(30, 30), 0, 0, INTER_CUBIC);
			//��Ϊ������
			temp = temp.reshape(1, 900);

			bp.transform(temp);
			double max = 0;
			for (int jj = 0; jj < 9; jj++)
			{
				if (temp(jj) > max)
				{
					max = temp(jj);
					sudoku[ii] = jj+1;
				}
			}
		}
	}

	delete &bp;

	//��ʾԭ����
	cout << "Sudoku is:" << endl;
	for (int ii = 0; ii < 81; ii++)
	{
		cout << sudoku[ii] << " ";
		if (ii % 9 == 8)
			cout << endl;
	}

	Sudoku su(sudoku);
	int flag = su.solve(sudoku);
	//��ʾ��
	if (!flag)
		cout << "There is no solutions." << endl;
	else
	{
		cout << "Solution is:" << endl;
		for (int ii = 0; ii < 81; ii++)
		{
			cout << sudoku[ii] << " ";
			if (ii % 9 == 8)
				cout << endl;
		}
	}
	waitKey();

	return 0;
}
