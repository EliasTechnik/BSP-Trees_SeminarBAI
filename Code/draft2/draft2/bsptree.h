#pragma once

struct 2d{
	float x;
	float y;
};

template <class T,class P, class L, int dimension> 
class TreeNode {
protected:
	T childs[dimension]; 
	int maxchildcount
	L location;
	bool isLeaf;
	P* payload;
public:
	//TreeNode();
	L getLocation() { return location; };
	P* getPayload() { return payload; };
	bool isLeaf() { return isLeaf; }
	void divide();


};