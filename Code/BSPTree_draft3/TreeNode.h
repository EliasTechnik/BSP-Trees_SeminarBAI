#pragma once
#include "dList.h"
#include <limits>
#include "helpers.h"

//for testing:
#include <iostream>

enum class BSPTreeNodeState{sEmpty,sBranch,sLeaf,sEmptyConfigured};

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
};

template <class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
class BSPTreeNode {
protected:
	BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree> *childs;	//array which holds child nodes	//created once at subdivide()
	BSPTreeNode* parent;	//pointer to the parent
	FPackage<Location, NodeLocation, OperationBorder> customFunctions; //Function Pointer Package	
	dList<PLPackage<Payload,Location>> *nodePayload;  //althougth the size is dynamic, it gets initialized to hold only PayloadLimit
	NLPackage<NodeLocation, OperationBorder> nodeLoc; //this can be a point, a plane or a room, the node does not care
	BSPTreeNodeState nodeState;		//current state of the node
	unsigned int payloadLimit;	 //how much payload can one node hold until it subdivides
	void cleanup();	//checks the childs, if the are all empty leves then cleanup is performed and the leaves removed
	void subdivide(); //subdivides the node
	bool addItemToChild(PLPackage<Payload, Location> p);
public:
	BSPTreeNode();
	BSPTreeNode(
		unsigned int _PayloadLimit, 
		NLPackage<NodeLocation,OperationBorder> _NodeLoc, 
		FPackage<Location, NodeLocation, OperationBorder> _customFunctions);	//Default Constructor
	~BSPTreeNode(); //Destructor: called by parent when the sum of all elements of all childs are less the PayloadLimit
	NLPackage<NodeLocation, OperationBorder> getNodeLocation() { return nodeLoc; };  //returns the Location of the Node
	bool isLeaf(); //true if it is a leaf
	BSPTreeNode* getChild(unsigned int index);	//gets the child at index in this node (or void if it isnt a leaf
	BSPTreeNode* getParent();			//gets the parent Node
	BSPTreeNode* getNodeToPayload(PLPackage<Payload, Location> p);	 //returns the node owning this Payload
	bool addPayload(PLPackage<Payload,Location> p);		//adds Payload to the tree and subdivides it if necesarry (should only used by the top node) 
	Payload getPayload(Location l);	//finds the payload at that location
	DistResult<Payload, Location, NodeLocation, OperationBorder, degree> getNearestPayload(Location l, double (*distFunction)(Location a, Location b), double minDist = 0.0); //finds the nearest payload to that location whith distance is greater or equal to minDist 
	Payload getPayload(unsigned int index);
	int getPayloadCount();
	bool deletePayload(PLPackage<Payload, Location>); //true if the Payload was deleted //issues cleanup
	BSPTreeNode* getRoot();
	void clean(); //deletes payload and child from the bottom up
	void setPayloadLimit(unsigned int _PayloadLimit);
	void setNodeLocation(NLPackage<NodeLocation, OperationBorder> _nodeLoc);
	void setCustomFunctions(FPackage<Location, NodeLocation, OperationBorder> _customFunctions);
	void setParent(BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree> *_Parent);
};

//subdivide
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
void BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::subdivide()
{
	NLPackage<NodeLocation, OperationBorder> np;
	childs = new BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>[degree]; //allocate space for the childs //this must happen here because otherwise the tree would not end
	for (unsigned int i = 0; i < degree; i++) {
		np = customFunctions.nodeDivideFunction(this->nodeLoc, i);			  
		this->childs[i].setPayloadLimit(this->payloadLimit);
		this->childs[i].setNodeLocation(np);
		this->childs[i].setCustomFunctions(this->customFunctions);
		this->childs[i].setParent(this);
		std::cout << "configured Child No. " << i <<std::endl;
	}
	//at this point the payload should be distributed to the childs	becaus the node is no longer a leaf
	for(unsigned int i = 0; i < this->nodePayload->getItemCount(); i++) {
		this->addItemToChild(this->nodePayload->getItem(i));
	}
	nodeState = BSPTreeNodeState::sBranch;
	delete this->nodePayload;
}

//addItemToChild
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
bool BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::addItemToChild(PLPackage<Payload, Location> p)
{
	BSPTreeNodeDivisionArg<Location, NodeLocation> arg;
	arg.NodeLocation = this->nodeLoc.nodeLoc;
	arg.PayloadLocation = p.point;
	unsigned int childID = customFunctions.payloadDivideFunction(arg);
	std::cout << "Try adding Payload to child No. " << childID << std::endl;
	return this->childs[childID].addPayload(p);
}

//Default constructor
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::BSPTreeNode()
{
	log("BSPTreeNode: Default constructor called.");
	this->nodeState = BSPTreeNodeState::sEmpty;
}

//Constructor extern (root)
template <class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
BSPTreeNode<Payload, Location, NodeLocation,OperationBorder, degree>::BSPTreeNode(
	unsigned int _PayloadLimit, 
	NLPackage<NodeLocation, OperationBorder> _NodeLoc,
	FPackage<Location, NodeLocation, OperationBorder> _customFunctions)
{
	this->parent = nullptr;
	this->payloadLimit = _PayloadLimit;
	this->nodeLoc = _NodeLoc;
	this->customFunctions = _customFunctions;
	this->nodeState = BSPTreeNodeState::sEmpty;
}

//Destructor
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::~BSPTreeNode() {
	log("Tree Node Deconstructor call");
	if (!(this->nodeState == BSPTreeNodeState::sEmpty)) {
		log("The TreeNode is not in an empty state...");
		this->clean();
	}
}

template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
bool BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::isLeaf()
{
	return this->nodeState == BSPTreeNodeState::sLeaf;
}

//getChild
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>* 
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::getChild(unsigned int index)
{
	if (this->nodeState == BSPTreeNodeState::sBranch) {
		return this->childs[index];
	}
	else {
		return nullptr;
	}
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
	BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree> *result = nullptr;
	if (this->nodeState == BSPTreeNodeState::sLeaf) {
		dListSerchResult<PLPackage<Payload, Location>> res = this->nodePayload->findItem(p);
		if (res.succes) {
			result = this;
			return result;	//we found it in this node
		};
	}
	else {
		for (int i = 0; i < degree; i++) {
			result = this->childs[i].getNodeToPayload(p);
			if (result != nullptr) {
				return result; //we found it in a child node
			}
		}
		return result; //we found nothing
	}
}	

//addPayload
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
bool BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::addPayload(PLPackage<Payload, Location> p)
{
	if ((this->nodeState == BSPTreeNodeState::sEmpty) || (this->nodeState == BSPTreeNodeState::sEmptyConfigured)) {
		this->nodePayload = new dList<PLPackage<Payload, Location>>(payloadLimit);
		this->nodeState = BSPTreeNodeState::sLeaf;
	}
	//adds Payload to the tree and subdivides it if necesarry (should only used by the top node)
	bool result = false;
	//Check if Payload is out of bound
	if (this->customFunctions.outOfBoundsFunction(this->nodeLoc.opBorder, p.point)) {
		//check if we have a parent
		if (this->parent != nullptr) {
			//give the Payload to the Parent
			log("addPayload: Item belongs to parent or higher node.");
			result = this->parent->addPayload(p);
		}
		else {
			log("addPayload: Item out of bounds.");
			return false; //We return false because ther is no parent and the bounds are exceded for that item
		}
	}
	else {
		if (this->nodeState == BSPTreeNodeState::sBranch) {
			log("addPayload: Item belongs to leave...");
			result = this->addItemToChild(p);
		}
		else {
			//is Leaf
			//check for itemLimit
			if (this->nodePayload->getItemCount() >= (this->payloadLimit - 1)) {
				//subdivide	Node
				log("addPayload: Item belongs this node but we are full so we subdivide.");
				this->subdivide();
				result = this->addItemToChild(p);
			}
			else {
				this->nodePayload->addItem(p);
				log("addPayload: Item belongs to this node.");
				result = true;
			}
		}
	}
	return result;
}

template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
Payload BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::getPayload(Location l)
{
	//check if this node is leave
	if (this->nodeState == BSPTreeNodeState::sLeaf) {
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
	if (this->nodeState == BSPTreeNodeState::sLeaf) {
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
	if (this->nodeState == BSPTreeNodeState::sLeaf) {
		return this->nodePayload->getItem(index);
	}
	else {
		return nullptr;
	}
}

template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
int BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::getPayloadCount()
{
	if (this->nodeState == BSPTreeNodeState::sLeaf) {
		return this->nodePayload->getItemCount();
	}
	else {
		return 0;
	}
}

template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
bool BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::deletePayload(PLPackage<Payload, Location>)
{
	if (this->nodeState == BSPTreeNodeState::sLeaf) {
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

template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
void BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::clean()
{
	if (!(this->nodeState == BSPTreeNodeState::sEmpty)) { //check because this might be called out of recursion and not from the deconstructor
		log("Cleaning started...");
		if (this->nodeState == BSPTreeNodeState::sBranch) {
			log("Cleaning the apended childs...");
			for (unsigned int i = 0; i < degree; i++) {
				this->childs[i].clean();
			}
			log("deleting the childs");
			delete[] this->childs;
		}
		else {
			if (this->nodeState == BSPTreeNodeState::sLeaf) { //obvious //might be removed
				log("Deleting the Payload...");
				delete this->nodePayload;
			}
		}
		this->nodeState = BSPTreeNodeState::sEmpty;
	}
}

template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
void BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::setPayloadLimit(unsigned int _PayloadLimit)
{
	this->payloadLimit = _PayloadLimit;
	this->nodeState = BSPTreeNodeState::sEmptyConfigured;
}

template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
void BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::setNodeLocation(NLPackage<NodeLocation, OperationBorder> _nodeLoc)
{
	this->nodeLoc = _nodeLoc;
	this->nodeState = BSPTreeNodeState::sEmptyConfigured;
}

template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
void BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::setCustomFunctions(FPackage<Location, NodeLocation, OperationBorder> _customFunctions)
{
	this->customFunctions = _customFunctions;
	this->nodeState = BSPTreeNodeState::sEmptyConfigured;
}

template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
void BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::setParent(BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree> *_Parent)
{
	this->parent = _Parent;
	this->nodeState = BSPTreeNodeState::sEmptyConfigured;
}




