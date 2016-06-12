#ifndef RECOGNIZE_H
#define RECOGNIZE_H

#include "stdafx.h"

using namespace cv;
using namespace std;

int RecognizeSudoku(int* Result, vector<Mat> const Image);

int Projection(Vecd &Result, Mat const Image);

#endif