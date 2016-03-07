#include "stdafx.h"

using namespace cv;
using namespace std;

//寻找某一层边界数量
int contoursNumber(vector<Vec4i, allocator<Vec4i>> const SourceHierarchy, int Index, int Level)
{
	while (Level > 0 && Index >= 0)
	{
		Index = SourceHierarchy.at(Index)(2);
		Level += -1;
	}
	while (Level < 0 && Index >= 0)
	{
		Index = SourceHierarchy.at(Index)(3);
		Level += 1;
	}
	if (Index < 1)
		return 0;

	int number = -1;
	int ii = Index;
	while (ii>=0)
	{
		ii = SourceHierarchy.at(ii)(1);
		number += 1;
	}
	ii = Index;
	while (ii>=0)
	{
		ii = SourceHierarchy.at(ii)(0);
		number += 1;
	}
	return number;
}

//寻找最大面积、有足够子边界和四边形元素
int InterestContour(vector<vector<Point>> const SourceContours, vector<Vec4i, allocator<Vec4i>> const SourceHierarchy, int const Number_Threshold)
{
	vector<Point> approxContours;
	int index = 0; //hierarchy.at(0)(2);
	int maxIndex = -1;
	double area = 0, maxArea = 0;
	for (index = 0; index < (int)SourceContours.size(); index++)
	{
		area = contourArea(SourceContours.at(index));
		if (area > maxArea && contoursNumber(SourceHierarchy, index, 1) >= Number_Threshold)
		{
			//多边形拟合
			approxPolyDP(SourceContours.at(index), approxContours, arcLength(SourceContours.at(index), true)*0.02, true);
			if (approxContours.size() == 4 && isContourConvex(approxContours))
			{
				maxArea = area;
				maxIndex = index;
			}
		}
	}

	return maxIndex;
}
