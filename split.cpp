#include "stdafx.h"

using namespace cv;
using namespace std;

/*vector<Mat>*/void splitImage(Mat const SourceImage)
{
	//二值化
	Mat binaryImage;
	threshold(SourceImage, binaryImage, 191, 255, CV_THRESH_OTSU);

	/*//显示二值化图像
	namedWindow("二值化图像");
	imshow("二值化图像", binaryImage);*/



}
