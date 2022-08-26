#pragma once

#include "TreeNode.h"
#include <cmath>
#include "helpers.h"

//Location (for the Oc-tree this is also NodeLocation)
struct OCTLocation {
	double x = 0;
	double y = 0;
	double z = 0;
};

//OperationBorder
struct OCTOperationBorder {
	double min_x = 0;
	double max_x = 0;
	double min_y = 0;
	double max_y = 0;
	double min_z = 0;
	double max_z = 0;
};

void printOCTLocation(OCTLocation payload, OCTLocation node) {	 //for debgging
	std::cout << "Payload at (" << payload.x << "|" << payload.y << "|" << payload.z << ")" << std::endl;
	std::cout << "Node at (" << node.x << "|" << node.y << "|" << node.z << ")" << std::endl;
}

//PayloadDivideFunction
int OCTPayloadDivideFunction(BSPTreeNodeDivisionArg<OCTLocation, OCTLocation> arg) {
	if ((arg.PayloadLocation.x < arg.NodeLocation.x) && (arg.PayloadLocation.y < arg.NodeLocation.y) && (arg.PayloadLocation.z < arg.NodeLocation.z)) {
		//northwest front
		//log("northwest: 0");
		return 0;
	}
	if ((arg.PayloadLocation.x >= arg.NodeLocation.x) && (arg.PayloadLocation.y < arg.NodeLocation.y) && (arg.PayloadLocation.z < arg.NodeLocation.z)) {
		//northeast	front
		//log("northeast: 1");
		return 1;
	}
	if ((arg.PayloadLocation.x >= arg.NodeLocation.x) && (arg.PayloadLocation.y >= arg.NodeLocation.y) && (arg.PayloadLocation.z < arg.NodeLocation.z)) {
		//southeast	front
		//log("southeast: 2");
		return 2;
	}
	if ((arg.PayloadLocation.x < arg.NodeLocation.x) && (arg.PayloadLocation.y >= arg.NodeLocation.y) && (arg.PayloadLocation.z < arg.NodeLocation.z)) {
		//southwest	front
		//log("southwest: 3");
		return 3;
	}
	if ((arg.PayloadLocation.x < arg.NodeLocation.x) && (arg.PayloadLocation.y < arg.NodeLocation.y) && (arg.PayloadLocation.z >= arg.NodeLocation.z)) {
		//northwest	back
		//log("northwest: 4");
		return 4;
	}
	if ((arg.PayloadLocation.x >= arg.NodeLocation.x) && (arg.PayloadLocation.y < arg.NodeLocation.y) && (arg.PayloadLocation.z >= arg.NodeLocation.z)) {
		//northeast	back
		//log("northeast: 5");
		return 5;		
	}
	if ((arg.PayloadLocation.x >= arg.NodeLocation.x) && (arg.PayloadLocation.y >= arg.NodeLocation.y) && (arg.PayloadLocation.z >= arg.NodeLocation.z)) {
		//southeast	back
		//log("southeast: 6");
		return 6;
	}
	if ((arg.PayloadLocation.x < arg.NodeLocation.x) && (arg.PayloadLocation.y >= arg.NodeLocation.y) && (arg.PayloadLocation.z >= arg.NodeLocation.z)) {
		//southwest	back
		//log("southwest: 7");
		return 7;
	}
	log("unknown: 8");
	return 8; //if we end up here somehow something is broken
}

//NodeDivideFunction
NLPackage<OCTLocation, OCTOperationBorder> OCTNodeDivideFunction(NLPackage<OCTLocation, OCTOperationBorder> Node, int targetSubspace) {
	NLPackage<OCTLocation, OCTOperationBorder> result = Node;
	double x_shift = (Node.opBorder.max_x - Node.opBorder.min_x) / 4;
	double y_shift = (Node.opBorder.max_y - Node.opBorder.min_y) / 4;
	double z_shift = (Node.opBorder.max_z - Node.opBorder.min_z) / 4;

	switch (targetSubspace) {
	case 0:
		result.nodeLoc.x = Node.opBorder.min_x + x_shift;
		result.nodeLoc.y = Node.opBorder.min_y + y_shift;
		result.nodeLoc.z = Node.opBorder.min_z + z_shift;
		result.opBorder.min_x = Node.opBorder.min_x;
		result.opBorder.min_y = Node.opBorder.min_y;
		result.opBorder.min_z = Node.opBorder.min_z;
		result.opBorder.max_x = Node.opBorder.min_x + x_shift * 2;
		result.opBorder.max_y = Node.opBorder.min_y + y_shift * 2;
		result.opBorder.max_z = Node.opBorder.min_z + z_shift * 2;
		break;
	case 1:
		result.nodeLoc.x = Node.opBorder.min_x + x_shift * 3;
		result.nodeLoc.y = Node.opBorder.min_y + y_shift;
		result.nodeLoc.z = Node.opBorder.min_z + z_shift;
		result.opBorder.min_x = Node.opBorder.min_x + x_shift * 2;
		result.opBorder.min_y = Node.opBorder.min_y;
		result.opBorder.min_z = Node.opBorder.min_z;
		result.opBorder.max_x = Node.opBorder.max_x;
		result.opBorder.max_y = Node.opBorder.min_y + y_shift * 2;
		result.opBorder.max_z = Node.opBorder.min_z + z_shift * 2;
		break;
	case 2:
		result.nodeLoc.x = Node.opBorder.min_x + x_shift * 3;
		result.nodeLoc.y = Node.opBorder.min_y + y_shift * 3;
		result.nodeLoc.z = Node.opBorder.min_z + z_shift;
		result.opBorder.min_x = Node.opBorder.min_x + x_shift * 2;
		result.opBorder.min_y = Node.opBorder.min_y + y_shift * 2;
		result.opBorder.min_z = Node.opBorder.min_z;
		result.opBorder.max_x = Node.opBorder.max_x;
		result.opBorder.max_y = Node.opBorder.max_y;
		result.opBorder.max_z = Node.opBorder.min_z + z_shift * 2;
		break;
	case 3:
		result.nodeLoc.x = Node.opBorder.min_x + x_shift;
		result.nodeLoc.y = Node.opBorder.min_y + y_shift * 3;
		result.nodeLoc.z = Node.opBorder.min_z + z_shift;
		result.opBorder.min_x = Node.opBorder.min_x;
		result.opBorder.min_y = Node.opBorder.min_y + y_shift * 2;
		result.opBorder.min_z = Node.opBorder.min_z;
		result.opBorder.max_x = Node.opBorder.min_x + x_shift * 2;
		result.opBorder.max_y = Node.opBorder.max_y;
		result.opBorder.max_z = Node.opBorder.min_z + z_shift * 2;
		break;

	case 4:
		result.nodeLoc.x = Node.opBorder.min_x + x_shift;
		result.nodeLoc.y = Node.opBorder.min_y + y_shift;
		result.nodeLoc.z = Node.opBorder.min_z + z_shift * 3;
		result.opBorder.min_x = Node.opBorder.min_x;
		result.opBorder.min_y = Node.opBorder.min_y;
		result.opBorder.min_z = Node.opBorder.min_z + z_shift * 2;
		result.opBorder.max_x = Node.opBorder.min_x + x_shift * 2;
		result.opBorder.max_y = Node.opBorder.min_y + y_shift * 2;
		result.opBorder.max_z = Node.opBorder.max_z;
		break;
	case 5:
		result.nodeLoc.x = Node.opBorder.min_x + x_shift * 3;
		result.nodeLoc.y = Node.opBorder.min_y + y_shift;
		result.nodeLoc.z = Node.opBorder.min_z + z_shift * 3;
		result.opBorder.min_x = Node.opBorder.min_x + x_shift * 2;
		result.opBorder.min_y = Node.opBorder.min_y;
		result.opBorder.min_z = Node.opBorder.min_z + z_shift * 2;
		result.opBorder.max_x = Node.opBorder.max_x;
		result.opBorder.max_y = Node.opBorder.min_y + y_shift * 2;
		result.opBorder.max_z = Node.opBorder.max_z;
		break;
	case 6:
		result.nodeLoc.x = Node.opBorder.min_x + x_shift * 3;
		result.nodeLoc.y = Node.opBorder.min_y + y_shift * 3;
		result.nodeLoc.z = Node.opBorder.min_z + z_shift * 3;
		result.opBorder.min_x = Node.opBorder.min_x + x_shift * 2;
		result.opBorder.min_y = Node.opBorder.min_y + y_shift * 2;
		result.opBorder.min_z = Node.opBorder.min_z + z_shift * 2;
		result.opBorder.max_x = Node.opBorder.max_x;
		result.opBorder.max_y = Node.opBorder.max_y;
		result.opBorder.max_z = Node.opBorder.max_z;
		break;
	case 7:
		result.nodeLoc.x = Node.opBorder.min_x + x_shift;
		result.nodeLoc.y = Node.opBorder.min_y + y_shift * 3;
		result.opBorder.min_x = Node.opBorder.min_x;
		result.opBorder.min_y = Node.opBorder.min_y + y_shift * 2;
		result.opBorder.min_z = Node.opBorder.min_z + z_shift * 2;
		result.opBorder.max_x = Node.opBorder.min_x + x_shift * 2;
		result.opBorder.max_y = Node.opBorder.max_y;
		result.opBorder.max_z = Node.opBorder.max_z;
		break;
	}
	return result;
}

//distFunction
double OCTDistFunction(OCTLocation a, OCTLocation b) {
	double distance = sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2) + pow((a.z - b.z),2));
	return distance;
}

//outOfBounds Function 
bool OCTOutOfBoundsFunction(OCTOperationBorder border, OCTLocation location) {
	return ((location.x < border.min_x) 
		|| (location.x > border.max_x) 
		|| (location.y < border.min_y) 
		|| (location.y > border.max_y) 
		|| (location.z < border.min_z) 
		|| (location.z > border.max_z));
}

FPackage<OCTLocation, OCTLocation, OCTOperationBorder> getOCTFPackage() {
	FPackage<OCTLocation, OCTLocation, OCTOperationBorder> OCTFPackage;
	OCTFPackage.nodeDivideFunction = { OCTNodeDivideFunction };
	OCTFPackage.outOfBoundsFunction = { OCTOutOfBoundsFunction };
	OCTFPackage.payloadDivideFunction = { OCTPayloadDivideFunction };
	return OCTFPackage;
}

OCTLocation getOCTNodeLocation(OCTOperationBorder border) {
	double dx = border.max_x - border.min_x;
	double dy = border.max_y - border.min_y;
	double dz = border.max_z - border.min_z;
	OCTLocation l;
	l.x = dx / 2.0;
	l.y = dy / 2.0;
	l.z = dz / 2.0;
	return l;
}