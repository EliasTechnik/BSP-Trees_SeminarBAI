#pragma once

template <class Payload>
class dList {
private:
	Payload* items; //pointer to the first item in memory
	int Count;	   //actual used space	
	int allocSize; //allocated memory space //might be useless to store
	void realloc_mem(int newsize);
	int stepSize; //size with which the list is expanded
public:
	dList(int size = 256); //constructor
	~dList();//TODO: Destructor
	int addItem(Payload item); //adds one Item to the list
	Payload* getItem(int index); //get Item at position x
	int getItemCount() { return Count; }; //count of items
};

template <class Payload>
dList<Payload>::dList(int size) {
	Count = 0;
	stepSize = size;
	realloc_mem(stepSize);
}

template<class Payload>
dList<Payload>::~dList(){
	delete[] items; //free ocupied memory
}

template <class Payload>
void dList<Payload>::realloc_mem(int newsize) {
	Payload* newitems = new Payload[newsize]; //throws "bad_alloc" if it fails 
	memcpy(newitems, items, sizeof(Payload) * allocSize); //copy old arry to new destination
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
Payload* dList<Payload>::getItem(int index) {
	if (index >= Count) {
		return NULL;
	}
	else {
		return &items[index];
	}
}