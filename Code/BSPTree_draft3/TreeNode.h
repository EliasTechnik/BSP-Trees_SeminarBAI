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
	unsigned int level = 0; //The level of this node within the tree
	unsigned int childID = 0;  //this might be set for childs or not
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

void printNodeState(BSPTreeNodeState state) {
	switch (state) {
	case BSPTreeNodeState::sEmpty:
		log("NodeState: sEmpty");
		break;
	case BSPTreeNodeState::sBranch:
		log("NodeState: sBranch");
		break;
	case BSPTreeNodeState::sLeaf:
		log("NodeState: sLeaf");
		break;
	case BSPTreeNodeState::sEmptyConfigured:
		log("NodeState: sEmptyConfigured");
		break;
	default:
		log("NodeStaete: Unknown!");
	}
}

template <class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
class BSPTreeNodeRoot {
private: 
	FPackage<Location, NodeLocation, OperationBorder> customFunctions; //Function Pointer Package
	unsigned int payloadLimit;
protected:
	BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>* childs;	//array which holds child nodes	//created once at subdivide()
	dList<PLPackage<Payload, Location>>* nodePayload;  //althougth the size is dynamic, it gets initialized to hold only PayloadLimit
	NLPackage<NodeLocation, OperationBorder> nodeLoc; //this can be a point, a plane or a room, the node does not care
	BSPTreeNodeState nodeState;		//current state of the node
	void subdivide(); //subdivides the node
	bool addItemToChild(PLPackage<Payload, Location> p);
public:
	BSPTreeNodeRoot();
	BSPTreeNodeRoot(
		unsigned int _PayloadLimit,
		NLPackage<NodeLocation, OperationBorder> _NodeLoc,
		FPackage<Location, NodeLocation, OperationBorder> _customFunctions);	//Root Constructor
	~BSPTreeNodeRoot(); //Destructor of the root or whole tree
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
	void subdivide(); //subdivides the node
	bool addItemToChild(PLPackage<Payload, Location> p);
public:
	BSPTreeNode();
	BSPTreeNode(
		unsigned int _PayloadLimit, 
		NLPackage<NodeLocation,OperationBorder> _NodeLoc, 
		FPackage<Location, NodeLocation, OperationBorder> _customFunctions);	//Root Constructor
	~BSPTreeNode(); //Destructor: called by parent when the sum of all elements of all childs are less the PayloadLimit
	NLPackage<NodeLocation, OperationBorder> getNodeLocation() { return nodeLoc; };  //returns the Location of the Node
	bool isLeaf(); //true if it is a leaf
	BSPTreeNode* getChild(unsigned int index);	//gets the child at index in this node (or void if it isnt a leaf
	BSPTreeNode* getParent();			//gets the parent Node
	BSPTreeNode* getNodeToPayload(PLPackage<Payload, Location> p, bool (*compareMethod)(PLPackage<Payload, Location> a, PLPackage<Payload, Location> b));	 //returns the node owning this Payload
	BSPTreeNode* getNodeToLocation(Location l);
	bool addPayload(PLPackage<Payload,Location> p);		//adds Payload to the tree and subdivides it if necesarry 
	bool addPayload(dList<PLPackage<Payload, Location>> *p); //adds Payload to the tree and subdivides it if necesarry 
	Payload getPayload(Location l);	//finds the payload at that location
	DistResult<Payload, Location, NodeLocation, OperationBorder, degree> getNearestPayload(Location l, double (*distFunction)(Location a, Location b), double minDist = 0.0); //finds the nearest payload to that location whith distance is greater or equal to minDist 
	Payload getNodePayload(unsigned int index);
	dList<PLPackage<Payload, Location>>* getPayload(); //gethers all payload under this node;
	unsigned int getNodePayloadCount();
	unsigned int getTreePayloadCount(unsigned int offset=0);
	bool deletePayload(unsigned int index); //true if the Payload was deleted //issues cleanup
	BSPTreeNode* getRoot();
	void clean(); //deletes payload and child from the bottom up
	void removeChilds(); //removes childs under that node and converts it to sLeaf
	void setPayloadLimit(unsigned int _PayloadLimit);
	void setNodeLocation(NLPackage<NodeLocation, OperationBorder> _nodeLoc);
	void setCustomFunctions(FPackage<Location, NodeLocation, OperationBorder> _customFunctions);
	void setParent(BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree> *_Parent);
	void cleanupTree();	//checks the childs, if the combined payload is whithin the limit, the tree gets reduced
};

//cleanupTree
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
void BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::cleanupTree()
{
	if (this->nodeState == BSPTreeNodeState::sBranch) {
		log("we are branch so cleanup is approved.");
		//check if we have any data in childs
		unsigned int payloadcount = this->getTreePayloadCount();
		std::cout << "Payloadcount: " << payloadcount << std::endl;
		if (payloadcount <= this->payloadLimit) {
			log("going to remove childs...");
			dList<PLPackage<Payload, Location>> *tempPayload;
			tempPayload = this->getPayload();
			
			this->removeChilds();
			this->addPayload(tempPayload);
			delete tempPayload;
			log("removed one branch off");
		}
		else {
			log("payloadcount is still to high.");
			for (unsigned int i = 0; i < degree; i++) {
				this->childs[i].cleanupTree();
			}
		}
	}
}

//subdivide
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
void BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::subdivide()
{
	NLPackage<NodeLocation, OperationBorder> np;
	childs = new BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>[degree]; //allocate space for the childs //this must happen here because otherwise the tree would not end
	for (unsigned int i = 0; i < degree; i++) {
		np = customFunctions.nodeDivideFunction(this->nodeLoc, i);	
		np.level = this->nodeLoc.level + 1; //overwrite it here because the user might get it wrong
		np.childID = i;
		this->childs[i].setPayloadLimit(this->payloadLimit);
		this->childs[i].setNodeLocation(np);
		this->childs[i].setCustomFunctions(this->customFunctions);
		this->childs[i].setParent(this);
		//std::cout << "configured Child No. " << i <<std::endl;
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
	//std::cout << "Try adding Payload to child No. " << childID << std::endl;
	return this->childs[childID].addPayload(p);
}

//Default constructor
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::BSPTreeNode()
{
	//log("BSPTreeNode: Default constructor called.");
	this->nodeState = BSPTreeNodeState::sEmpty;
}

//Constructor extern (root)
template <class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
BSPTreeNode<Payload, Location, NodeLocation,OperationBorder, degree>::BSPTreeNode(
	unsigned int _PayloadLimit, 
	NLPackage<NodeLocation, OperationBorder> _NodeLoc,
	FPackage<Location, NodeLocation, OperationBorder> _customFunctions)
{
	log("external Constructor call.");
	this->parent = nullptr;
	this->payloadLimit = _PayloadLimit;
	this->nodeLoc = _NodeLoc;
	this->customFunctions = _customFunctions;
	this->nodeState = BSPTreeNodeState::sEmpty;
	log("external Constructor done.");
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
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::getNodeToPayload(
	PLPackage<Payload, Location> p, 
	bool (*compareMethod)(PLPackage<Payload, Location> a, PLPackage<Payload, Location> b))
{
	//returns the node owning this Payload
	BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree> *result = nullptr;
	//printNodeState(this->nodeState);

	if (this->nodeState == BSPTreeNodeState::sLeaf) {
		dListSearchResult<PLPackage<Payload, Location>> res = this->nodePayload->findItem(p, compareMethod);
		if (res.succes) {
			result = this;
			return result;	//we found it in this node
		};
	}
	else {
		if(this->nodeState==BSPTreeNodeState::sBranch){
			for (unsigned int i = 0; i < degree; i++) {
				result = this->childs[i].getNodeToPayload(p, compareMethod);
				if (result != nullptr) {
					return result; //we found it in a child node
				}
			}
		}
		return result; //we found nothing
	}
}

//getNodeToLocation
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>* 
BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::getNodeToLocation(Location l)
{
	if (customFunctions.outOfBoundsFunction(this->nodeLoc.opBorder, l)) {
		return nullptr;
	}
	if (this->nodeState == BSPTreeNodeState::sBranch) {
		BSPTreeNodeDivisionArg<Location, NodeLocation> arg;
		arg.NodeLocation = this->nodeLoc.nodeLoc;
		arg.PayloadLocation = l;
		return this->childs[this->customFunctions.payloadDivideFunction(arg)].getNodeToLocation(l);
	}
	else {
		return this;
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
		log("addPayload: Item is out of bounds for that node.");
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
		if (this->nodeState == BSPTreeNodeState::sLeaf) {
			//is Leaf
			//check for itemLimit
			if (this->nodePayload->getItemCount() == this->payloadLimit) {
				//subdivide	Node
				log("addPayload: Item belongs this node but we are full so we subdivide.");
				this->subdivide();
				result = this->addItemToChild(p);
			}
			else {
				this->nodePayload->addItem(p);
				//std::cout << "Node [" << this->nodeLoc.level << "; " << this->nodeLoc.childID << "] saved payload." << std::endl;
				result = true;
			}
		}
	}
	return result;
}

//addPayload (dlist)
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
bool BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::addPayload(dList<PLPackage<Payload, Location>> *p)
{
	bool result = true;
	for (unsigned int i = 0; i < p->getItemCount(); i++) {
		if (!(this->addPayload(p->getItem(i)))) {
			result = false;
		}
	}
	return result;
}

//getPayload
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

//getNearestPayload
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

//getNodePayload
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
Payload BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::getNodePayload(unsigned int index)
{
	if (this->nodeState == BSPTreeNodeState::sLeaf) {
		return this->nodePayload->getItem(index);
	}
	else {
		return nullptr;
	}
}

//getPayload (dList)
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
dList<PLPackage<Payload, Location>>* BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::getPayload()
{
	dList<PLPackage<Payload, Location>> *res = new dList<PLPackage<Payload, Location>>(this->payloadLimit);
	if (this->nodeState == BSPTreeNodeState::sLeaf) {
		for (unsigned int i = 0; i < this->nodePayload->getItemCount(); i++) {
			res->addItem(this->nodePayload->getItem(i));
		}
	}
	if (this->nodeState == BSPTreeNodeState::sBranch) {
		for (unsigned int i = 0; i < degree; i++) {
			res->addList(this->childs[i].getPayload());
		}
	}
	return res;
}

//getNodePayloadCount
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
unsigned int BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::getNodePayloadCount()
{
	printNodeState(this->nodeState);
	if (this->nodeState == BSPTreeNodeState::sLeaf) {
		return this->nodePayload->getItemCount();
	}
	else {
		return 0;
	}
}

//getTreePaloadCount
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
unsigned int BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::getTreePayloadCount(unsigned int offset)
{
	switch (this->nodeState) {
	case BSPTreeNodeState::sLeaf:
		return offset + this->nodePayload->getItemCount();
		break;
	case BSPTreeNodeState::sBranch:
		for (unsigned int i = 0; i < degree; i++) {
			offset = this->childs[i].getTreePayloadCount(offset);
		}
		return offset;
		break;
	default:
		return offset;
	}
}

//deletePayload
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
bool BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::deletePayload(unsigned int index)
{
	log("delete call.");
	if (this->nodeState == BSPTreeNodeState::sLeaf) {
		if (this->nodePayload->getItemCount() > index) {
		    this->nodePayload->deleteItem(index);
			return true;
		}
		return false;
	}
	else {
		return false;
	}
}

//getRoot
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

//clean
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

//remove childs
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
void BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::removeChilds()
{
	log("removeChilds call.");
	if (this->nodeState == BSPTreeNodeState::sBranch) {
		this->nodeState = BSPTreeNodeState::sEmptyConfigured;
		for (unsigned int i = 0; i < degree; i++) {
			this->childs[i].clean();
		}
		delete[] this->childs;
	}
}

//setPayloadLimit
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
void BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::setPayloadLimit(unsigned int _PayloadLimit)
{
	this->payloadLimit = _PayloadLimit;
	this->nodeState = BSPTreeNodeState::sEmptyConfigured;
}

//setNodeLocation
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
void BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::setNodeLocation(NLPackage<NodeLocation, OperationBorder> _nodeLoc)
{
	this->nodeLoc = _nodeLoc;
	this->nodeState = BSPTreeNodeState::sEmptyConfigured;
}

//setCustomFunctions
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
void BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::setCustomFunctions(FPackage<Location, NodeLocation, OperationBorder> _customFunctions)
{
	this->customFunctions = _customFunctions;
	this->nodeState = BSPTreeNodeState::sEmptyConfigured;
}

//setParent
template<class Payload, class Location, class NodeLocation, class OperationBorder, unsigned int degree>
void BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree>::setParent(BSPTreeNode<Payload, Location, NodeLocation, OperationBorder, degree> *_Parent)
{
	this->parent = _Parent;
	this->nodeState = BSPTreeNodeState::sEmptyConfigured;
}




