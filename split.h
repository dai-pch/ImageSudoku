#ifndef SPLIT_H
#define SPLIT_H

#include "stdafx.h"

using namespace cv;
using namespace std;

#define SPLIT_CUT 5 //分离出的每个小矩形剪切的边距

vector<Mat> splitImage(Mat const SourceImage);

#endif
