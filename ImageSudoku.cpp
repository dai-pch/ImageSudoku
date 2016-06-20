// ImageSudoku.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

using namespace std;
using namespace cv;

int _tmain(int argc, _TCHAR* argv[])
{
	string fileName;
	Mat image_gray, image, image0;
	
	//cout << "Please input path and name of the image file:" << endl;
	//cin >> filename;
	fileName = "D:\\sudopic\\pic01.jpg"; //"testimage.png";//
	fileName[14] = NUM / 10 + 48;
	fileName[15] = NUM % 10 + 48;
	cout << fileName << endl;
	image0 = imread(fileName, IMREAD_COLOR);

	if (image0.empty())
	{
		cout << "Can't open the file." << endl;
		system("pause");
		return 0;
	}
	double ratio;
	ratio = image0.cols / 800.0;

	cvtColor(image0, image_gray, CV_RGB2GRAY);
	resize(image_gray, image, Size(), 1 / ratio, 1 / ratio, INTER_CUBIC);

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
	warpPerspective(image_gray, targetImage, transformMatrix, targetImage.size(), INTER_CUBIC);


	//��ʾѰ�ҵ�������
	namedWindow("ROI");
	imshow("ROI", targetImage);
	waitKey();
	//destroyAllWindows();

	vector<Mat> splitedImage = splitImage(targetImage);
	
	//����ָ������ͼ��
	//char name[] = "D:\\sudopic\\split8-00.png";
	/*for (int ii = 0; ii < 81; ii++)
	{*/
		/*//д�ļ�
		(char)((ii + 1) / 10 + 48);
		name[18] = (char)((ii+1) / 10 + 48);
		name[19] = (char)((ii+1) % 10 + 48);
		imwrite(name, splitedImage.at(ii));*/

		/*//��ʾ
		namedWindow("splitedImage");
		imshow("splitedImage", splitedImage.at(ii));
		waitKey();
		destroyWindow("splitedImage");
	}*/

	//ʶ�����ɾ���
	int sudoku[81];
	RecognizeSudoku(sudoku, splitedImage);


	//��ʾԭ����
	cout << "Sudoku is:" << endl;
	for (int ii = 0; ii < 81; ii++)
	{
		cout << sudoku[ii] << " ";
		if (ii % 9 == 8)
			cout << endl;
	}

	//������
	int sudokures[81];
	Sudoku su(sudoku);
	int success = su.solve(sudokures);

	//�����
	if (!success) //������ɹ�
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

		//��ʾ��
		//�����׼ͼƬ
		Mat numberImage[9];
		char picName[100] = ".\\pic\\1-0.bmp";
		for (int ii = 0; ii < 9; ii++)
		{
			picName[8] = ii + 49;
			numberImage[ii] = imread(picName, IMREAD_GRAYSCALE);
			//cout << numberImage[ii];
			/*imshow("1", numberImage[ii]);
			waitKey();*/
		}
		//���г�һ��ͼƬ
		Mat resultImage = Mat::zeros(Size(150 * 9, 150 * 9), CV_8U);
		//cout << resultImage;
		if (success)
		{
			for (int ii = 0; ii < 9; ii++)
			{
				for (int jj = 0; jj < 9; jj++)
				{
					if (sudoku[9 * ii + jj] == 0)
					{
						Mat imageROI = resultImage(Rect(150 * jj, 150 * ii, 150, 150));
						imageROI += numberImage[sudokures[9 * ii + jj] - 1];
					}
				}
			}
		}
		/*imshow("2", resultImage);
		waitKey();*/
		//����任
		transedVertices[0] = Point2f(0, 0);
		transedVertices[1] = Point2f(1350, 0);
		transedVertices[2] = Point2f(1350, 1350);
		transedVertices[3] = Point2f(0, 1350);
		transformMatrix = getPerspectiveTransform(transedVertices, vertices);
		//cout << transformMatrix;

		Mat subedImage = Mat::zeros(image_gray.size(), CV_8UC1);
		warpPerspective(resultImage, subedImage, transformMatrix, subedImage.size(), INTER_CUBIC);

		//�����ͼ��ӵ�ԭͼ����
		Mat channelsImage[3];
		split(image0, channelsImage);
		channelsImage[0] -= subedImage;
		channelsImage[1] -= subedImage;
		channelsImage[2] += subedImage;
		merge(channelsImage, 3, image0);

		imwrite("res.jpg", image0);
		imshow("2", image0);
		waitKey();
		destroyAllWindows();
	}

	system("pause");

	return 0;
}
