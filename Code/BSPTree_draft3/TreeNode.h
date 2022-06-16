#pragma once
#include "dList.h"
#include "cLocation.h"

template <class Payload, class Location, int dimension>
class BSPTreeNode {
protected:
	dList<BSPTreeNode> * childs;
	int maxchildcount;
	Location * NodeLocation; //this can be a point, a plane or a room: The node wil check for that and adapt
	bool NodeIsLeaf;
	dList<Payload> * payload;
public:
	BSPTreeNode(dList<Payload>* p);	//Constructor
	Location getLocation() { return NodeLocation; };
	dList<Payload> * getPayload() { return payload; };
	bool isLeaf() { return NodeIsLeaf; };
};

template <class Payload, class Location, int dimension>
class kdTreeNode : protected BSPTreeNode<Payload, Location, dimension>{
protected:
	int NodeDividingAxis; //axis at witch the node divides the dataset
public:
	void divide();
};



template <class Payload, class Location>
class QcTreeNode : protected BSPTreeNode<Payload, Location, 8> {
protected:
	double width; //width/height/depth of the Node cube
public:

	void divide();
};



//Implementation of BSPTreeNode
template <class Payload, class Location, int dimension>
BSPTreeNode<Payload, Location, dimension>::BSPTreeNode(dList<Payload>* p) {
	isLeaf = true; //true because there is not any childs added jet
	this->maxchildcount = dimension;
}





