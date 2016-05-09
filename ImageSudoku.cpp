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
	fileName = "testimage3.png";//"D:\\sudopic\\pic6.jpg";
	image0 = imread(fileName, IMREAD_GRAYSCALE);

	if (image0.empty())
	{
		cout << "Can't open the file." << endl;
		system("pause");
		return 0;
	}
	double ratio;
	ratio = image0.cols / 1000.0;

 	resize(image0, image, Size(), 1/ratio, 1/ratio, INTER_CUBIC);
	//image0.release();
	//��ʾԭʼ�Ҷ�ͼ��
	namedWindow("ԭʼͼ��");
	imshow("ԭʼͼ��", image);
	waitKey();
	destroyAllWindows();

	//Ѱ����������
 	Point2f vertices[4];
	FindROI(image, vertices);
	for (int ii = 0; ii < 4; ii++)
		vertices[ii] *= ratio;

	//����任
	Point2f transedVertices[4];
	transedVertices[0] = Point2f(TRANSFORMED_MARGIN, TRANSFORMED_MARGIN);
	transedVertices[1] = Point2f(TRANSFORMED_SIZE - TRANSFORMED_MARGIN + 1, TRANSFORMED_MARGIN);
	transedVertices[2] = Point2f(TRANSFORMED_SIZE - TRANSFORMED_MARGIN + 1, TRANSFORMED_SIZE - TRANSFORMED_MARGIN + 1);
	transedVertices[3] = Point2f(TRANSFORMED_MARGIN, TRANSFORMED_SIZE - TRANSFORMED_MARGIN + 1);

	Mat transformMatrix = getPerspectiveTransform(vertices, transedVertices);
	Mat targetImage = Mat::zeros(Size(TRANSFORMED_SIZE, TRANSFORMED_SIZE), CV_8UC1);
	warpPerspective(image0, targetImage, transformMatrix, targetImage.size(), INTER_CUBIC);


	//��ʾѰ�ҵ�������
	namedWindow("ROI");
	imshow("ROI", targetImage);
	waitKey();
	//destroyAllWindows();

	vector<Mat> splitedImage = splitImage(targetImage);
	
	//��ʾ�ָ������ͼ��
	char name[] = "D:\\sudopic\\split8-00.png";
	for (int ii = 0; ii < 81; ii++)
	{
		(char)((ii + 1) / 10 + 48);
		name[18] = (char)((ii+1) / 10 + 48);
		name[19] = (char)((ii+1) % 10 + 48);
		imwrite(name, splitedImage.at(ii));
		/*
		namedWindow("splitedImage");
		imshow("splitedImage", splitedImage.at(ii));
		waitKey();
		destroyWindow("splitedImage");*/
	}

	//ʶ�����ɾ���
	int sudoku[81], sudokures[81];
	//��ʼ��������
	CreateInfo info;
	info.__numHiddenLayers = 1;
	info.__numInputsNodes = 900;
	info.__numLayerNodes.push_back(10);
	info.__numLayerNodes.push_back(9);
	Bpnn &bp = *new Bpnn(info);
	ifstream fs("log-train0.txt");
	bp.Import(fs);

	int empnum = (int)(splitedImage[0].rows * splitedImage[0].cols / 8.0);
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
	int flag = su.solve(sudokures);
	//��ʾ��
	if (!flag)
		cout << "There is no solutions." << endl;
	else
	{
		cout << "Solution is:" << endl;
		for (int ii = 0; ii < 81; ii++)
		{
			cout << sudokures[ii] << " ";
			if (ii % 9 == 8)
				cout << endl;
		}
	}
	waitKey();

	return 0;
}
