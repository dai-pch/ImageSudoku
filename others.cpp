#include "stdafx.h"

using namespace cv;
using namespace std;

int contoursNumber(vector<Vec4i, allocator<Vec4i>> const TargetHierarchy, int Index, int Level)
{
	while (Level > 0 && Index >= 0)
	{
		Index = TargetHierarchy.at(Index)(2);
		Level += -1;
	}
	while (Level < 0 && Index >= 0)
	{
		Index = TargetHierarchy.at(Index)(3);
		Level += 1;
	}
	if (Index < 1)
		return 0;

	int number = -1;
	int ii = Index;
	while (ii>=0)
	{
		ii = TargetHierarchy.at(ii)(1);
		number += 1;
	}
	ii = Index;
	while (ii>=0)
	{
		ii = TargetHierarchy.at(ii)(0);
		number += 1;
	}
	return number;
}
