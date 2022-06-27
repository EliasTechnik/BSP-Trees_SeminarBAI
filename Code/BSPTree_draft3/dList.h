#pragma once

#include "helpers.h"

template <class Payload>
class dList {//a semidynamic list of pointers 
private:
	Payload* items; //pointer to the first item in memory
	unsigned int Count;	   //actual used space	
	unsigned int allocSize; //allocated memory space //might be useless to store
	void realloc_mem(int newsize);
	unsigned int stepSize; //size with which the list is expanded
public:
	dList(unsigned int size = 256); //constructor
	~dList();//TODO: Destructor
	unsigned int addItem(Payload item); //adds one Item to the list
	Payload* getItemPointer(unsigned int index); //get Pointer to Item at position x
	Payload getItem(unsigned int index); //get Item at position x
	unsigned int findItem(Payload item); //returns the index if found or NULL
	unsigned int getItemCount() { return Count; }; //count of items
	void clear(); //clears the list and releases memory without destroing the instance
};

template <class Payload>
dList<Payload>::dList(unsigned int size) {
	this->Count = 0;
	this->stepSize = size;
	realloc_mem(this->stepSize);
}

template<class Payload>
dList<Payload>::~dList(){
	log("dlist deconstructor called.");
	delete[] this->items; //free ocupied memory
}

template <class Payload>
void dList<Payload>::realloc_mem(int newsize) {
	Payload* newitems = new Payload[newsize]; //throws "bad_alloc" if it fails 
	memcpy(newitems, this->items, sizeof(Payload) * this->allocSize); //copy old arry to new destination
	delete[] this->items;	//free's old memory
	this->items = newitems;
	this->allocSize = newsize;
	//log("expanded the list.");
}

template <class Payload>
unsigned int dList<Payload>::addItem(Payload item) {
	if (this->Count + 1 > allocSize) {
		realloc_mem(this->allocSize + this->stepSize);
	}
	this->items[this->Count] = item;
	this->Count += 1;
	return this->Count - 1;
}

template <class Payload>
Payload* dList<Payload>::getItemPointer(unsigned int index) {
	if (index >= Count) {
		return NULL;
	}
	else {
		return &this->items[index];
	}
}

template <class Payload>
Payload dList<Payload>::getItem(unsigned int index) {
	if (index >= this->Count) {
		throw "Index out of Bounds";
	}
	else {
		return this->items[index];
	}
}

template <class Payload>
unsigned int dList<Payload>::findItem(Payload item) {
	for (unsigned int i = 0; i < this->Count; i++) {
		if (this->items[i] == item) {
			return i;
		}
	}
	return NULL;
}

template<class Payload>
void dList<Payload>::clear()
{
	Payload* newitems = new Payload[this->stepSize]; //throws "bad_alloc" if it fails 
	delete[] this->items;	//free's old memory
	this->items = newitems;
	this->allocSize = this->stepSize;
	this->Count = 0;

}
