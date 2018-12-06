#include <queue>
#include <cassert>
#include <algorithm>

using namespace std;

class TreeNode {
public:
	TreeNode(int value) : left(nullptr), right(nullptr), parent(nullptr), left_depth(0), right_depth(0), value(value) {};
	TreeNode * left;
	TreeNode * right;
	int value;
	unsigned int left_depth;
	unsigned int right_depth;

	TreeNode * parent;

	

private:

};

class AVL {
public:
	AVL() : root(nullptr) {};
	AVL(AVL &&other) {
		this->destroy(this->root);
		this->root = other.root;
		other.root = nullptr;
	}

	~AVL() {
		this->destroy(this->root);
	}
	// TODO: move operator

	bool insert(int value);
	bool remove(int value);
private:
	TreeNode * root;

	void rotate_left(TreeNode * pivot);
	void rotate_right(TreeNode * pivot);
	void rotate_left_right(TreeNode * &pivot);
	void rotate_right_left(TreeNode * &pivot);

	void destroy(TreeNode * node) {
		if (!node) return;
		queue<TreeNode *> nodes;
		nodes.push(node);

		while (!nodes.empty()) {
			TreeNode * n = nodes.front();
			nodes.pop();

			if (n->left) nodes.push(n->left);
			if (n->right) nodes.push(n->right);
			delete n;
		}
	}
};

bool AVL::insert(int value) {
	TreeNode * n = this->root;
	TreeNode * parent = nullptr;
	while (n != nullptr) {
		parent = n;
		if (value == n->value)
			return false;
		if (value > n->value) {
			n = n->right;
		}
		else {
			n = n->left;
		}
	}

	if (!parent) {
		this->root = new TreeNode(value);
		return true;
	}

	TreeNode * current = new TreeNode(value);
	if (value > parent->value) {
		parent->right = current;
		parent->right->parent = parent;
	}
	else {
		parent->left = current;
		parent->left->parent = parent;
	}

	
	for (TreeNode * backup = parent; backup != nullptr; current = backup, backup = backup->parent) {
		if (current == backup->right) {
			unsigned int original_depth = max(backup->left_depth, backup->right_depth);
			backup->right_depth++;
			if (backup->left_depth < backup->right_depth - 1) {
				if (backup->right_depth > backup->left_depth) {
					this->rotate_left(backup);
				}
				else {
					this->rotate_right_left(backup);
				}
			}

			if (max(backup->left_depth, backup->right_depth) == original_depth) {
				break;
			}
		}
		else {
			unsigned int original_depth = max(backup->left_depth, backup->right_depth);
			backup->left_depth++;
			if (backup->right_depth < backup->left_depth - 1) {
				if (backup->left_depth > backup->right_depth) {
					this->rotate_right(backup);
				}
				else {
					this->rotate_left_right(backup);
				}
			}

			if (max(backup->right_depth, backup->left_depth) == original_depth) {
				break;
			}
		}
	}
}

void AVL::rotate_right(TreeNode * pivot) {
	assert(pivot->parent != nullptr);

	TreeNode * parent = pivot->parent;
	if (parent == this->root) {
		this->root = pivot;
	}
	
	pivot->parent = parent->parent;

	parent->left = pivot->right;
	pivot->right->parent = parent->left;
	parent->left_depth = pivot->right_depth;
	
	pivot->right = parent;
	parent->parent = pivot;
	pivot->right_depth = max(parent->left_depth, parent->right_depth) + 1;
}

void AVL::rotate_left(TreeNode * pivot) {
	assert(pivot->parent != nullptr);

	TreeNode * parent = pivot->parent;
	if (parent == this->root) {
		this->root = pivot;
	}

	pivot->parent = parent->parent;

	parent->right = pivot->left;
	if(pivot->left) pivot->left->parent = parent->right;
	parent->right_depth = pivot->left_depth;

	pivot->left = parent;
	parent->parent = pivot;
	pivot->left_depth = max(parent->right_depth, parent->left_depth) + 1;
}

void AVL::rotate_left_right(TreeNode * &pivot) {
	assert(pivot->parent != nullptr);
	assert(pivot->right != nullptr);

	TreeNode * parent = pivot->parent;
	TreeNode * x = pivot->right;
	if (parent == this->root) {
		this->root = x;
	}

	x->parent = parent->parent;

	pivot->right = x->left;
	pivot->right_depth = x->left_depth;
	if (pivot->right) pivot->right->parent = pivot;

	parent->left = x->right;
	parent->left_depth = x->right_depth;
	if (parent->left) parent->left->parent = parent;

	parent->parent = x;
	pivot->parent = x;
	x->left = pivot;
	x->right = parent;
	x->right_depth = 1 + max(parent->left_depth, parent->right_depth);
	x->left_depth = 1 + max(pivot->right_depth, pivot->left_depth);
	pivot = x;
}

void AVL::rotate_right_left(TreeNode * &pivot) {
	assert(pivot->parent != nullptr);
	assert(pivot->left != nullptr);

	TreeNode * parent = pivot->parent;
	TreeNode * x = pivot->left;
	if (parent == this->root) {
		this->root = x;
	}

	x->parent = parent->parent;

	pivot->left = x->right;
	pivot->left_depth = x->right_depth;
	if (pivot->left) pivot->left->parent = pivot;

	parent->right = x->left;
	parent->right_depth = x->left_depth;
	if (parent->right) parent->right->parent = parent;

	parent->parent = x;
	pivot->parent = x;
	x->right = pivot;
	x->left = parent;
	x->left_depth = 1 + max(parent->right_depth, parent->left_depth);
	x->right_depth = 1 + max(pivot->left_depth, pivot->right_depth);
	pivot = x;
}