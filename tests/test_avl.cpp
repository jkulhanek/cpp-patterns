//Link to Boost
#define BOOST_TEST_DYN_LINK

//Define our Module name (prints at testing)
#define BOOST_TEST_MODULE "AVL"

//VERY IMPORTANT - include this last
#include <boost/test/unit_test.hpp>

#include "../binary_search_tree/avl.hpp"


bool validateTreeInternal(TreeNode * node, int * depth, int * maxNode, int * minNode) {
	if (!node) {
		*depth = 0;
		return true;
	}

	int ldepth, rdepth;
	int lmaxNode, rminNode;
	lmaxNode = INT32_MIN;
	rminNode = INT32_MAX;
	validateTreeInternal(node->left, &ldepth, &lmaxNode, minNode);	
	validateTreeInternal(node->right, &rdepth, maxNode, &rminNode);

	if (node->right_depth != rdepth)
		BOOST_FAIL("Right depth is not correct");
	if (node->left_depth != ldepth)
		BOOST_FAIL("Left depth is not correct");

	int balanceFactor = (int)node->right_depth - (int)node->left_depth;
	if (balanceFactor > 1 || balanceFactor < -1) 
		BOOST_FAIL("Tree is unbalanced");
	if (lmaxNode > node->value) 
		BOOST_FAIL("BST corrupted");
	if (rminNode < node->value) 
		BOOST_FAIL("BST corrupted");

	if (node->value < *minNode) {
		*minNode = node->value;
	}

	if (node->value > *maxNode) {
		*maxNode = node->value;
	}

	*depth = 1 + max(ldepth, rdepth);
	return true;
}

void validateTree(TreeNode * node) {
	if (!node) return;
	int depth, max, min;
	max = INT32_MIN;
	min = INT32_MAX;
	validateTreeInternal(node, &depth, &max, &min);
}


void validateTree(const AVL &tree) {
	BOOST_FAIL("Test failed");
}

//Name your test cases for what they test
BOOST_AUTO_TEST_CASE(insert_no_rebalance)
{
	AVL obj;
	
	obj.insert(2);
	obj.insert(1);
	obj.insert(3);

	validateTree(obj.get_root());
}

BOOST_AUTO_TEST_CASE(insert_l_rotate)
{
	AVL obj;
	
	obj.insert(1);
	obj.insert(2);
	obj.insert(3);

	validateTree(obj.get_root());
}

BOOST_AUTO_TEST_CASE(insert_r_rotate)
{
	AVL obj;
	
	obj.insert(3);
	obj.insert(2);
	obj.insert(1);

	validateTree(obj.get_root());
}