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

//PayloadDivideFunction
int QTPayloadDivideFunction(BSPTreeNodeDivisionArg<QTLocation, QTLocation> arg) {
	if ((arg.PayloadLocation.x < arg.NodeLocation.x) && (arg.PayloadLocation.y < arg.NodeLocation.y)) {
		//northwest
		return 0;
	}
	if ((arg.PayloadLocation.x >= arg.NodeLocation.x) && (arg.PayloadLocation.y < arg.NodeLocation.y)) {
		//northeast
		return 1;
	}
	if ((arg.PayloadLocation.x > arg.NodeLocation.x) && (arg.PayloadLocation.y >= arg.NodeLocation.y)) {
		//southwest
		return 2;
	}
	if ((arg.PayloadLocation.x < arg.NodeLocation.x) && (arg.PayloadLocation.y >= arg.NodeLocation.y)) {
		//southeast
		return 3;
	}
}

//NodeDivideFunction
NLPackage<QTLocation, QTOperationBorder> QTNodeDivideFunction(NLPackage<QTLocation, QTOperationBorder> Node, int targetSubspace) {
	NLPackage<QTLocation, QTOperationBorder> result = Node;
	double x_shift = (Node.opBorder.max_x - Node.opBorder.min_x) / 4;
	double y_shift = (Node.opBorder.max_y - Node.opBorder.min_y) / 4;

	switch (targetSubspace) {
	case 0:
		result.nodeLoc.x = Node.opBorder.min_x + x_shift;
		result.nodeLoc.y = Node.opBorder.min_y + y_shift;
		result.opBorder.min_x = Node.opBorder.min_x;
		result.opBorder.min_y = Node.opBorder.min_y;
		result.opBorder.max_x = Node.opBorder.min_x + x_shift * 2;
		result.opBorder.max_y = Node.opBorder.min_y + y_shift * 2;
		break;
	case 1:
		result.nodeLoc.x = Node.opBorder.min_x + x_shift*3;
		result.nodeLoc.y = Node.opBorder.min_y + y_shift;
		result.opBorder.min_x = Node.opBorder.min_x + x_shift * 2;;
		result.opBorder.min_y = Node.opBorder.min_y;
		result.opBorder.max_x = Node.opBorder.max_x;
		result.opBorder.max_y = Node.opBorder.min_y + y_shift * 2;
		break;
	case 2:
		result.nodeLoc.x = Node.opBorder.min_x + x_shift * 3;
		result.nodeLoc.y = Node.opBorder.min_y + y_shift * 3;
		result.opBorder.min_x = Node.opBorder.min_x + x_shift * 2;
		result.opBorder.min_y = Node.opBorder.min_y + y_shift * 2;
		result.opBorder.max_x = Node.opBorder.max_x;
		result.opBorder.max_y = Node.opBorder.max_y;
		break;
	case 3:
		result.nodeLoc.x = Node.opBorder.min_x + x_shift;
		result.nodeLoc.y = Node.opBorder.min_y + y_shift * 3;
		result.opBorder.min_x = Node.opBorder.min_x;
		result.opBorder.min_y = Node.opBorder.min_y + y_shift * 2;
		result.opBorder.max_x = Node.opBorder.min_x + x_shift * 2;
		result.opBorder.max_y = Node.opBorder.max_y;
		break;
	}
	return result;
}

//distFunction
double QTDistFunction(QTLocation a, QTLocation b) {
	double distance = sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
	return distance;
}

//outOfBounds Function 
bool QTOutOfBoundsFunction(QTOperationBorder border, QTLocation location) {
	return (location.x < border.min_x) || (location.x > border.max_x) || (location.y < border.min_y) || (location.y > border.max_y);
}

FPackage<QTLocation, QTLocation, QTOperationBorder> getQTFPackage() {
	FPackage<QTLocation, QTLocation, QTOperationBorder> QTFPackage;
	QTFPackage.nodeDivideFunction = { QTNodeDivideFunction };
	QTFPackage.outOfBoundsFunction = { QTOutOfBoundsFunction };
	QTFPackage.payloadDivideFunction = { QTPayloadDivideFunction };
	return QTFPackage;
}

QTLocation getQTNodeLocation(QTOperationBorder border) {
	double dx = border.max_x - border.min_x;
	double dy = border.max_y - border.min_y;
	QTLocation l;
	l.x = dx / 2.0;
	l.y = dy / 2.0;
	return l;
}