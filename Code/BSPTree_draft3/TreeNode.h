#pragma once
#include "dList.h"
#include <limits>

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

template <class Location, class NodeLocation, class OperationBorder>
struct FPackage { //Package to group all functionpointers necesary for correct operation
	int (*payloadDivideFunction)(BSPTreeNodeDivisionArg<Location, NodeLocation>);	//functionpointer to the divide function
	NLPackage<NodeLocation, OperationBorder>(*nodeDivideFunction)(NLPackage<NodeLocation, OperationBorder>, int);	//functionpointer to the subdivide function	
	bool (*outOfBoundsFunction)(OperationBorder border, Location location); //returns true if the location is outside the border
};

template <class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
struct DistResult {	//used for finding the neares neighbor
	double distance;
	Payload data;
	//BSPTreeNode* owner;
};

template <class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
class BSPTreeNode {
protected:
	BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree> *childs[degree];	//holds child nodes
	BSPTreeNode* parent;	//pointer to the parent
	FPackage<Location, NodeLocation, OperationBorder> customFunctions; //Function Pointer Package	
	dList<PLPackage<Payload,Location>> *nodePayload;  //althougth the size is dynamic, it gets initialized to hold only PayloadLimit
	NLPackage<NodeLocation, OperationBorder> nodeLoc; //this can be a point, a plane or a room, the node does not care
	bool nodeIsLeaf;
	unsigned int payloadLimit;	 //how much payload can one node hold until it subdivides
	void cleanup();	//checks the childs, if the are all leves then cleanup is performed
	void subdivide(); //subdivides the node
	BSPTreeNode* addItemToChild(PLPackage<Payload, Location> p);
public:
	BSPTreeNode(
		unsigned int _PayloadLimit, 
		NLPackage<NodeLocation,OperationBorder> _NodeLoc, 
		FPackage<Location, NodeLocation, OperationBorder> _customFunctions);	//Default Constructor
	BSPTreeNode(
		unsigned int _PayloadLimit, 
		NLPackage<NodeLocation, OperationBorder>, 
		FPackage<Location, NodeLocation, OperationBorder> _customFunctions,
		BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree> _Parent); //Constructor for childs
	~BSPTreeNode(); //Destructor: called by parent when the sum of all elements of all childs are less the PayloadLimit
	NLPackage<NodeLocation, OperationBorder> getNodeLocation() { return nodeLoc; };  //returns the Location of the Node
	bool isLeaf() { return nodeIsLeaf; }; //true if it is a leaf
	BSPTreeNode* getChild(unsigned int index);	//gets the child at index in this node (or void if it isnt a leaf
	BSPTreeNode* getParent();			//gets the parent Node
	BSPTreeNode* getNodeToPayload(PLPackage<Payload, Location> p);	 //returns the node owning this Payload
	BSPTreeNode* addPayload(PLPackage<Payload,Location> p);		//adds Payload to the tree and subdivides it if necesarry (should only used by the top node) 
	Payload getPayload(Location l);	//finds the payload at that location
	DistResult<Payload, Location, NodeLocation, OperationBorder, degree> getNearestPayload(Location l, double (*distFunction)(Location a, Location b), double minDist = 0.0); //finds the nearest payload to that location whith distance is greater or equal to minDist 
	Payload getPayload(unsigned int index);
	int getPayloadCount();
	bool deletePayload(PLPackage<Payload, Location>); //true if the Payload was deleted //issues cleanup
	BSPTreeNode* getRoot();
};

//subdivide
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
void BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::subdivide()
{
	NLPackage<NodeLocation, OperationBorder> np;
	for (unsigned int i = 0; i < degree; i++) {
		np = customFunctions.nodeDivideFunction(this->nodeLoc, i);
		childs[i] = new BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>(
			this->payloadLimit,
			np,
			this->customFunctions,
			*this);
	}
	this->nodeIsLeaf = false;
	//at this point the payload should be distributed to the childs	becaus the node is no longer a leaf

	for(unsigned int i = 0; i < this->nodePayload->getItemCount(); i++) {
		this->addItemToChild(*this->nodePayload->getItem(i));
	}
	delete this->nodePayload; 
}

//addItemToChild
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>*
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::addItemToChild(PLPackage<Payload, Location> p)
{
	BSPTreeNodeDivisionArg<Location, NodeLocation> arg;
	arg.NodeLocation = this->nodeLoc.nodeLoc;
	arg.PayloadLocation = p.point;
	return this->childs[customFunctions.payloadDivideFunction(arg)]->addPayload(p);
}

//Constructor 1
template <class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
BSPTreeNode<Payload, Location, NodeLocation,OperationBorder, degree>::BSPTreeNode(
	unsigned int _PayloadLimit, 
	NLPackage<NodeLocation, OperationBorder> _NodeLoc,
	FPackage<Location, NodeLocation, OperationBorder> _customFunctions)
{
	this->nodeIsLeaf = true; //true because there is not any childs added jet
	this->parent = nullptr;
	this->payloadLimit = _PayloadLimit;
	this->nodeLoc = _NodeLoc;
	this->nodePayload = new dList<PLPackage<Payload, Location>>(payloadLimit);
	this->customFunctions = _customFunctions;
}

//Constructor 2
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::BSPTreeNode(
	unsigned int _PayloadLimit,
	NLPackage<NodeLocation, OperationBorder> _NodeLoc,
	FPackage<Location, NodeLocation, OperationBorder> _customFunctions,
	BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree> _Parent)
{
	this->nodeIsLeaf = true; //true because there is not any childs added jet
	this->parent = &_Parent;
	this->payloadLimit = _PayloadLimit;
	this->nodeLoc = _NodeLoc;
	this->nodePayload = new dList<PLPackage<Payload, Location>>(payloadLimit);
	this->customFunctions = _customFunctions;
}

//Destructor
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::~BSPTreeNode() {
	delete this->nodePayload;
	for (unsigned int i = 0; i < degree; i++) {
		delete this->childs[i];
	}
}

//getChild
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>* 
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::getChild(unsigned int index)
{
	return this->childs[index];
}

//getParent
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>*
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::getParent()
{
	return this->parent;
}

//getNodeToPayload
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>*
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::getNodeToPayload(PLPackage<Payload, Location> p)
{
	//returns the node owning this Payload
	BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree> result = nullptr;
	if (this->nodeIsLeaf) {
		if (this->nodePayload->findItem(p) != nullptr) {
			result = this;
			return result;	//we found it in this node
		};
	}
	else {
		for (int i = 0; i < degree; i++) {
			result = this->childs[i]->getNodeToPayload(p);
			if (result != nullptr) {
				return result; //we found it in a child node
			}
		}
		return result; //we found nothing
	}
}	

//addPayload
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>*
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::addPayload(PLPackage<Payload, Location> p)
{
	//adds Payload to the tree and subdivides it if necesarry (should only used by the top node)
	BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree> *result = nullptr;
	//Check if Payload is out of bound
	if (this->customFunctions.outOfBoundsFunction(this->nodeLoc.opBorder, p.point)) {
		//check if we have a parent
		if (this->parent != nullptr) {
			//give the Payload to the Parent
			result = this->parent->addPayload(p);
		}
		else {
			return nullptr; //We return nullptr because ther is no parent and the bounds are exceded for that item
		}
	}
	else {
		if (!this->nodeIsLeaf) {
			result = this->addItemToChild(p);
		}
		else {
			//is Leaf
			//check for itemLimit
			if (this->nodePayload->getItemCount() >= this->payloadLimit - 1) {
				//subdivide	Node
				this->subdivide();
				result = this->addItemToChild(p);
			}
			else {
				this->nodePayload->addItem(p);
				result = this;
			}
		}
	}
	return result;
}

template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
Payload BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::getPayload(Location l)
{
	//check if this node is leave
	if (this->nodeIsLeaf) {
		for (unsigned int i = 0; i < this->nodePayload->getItemCount(); i++) {
			PLPackage<Payload, Location> p = this->nodePayload->getItem(i);
			if (p.point == l) {
				return p.data;
			}
		}
		return nullptr;
	}
	else {
		//determine the right child to ask
		BSPTreeNodeDivisionArg<Location, NodeLocation> arg;
		arg.NodeLocation = this->nodeLoc;
		arg.PayloadLocation = l;
		return this->childs[payloadDivideFunction(arg)]->getPayload(l);
	}
}

template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
DistResult<Payload, Location, NodeLocation, OperationBorder,degree> BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::getNearestPayload(Location l, double(*distFunction)(Location a, Location b), double minDist)
{
	DistResult<Payload> dr = nullptr;
	if (this->nodeIsLeaf) {
		double distance = std::numeric_limits<double>::max();
		for (unsigned int i = 0; i < this->nodePayload->getItemCount(); i++) {
			PLPackage<Payload, Location> pl = this->nodePayload->getItem(i);
			double d = distFunction(l, pl.point);
			if (d < distance) {
				distance = d;
				//dr = pl.data;
			}
		}
	}
	else {
	   //todo. This is not trivial and might require some complex algorithm!
	}
}

template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
Payload BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::getPayload(unsigned int index)
{
	if (this->nodeIsLeaf) {
		return this->nodePayload->getItem(index);
	}
	else {
		return nullptr;
	}
}

template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
int BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::getPayloadCount()
{
	if (this->nodeIsLeaf) {
		return this->nodePayload->getItemCount();
	}
	else {
		return 0;
	}
}

template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
bool BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::deletePayload(PLPackage<Payload, Location>)
{
	if (this->nodeIsLeaf) {
		//delete payload
	}
	else {
		//get the node containing the payload
	}
}

template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>*
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::getRoot()
{
	if (this->parent != nullptr) {
		return parent->getParent();
	}
	else {
		return this;
	}
}




