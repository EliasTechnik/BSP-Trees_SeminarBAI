#include "bsptree.h"

template <class T, class P, class L, int dimension> TreeNode< T,  P,  L, dimension>::TreeNode{
	isLeaf = true; //true because there is not any childs added jet
	maxchildcount = dimension;
};

void TreeNode::divide() {

}
