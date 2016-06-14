// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO:  在此处引用程序需要的其他头文件
#include <iostream> 
#include <fstream>
#include <string>
#include <math.h>
//#include <error>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "lib\bpnnlib\Bpnnlib.h"
#include "lib\sudokulib\sudokulib.h"

#include "others.h"
#include "preproc.h"
#include "split.h"
#include "recognize.h"

#ifdef _DEBUG
#pragma comment(lib, "./lib/bpnnlib/debug/Bpnnlib.lib")
#pragma comment(lib, "./lib/sudokulib/debug/sudokulib.lib")
#else
#pragma comment(lib, "./lib/bpnnlib/release/Bpnnlib.lib")
#pragma comment(lib, "./lib/sudokulib/release/sudokulib.lib")
#endif

#define NUM 2
