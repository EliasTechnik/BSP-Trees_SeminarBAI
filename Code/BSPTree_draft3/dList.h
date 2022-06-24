#pragma once

template <class Payload>
class dList {
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
	Payload* getItem(unsigned int index); //get Item at position x
	unsigned int findItem(Payload item); //returns the index if found or NULL
	unsigned int getItemCount() { return Count; }; //count of items
	void clear(); //clears the list and releases memory without destroing the instance
};

template <class Payload>
dList<Payload>::dList(unsigned int size) {
	Count = 0;
	stepSize = size;
	realloc_mem(stepSize);
}

template<class Payload>
dList<Payload>::~dList(){
	//delete[] items; //free ocupied memory
}

template <class Payload>
void dList<Payload>::realloc_mem(int newsize) {
	Payload* newitems = new Payload[newsize]; //throws "bad_alloc" if it fails 
	memcpy(newitems, items, sizeof(Payload) * allocSize); //copy old arry to new destination
	delete items;	//free's old memory
	items = newitems;
	allocSize = newsize;
}

template <class Payload>
unsigned int dList<Payload>::addItem(Payload item) {
	if (Count + 1 > allocSize) {
		realloc_mem(allocSize + stepSize);
	}
	items[Count] = item;
	Count += 1;
	return Count - 1;
}

template <class Payload>
Payload* dList<Payload>::getItem(unsigned int index) {
	if (index >= Count) {
		return NULL;
	}
	else {
		return &items[index];
	}
}

template <class Payload>
unsigned int dList<Payload>::findItem(Payload item) {
	for (unsigned int i = 0; i < Count; i++) {
		if (items[i] == item) {
			return i;
			break; //might be never reached
		}
	}
	return NULL;
}

template<class Payload>
void dList<Payload>::clear()
{
	Payload* newitems = new Payload[0]; //throws "bad_alloc" if it fails 
	delete items;	//free's old memory
	items = newitems;
	allocSize = 0;
	Count = 0;

}
