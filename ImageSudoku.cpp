// ImageSudoku.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

using namespace std;
using namespace cv;

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Initializing, please wait." << endl;

	//初始化神经网络
	CreateInfo info;
	info.__numHiddenLayers = 1;
	info.__numInputsNodes = 900;
	info.__numLayerNodes.push_back(20);
	info.__numLayerNodes.push_back(10);
	Bpnn &bp = *new Bpnn(info);
	ifstream fs(".\\pic\\train3.txt");
	bp.Import(fs);

	//读入标准图片
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

	string fileName;
	Mat image_gray, image, image0;

	cout << "Please input the name of the image file:" << endl;
	cin >> fileName;
	/*fileName = "D:\\sudopic\\pic01.jpg"; //"testimage.png";//
	fileName[14] = NUM / 10 + 48;
	fileName[15] = NUM % 10 + 48;
	cout << fileName << endl;*/

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
	//显示原始灰度图像
	namedWindow("原始图像", WINDOW_NORMAL);
	imshow("原始图像", image0);
	cout << "The original image is showed. Press any key to continue..." << endl;
	waitKey();
	//destroyAllWindows();

	//寻找网格区域
	Point2f vertices[4];
	FindROI(image, vertices);
	for (int ii = 0; ii < 4; ii++)
		vertices[ii] *= ratio;

	//仿射变换
	Point2f transedVertices[4];
	transedVertices[0] = Point2f(TRANSFORMED_MARGIN, TRANSFORMED_MARGIN);
	transedVertices[1] = Point2f(TRANSFORMED_SIZE - TRANSFORMED_MARGIN + 1, TRANSFORMED_MARGIN);
	transedVertices[2] = Point2f(TRANSFORMED_SIZE - TRANSFORMED_MARGIN + 1, TRANSFORMED_SIZE - TRANSFORMED_MARGIN + 1);
	transedVertices[3] = Point2f(TRANSFORMED_MARGIN, TRANSFORMED_SIZE - TRANSFORMED_MARGIN + 1);

	Mat transformMatrix = getPerspectiveTransform(vertices, transedVertices);
	Mat targetImage = Mat::zeros(Size(TRANSFORMED_SIZE, TRANSFORMED_SIZE), CV_8UC1);
	warpPerspective(image_gray, targetImage, transformMatrix, targetImage.size(), INTER_CUBIC);


	//显示寻找到的区域
	namedWindow("ROI", WINDOW_NORMAL);
	imshow("ROI", targetImage);
	cout << "The ROI is showed. Press any key to continue..." << endl;
	waitKey();
	//destroyAllWindows();

	vector<Mat> splitedImage = splitImage(targetImage);

	//处理分割出来的图像
	//char name[] = "D:\\sudopic\\split8-00.png";
	/*for (int ii = 0; ii < 81; ii++)
	{*/
	/*//写文件
	(char)((ii + 1) / 10 + 48);
	name[18] = (char)((ii+1) / 10 + 48);
	name[19] = (char)((ii+1) % 10 + 48);
	imwrite(name, splitedImage.at(ii));*/

	/*//显示
	namedWindow("splitedImage");
	imshow("splitedImage", splitedImage.at(ii));
	waitKey();
	destroyWindow("splitedImage");
	}*/

	//识别，生成矩阵
	int sudoku[81];
	RecognizeSudoku(sudoku, splitedImage, bp);


	//显示原数独
	cout << "The sudoku in the picture is:" << endl;
	for (int ii = 0; ii < 81; ii++)
	{
		cout << sudoku[ii] << " ";
		if (ii % 9 == 8)
			cout << endl;
	}

	//解数独
	int sudokures[81];
	Sudoku su(sudoku);
	int success = su.solve(sudokures);

	//输出答案
	if (!success) //如果不成功
	{
		cout << "There is no solutions." << endl;
	}
	else
	{
		cout << "Solution is:" << endl;
		for (int ii = 0; ii < 81; ii++)
		{
			cout << sudokures[ii] << " ";
			if (ii % 9 == 8)
				cout << endl;
		}

		//显示答案
		//排列成一幅图片
		Mat resultImage = Mat::zeros(Size(150 * 9, 150 * 9), CV_8U);
		//cout << resultImage;
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
		/*imshow("2", resultImage);
		waitKey();*/
		//仿射变换
		transedVertices[0] = Point2f(0, 0);
		transedVertices[1] = Point2f(1350, 0);
		transedVertices[2] = Point2f(1350, 1350);
		transedVertices[3] = Point2f(0, 1350);
		transformMatrix = getPerspectiveTransform(transedVertices, vertices);
		//cout << transformMatrix;

		Mat subedImage = Mat::zeros(image_gray.size(), CV_8UC1);
		warpPerspective(resultImage, subedImage, transformMatrix, subedImage.size(), INTER_CUBIC);

		//将结果图像加到原图像上
		Mat channelsImage[3];
		split(image0, channelsImage);
		channelsImage[0] -= subedImage;
		channelsImage[1] -= subedImage;
		channelsImage[2] += subedImage;
		merge(channelsImage, 3, image0);

		//输出图像
		cout << "The result is showed and saved as result.jpg, press any key to continue." << endl;
		imwrite("result.jpg", image0);
		//显示图像
		destroyAllWindows();
		namedWindow("输出结果", WINDOW_NORMAL);
		imshow("输出结果", image0);
		waitKey();
		destroyAllWindows();
	}

	system("pause");

	return 0;
}
