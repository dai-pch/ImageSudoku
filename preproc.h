#ifndef PRE_PROC_H
#define PRE_PROC_H

#include "stdafx.h"

using namespace cv;
using namespace std;

#define FIND_ROI_CUT 0
#define TRANSFORMED_SIZE 540
#define TRANSFORMED_MARGIN 5

Mat FindROI(Mat const SourceImage);

//给边界顶点按顺时针排序，要求边界为凸的
void SortVertex(vector<Point> &SourceVertex);

#endif
