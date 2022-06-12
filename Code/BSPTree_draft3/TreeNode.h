#pragma once
#include <stdlib.h> //for malloc()
#include <cstring> //for memcpy()


struct location_2d{
	float x;
	float y;
};

template <class Payload>
class dList {
private:
	Payload * items; //pointer to the first item in memory
	int Count;	   //actual used space	
	int allocSize; //allocated memory space //might be useless to store
	void realloc_mem(int newsize);
	int stepSize; //size with which the list is expanded
public:
	dList(int size = 256); //constructor
	//TODO: Destructor
	int addItem(Payload item); //adds one Item to the list
	Payload * getItem(int index); //get Item at position x
	int getItemCount() { return Count; }; //count of items
};



template <class Payload, class Location, int dimension>
class TreeNode {
protected:
	TreeNode childs[dimension];
	int maxchildcount;
	Location* NodeLocation;
	bool NodeIsLeaf;
	Payload* payload;
public:
	TreeNode();	//Constructor
	Location getLocation() { return NodeLocation; };
	Payload getPayload() { return payload; };
	bool isLeaf() { return NodeIsLeaf; };
	void divide();
};

template <class Payload>
dList<Payload>::dList(int size) {
	Count = 0;
	stepSize = size;
	realloc_mem(stepSize);
}

template <class Payload>
void dList<Payload>::realloc_mem(int newsize) {
	Payload* newitems = new Payload[newsize]; //throws "bad_alloc" if it fails 
	memcpy(newitems, items, allocSize); //copy old arry to new destination
	delete[] items;	//free's old memory
	items = newitems;
	allocSize = newsize;
}

template <class Payload>
int dList<Payload>::addItem(Payload item) {
	if (Count + 1 > allocSize) {
		realloc_mem(allocSize + stepSize);
	}
	items[Count] = item;
	Count += 1;
	return Count - 1;
}

template <class Payload>
Payload * dList<Payload>::getItem(int index) {
	if (index >= Count) {
		return NULL;
	}
	else {
		return &items[index];
	}
}


template <class Payload, class Location, int dimension>
TreeNode<Payload, Location, dimension>::TreeNode() {
	isLeaf = true; //true because there is not any childs added jet
	maxchildcount = dimension;
}

template <class Payload, class Location, int dimension>
void TreeNode<Payload, Location, dimension>::divide() {

}
