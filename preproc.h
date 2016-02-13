#ifndef PRE_PROC_H
#define PRE_PROC_H

#include "stdafx.h"

using namespace cv;
using namespace std;

#define ROI_INDENT 0
#define TRANSFORMED_SIZE 540

void FindROI(Mat const SourceImage, Mat& ROIImage);

#endif
