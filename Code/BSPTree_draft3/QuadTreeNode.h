#pragma once

#include "TreeNode.h"

template <class Payload>
class QuadTreeNode : protected BSPTreeNode<Payload, point2d, 4> {
protected:
	square2d boundary; //the boundary of the square. NodeLocation is computed from that
public:
	//using BSPTreeNode<Payload, point2d, 4>::BSPTreeNode;
	QuadTreeNode(dList<Payload>* p, square2d _boundary);
	void divide();

};

//Implementation of QuadTreeNode
//Constructor
template <class Payload>
QuadTreeNode<Payload>::QuadTreeNode(dList<Payload>* p, square2d _boundary) {
	maxchildcount = 4;
	if (p->Count() <= maxchildcount) {
		isLeaf = true;
	}
	else {
		isLeaf = false;
	}
	boundary = _boundary;
	NodeLocation.x = (boundary.rightBottomCorner.x - boundary.leftUpperCorner.x) / 2.0;
	NodeLocation.y = (boundary.rightBottomCorner.y - boundary.leftUpperCorner.y) / 2.0; //assumes that the y axis is inverted	   //maybe change that
	payload = p;
}

template <class Payload>
void QuadTreeNode<Payload>::divide() {
	dList<Payload>* north = new dList<Payload>(payload.Count());

}
