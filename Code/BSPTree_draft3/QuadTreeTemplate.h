#pragma once

#include "TreeNode.h"

//Location (for the Quadtree this is also NodeLocation)
struct QTLocation {
	double x;
	double y;
};

//OperationBorder
struct QTOperationBorder {
	double min_x;
	double max_x;
	double min_y;
	double max_y;
};

//divideFunction
BSPTreeNodeDivisionResult<4> QTDivideFct(BSPTreeNodeDivisionArg<QTLocation, QTLocation>) {

}

//distFunction