#ifndef SPLIT_H
#define SPLIT_H

#include "stdafx.h"

using namespace cv;
using namespace std;

#define SPLIT_CUT 3

vector<Mat> splitImage(Mat const SourceImage);

//Calculate the center of a set of points.
Point2d center(const vector<Point> SetOfPoint);

//Find the circumscribed rectangle of the conponents of a contours.
Rect ContentsRegion(vector<vector<Point>> const SourceContours,
					int const index);

#endif
