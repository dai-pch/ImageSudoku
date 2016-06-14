// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
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
