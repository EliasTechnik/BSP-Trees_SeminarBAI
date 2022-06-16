#pragma once

#include "TreeNode.h"

enum class QuadTreeDivisionType { southwest, southeast, northeast, northwest };

template <class Payload>
class QuadTreeNode : protected BSPTreeNode<Payload, point2d, 4> {
protected:
	square2d boundary; //the boundary of the square. NodeLocation is computed from that
	BSPTreeNodeDivision<QuadTreeNode, 4> (*dividingFunction)(point2d, dlist<Payload>); //functionpointer to the apropiate dividing function
public:
	//using BSPTreeNode<Payload, point2d, 4>::BSPTreeNode;
	QuadTreeNode(dList<Payload>* p, square2d _boundary, BSPTreeNodeDivisionType (*_dividingFunction)(point2d, Payload)); //bool (*comparisonFcn)(int, int)
	void divide();

};

//Implementation of QuadTreeNode
//Constructor
template <class Payload>
QuadTreeNode<Payload>::QuadTreeNode(dList<Payload>* p, square2d _boundary, BSPTreeNodeDivision<QuadTreeNode, 4>(*_dividingFunction)(point2d, dList<Payload>)) {
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
	dividingFunction = _dividingFunction;
}

template<class Payload>
QuadTreeNode<Payload>::QuadTreeNode(dList<Payload>* p, square2d _boundary, point2d(*dividingFunction)(point2d, dlist<Payload>))
{

}

template <class Payload>
void QuadTreeNode<Payload>::divide() {
	dList<Payload>* north = new dList<Payload>(payload.Count());

}
