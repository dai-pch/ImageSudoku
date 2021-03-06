#ifndef PRE_PROC_H
#define PRE_PROC_H

#include "stdafx.h"

using namespace cv;
using namespace std;

#define FIND_ROI_CUT 5 //在原图像上寻找矩形时，不考虑的边界部分宽度
#define TRANSFORMED_SIZE 540 //变换为正方形的边长
#define TRANSFORMED_MARGIN 0 //变换后边界增加的(白)边宽

int FindROI(Mat const SourceImage, Point2f *vertices);

//给边界顶点按顺时针排序，要求边界为凸的
void SortVertex(vector<Point> &SourceVertex);

#endif
