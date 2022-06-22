#pragma once
#include "dList.h"
#include "cLocation.h"

template <int degree> //assures typesafety
struct BSPTreeNodeDivisionResult { //returned by the division function
	int Subspace;
};

template <class Location, class NodeLocation>
struct BSPTreeNodeDivisionArg { //given to the division function
	NodeLocation NodeLocation;
	Location PayloadLocation;
};

template <class Payload, class Location>
struct PLPackage { //Package to group the Payload and the location data
	Payload data;  //actual Payload
	Location point;	 //spatial data
};

template <class NodeLocation, class OperationBorder>
struct NLPackage { //Package to group the NodeLocation and the OperationBorder together
	NodeLocation nodeLoc;		//location of the Node
	OperationBorder opBorder;	//Border within operation is possible
};

template <class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
class BSPTreeNode {
protected:
	BSPTreeNode childs[degree];	//holds child nodes
	BSPTreeNode* parent;	//pointer to the parent
	BSPTreeNodeDivisionResult<degree>(*divideFunction)(BSPTreeNodeDivisionArg<Location, NodeLocation>);	//functionpointer to the dividefunction	
	dList<PLPackage<Payload,Location>>* nodePayload;  //althougth the size is dynamic, it gets initialized to hold only PayloadLimit
	NLPackage<NodeLocation, OperationBorder> * nodeLoc; //this can be a point, a plane or a room, the node does not care
	bool nodeIsLeaf;
	unsigned int payloadLimit;	 //how much payload can one node hold until it subdivides
	void cleanup();	//checks the childs, if the are all leves then cleanup is performed
public:
	BSPTreeNode(
		unsigned int _PayloadLimit, 
		NLPackage<NodeLocation,
		OperationBorder> _NodeLoc, 
		BSPTreeNodeDivisionResult<degree>(*_divideFunction)(BSPTreeNodeDivisionArg<Location, NodeLocation>));	//Default Constructor
	BSPTreeNode(
		unsigned int _PayloadLimit, 
		NLPackage<NodeLocation, OperationBorder>, 
		BSPTreeNodeDivisionResult<degree>(*_divideFunction)(BSPTreeNodeDivisionArg<Location, NodeLocation>),
		BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree> _Parent); //Constructor for childs
	~BSPTreeNode(); //Destructor: called by parent when the sum of all elements of all childs are less the PayloadLimit
	NLPackage<NodeLocation, OperationBorder> getNodeLocation() { return nodeLoc; };  //returns the Location of the Node
	bool isLeaf() { return nodeIsLeaf; }; //true if it is a leaf
	BSPTreeNode getChild(unsigned int index);	//gets the child at index in this node (or void if it isnt a leaf
	BSPTreeNode getParent();			//gets the parent Node
	BSPTreeNode getNodeToPayload(PLPackage<Payload, Location>);	 //returns the node owning this Payload
	BSPTreeNode addPayload(PLPackage<Payload,Location> p);		//adds Payload to the tree and subdivides it if necesarry (should only used by the top node) 
	Payload getPayload(Location l);	//finds the payload at that location
	Payload getNearestPayload(Location l, double (*distFunction)(Location a, Location b), double minDist = 0.0); //finds the nearest payload to that location whith distance is greater or equal to minDist 
	Payload getPayload(unsigned int index);
	int getPayloadCount();
	bool deletePayload(PLPackage<Payload, Location>); //true if the Payload was deleted //issues sanitize
	BSPTreeNode getRoot();
};

//Implementation of BSPTreeNode
//Constructor
template <class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
BSPTreeNode<Payload, Location, NodeLocation,OperationBorder, degree>::BSPTreeNode(
	unsigned int _PayloadLimit, 
	NLPackage<NodeLocation, OperationBorder> _NodeLoc,
	BSPTreeNodeDivisionResult<degree>(*_divideFunction)(BSPTreeNodeDivisionArg<Location, NodeLocation>)) 
{
	nodeIsLeaf = true; //true because there is not any childs added jet
	parent = NULL;
	payloadLimit = _PayloadLimit;
	nodeLoc = _NodeLoc;
	nodePayload = new dList<PLPackage<Payload, Location>>(payloadLimit);
	(*divideFunction) = &_divideFunction;
}

//Constructor
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::BSPTreeNode(
	unsigned int _PayloadLimit,
	NLPackage<NodeLocation, OperationBorder> _NodeLoc,
	BSPTreeNodeDivisionResult<degree>(*_divideFunction)(BSPTreeNodeDivisionArg<Location, NodeLocation>),
	BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree> _Parent)
{
	nodeIsLeaf = true; //true because there is not any childs added jet
	parent = _Parent;
	payloadLimit = _PayloadLimit;
	nodeLoc = _NodeLoc;
	nodePayload = new dList<PLPackage<Payload, Location>>(payloadLimit);
	(*divideFunction) = &_divideFunction;
}
//Destructor
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::~BSPTreeNode() {
	nodePayload->~dList();
	for (int i = 0; i < degree; i++) {
		childs[i]->~BSPTreeNode();
	}
}

template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree> 
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::getChild(unsigned int index)
{
	return childs[index];
}

template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::getParent()
{
	return parent;
}





