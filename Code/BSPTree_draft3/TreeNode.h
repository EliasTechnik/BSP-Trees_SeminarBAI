#pragma once
#include "dList.h"

struct location_2d{
	float x;
	float y;
};

template <class Payload, class Location, int dimension>
class TreeNode {
protected:
	dList<TreeNode> * childs;
	int maxchildcount;
	Location * NodeLocation;
	bool NodeIsLeaf;
	dList<Payload> * payload;
public:
	TreeNode();	//Constructor
	Location getLocation() { return NodeLocation; };
	dList<Payload> * getPayload() { return payload; };
	bool isLeaf() { return NodeIsLeaf; };
	void divide();
};

template <class Payload, class Location, int dimension>
TreeNode<Payload, Location, dimension>::TreeNode() {
	isLeaf = true; //true because there is not any childs added jet
	maxchildcount = dimension;
}

template <class Payload, class Location, int dimension>
void TreeNode<Payload, Location, dimension>::divide() {
	Payload->getLocation();
}
