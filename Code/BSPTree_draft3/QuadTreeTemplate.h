#pragma once

#include "TreeNode.h"
#include <cmath>

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

template <class Payload>

//divideFunction
BSPTreeNodeDivisionResult<4> QTDivideFunction(BSPTreeNodeDivisionArg<QTLocation, QTLocation> arg) {
	BSPTreeNodeDivisionResult<4> result;
	if ((arg.PayloadLocation.x < arg.NodeLocation.x) && (arg.PayloadLocation.y < arg.NodeLocation.y)) {
		//northwest
		result.Subspace = 0;
	}
	if ((arg.PayloadLocation.x >= arg.NodeLocation.x) && (arg.PayloadLocation.y < arg.NodeLocation.y)) {
		//northeast
		result.Subspace = 1;
	}
	if ((arg.PayloadLocation.x > arg.NodeLocation.x) && (arg.PayloadLocation.y >= arg.NodeLocation.y)) {
		//southwest
		result.Subspace = 2;
	}
	if ((arg.PayloadLocation.x < arg.NodeLocation.x) && (arg.PayloadLocation.y >= arg.NodeLocation.y)) {
		//southeast
		result.Subspace = 3;
	}
	return result;
}

//SubDivideFunction

//distFunction
double QTDistFunction(QTLocation a, QTLocation b) {
	double distance = sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
	return distance;
}

//outOfBounds Function 
bool QTOutOfBounds(QTOperationBorder border, QTLocation location) {
	return (location.x < border.min_x) || (location.x > border.max_x) || (location.y < border.min_y) || (location.y > border.max_y);
}