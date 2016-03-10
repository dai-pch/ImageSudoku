#ifndef OTHERS_H
#define OTHERS_H

#include "stdafx.h"

using namespace cv;
using namespace std;

//寻找某一层边界数量
int contoursNumber(vector<Vec4i, allocator<Vec4i>> const SourceHierarchy,
					int Index, int Level);

//寻找最大面积、有足够子边界和四边形元素
int InterestContour(vector<vector<Point>> const SourceContours, 
					vector<Vec4i, allocator<Vec4i>> const SourceHierarchy,
					int const Number_Threshold);

//对比度拉伸
Mat ContrastStretch(Mat const SourceImage, double min);

#endif
