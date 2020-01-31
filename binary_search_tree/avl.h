#include <algorithm>
#include <functional>
#include <ostream>
#include <iomanip>

using namespace std;
struct TreeNode {
	TreeNode(int value) : value(value), right_depth(0), left_depth(0), left(nullptr), right(nullptr) {};
	TreeNode * left;
	TreeNode * right;
	int value;
	int right_depth;
	int left_depth;
};

class AVL {
public:
	AVL() : root(nullptr) {};
	~AVL() { this->destroyNode(this->root); }
	AVL(AVL &&other) {
		this->destroyNode(this->root);
		this->root = other.root ? this->cloneNode(other.root) : nullptr;
	};

	AVL & operator=(const AVL &other) {
		if (this != &other) {
			this->destroyNode(this->root);
			this->root = other.root ? this->cloneNode(other.root) : nullptr;
		}

		return *this;
	};
	bool insert(int value);
	bool erase(int value);
	void print(ostream &stream);
	TreeNode * get_root() const { return this->root; }
private:
	TreeNode * root;
	void destroyNode(TreeNode * &node);
	void balance(TreeNode * &node, int &depth);
	void rotate_r(TreeNode * &node, int &depth);
	void rotate_l(TreeNode * &node, int &depth);
	bool insert(TreeNode * &node, int &depth, int value);
	TreeNode * erase_down(TreeNode * &node, int & depth, bool isLeft);
	bool erase(TreeNode * &node, int &depth, int value);
	TreeNode * cloneNode(TreeNode * node) {
		if (!node) return nullptr;
		TreeNode * n = new TreeNode(node->value);
		n->left = cloneNode(node->left);
		n->left_depth = node->left_depth;
		n->right = cloneNode(node->right);
		n->right_depth = node->right_depth;
	}
};

bool AVL::insert(int value) {
	int depth;
	return insert(this->root, depth, value);
}

void AVL::balance(TreeNode * &node, int & depth) {
	if (node->left_depth - node->right_depth > 1) {
		if (node->left->left_depth < node->left->right_depth) {
			this->rotate_l(node->left, node->left_depth);
		}

		this->rotate_r(node, depth);
	}
	else if (node->right_depth - node->left_depth > 1) {
		if (node->right->right_depth < node->right->left_depth) {
			this->rotate_r(node->right, node->right_depth);
		}

		this->rotate_l(node, depth);
	}
}

void AVL::rotate_l(TreeNode * &node, int &depth) {
	TreeNode * parent = node;
	node = node->right;
	parent->right = node->left;
	parent->right_depth = node->left_depth;
	node->left = parent;
	node->left_depth = max(parent->left_depth, parent->right_depth) + 1;
	depth = max(node->left_depth, node->right_depth) + 1;
}

void AVL::rotate_r(TreeNode * &node, int &depth) {
	TreeNode * parent = node;
	node = node->left;
	parent->left = node->right;
	parent->left_depth = node->right_depth;
	node->right = parent;
	node->right_depth = max(parent->right_depth, parent->left_depth) + 1;
	depth = max(node->right_depth, node->left_depth) + 1;
}

bool AVL::insert(TreeNode * &node, int &depth, int value) {
	if (!node) {
		node = new TreeNode(value);
		depth++;
		return true;
	}

	bool wasInserted = false;
	if (node->value > value) {
		wasInserted = this->insert(node->left, node->left_depth, value);
	}
	else if (node->value < value) {
		wasInserted = this->insert(node->right, node->right_depth, value);
	}
	else {
		return false;
	}

	if (wasInserted) {
		depth = max(node->left_depth, node->right_depth) + 1;
		this->balance(node, depth);
	}
	
	return wasInserted;
}

bool AVL::erase(int value) {
	int depth;
	return this->erase(this->root, depth, value);
}

bool AVL::erase(TreeNode * &node, int &depth, int value) {
	if (!node) return false;
	if (node->value == value) {
		if (!node->left && !node->right) {
			// leaf
			delete node;
			node = nullptr;
			depth = 0;
			return true;
		}
		else if (node->left) {
			TreeNode * deleted = this->erase_down(node->left, node->left_depth, false);
			node->value = deleted->value;
			delete deleted;
			depth = max(node->left_depth, node->right_depth) + 1;
			this->balance(node, depth);
		}
		else {
			TreeNode * deleted = this->erase_down(node->right, node->right_depth, true);
			node->value = deleted->value;
			delete deleted;
			depth = max(node->left_depth, node->right_depth) + 1;
			this->balance(node, depth);
		}
	}
	else if (node->value < value) {
		bool ret = this->erase(node->right, node->right_depth, value);
		depth = max(node->left_depth, node->right_depth) + 1;
		this->balance(node, depth);
		return ret;
	}
	else {
		bool ret = this->erase(node->left, node->left_depth, value);
		depth = max(node->right_depth, node->left_depth) + 1;
		this->balance(node, depth);
		return ret;
	}
}

TreeNode * AVL::erase_down(TreeNode * &node, int & depth, bool isLeft) {
	if (isLeft) {
		if (!node->left) {
			TreeNode * deleted = node;
			node = node->right;
			depth--;
			return deleted;
		}
		else {
			TreeNode * deleted = this->erase_down(node->left, node->left_depth, isLeft);
			if (deleted) {
				depth = max(node->right_depth, node->left_depth) + 1;
				this->balance(node, depth);
			}
			return deleted;
		}
	}
	else {
		if (!node->right) {
			TreeNode * deleted = node;
			node = node->left;
			depth--;
			return deleted;
		}
		else {
			TreeNode * deleted = this->erase_down(node->right, node->right_depth, isLeft);
			if (deleted) {
				depth = max(node->right_depth, node->left_depth) + 1;
				this->balance(node, depth);
			}
			return deleted;
		}
	}
}

void AVL::destroyNode(TreeNode * &node) {
	if (node) {
		this->destroyNode(node->left);
		this->destroyNode(node->right);
		node = nullptr;
	}
}




// Print
void replicate(function<void()> f, int times) {
	if (times == 0) {
		return;
	}

	f();
	replicate(f, times - 1);
}

void AVL::print(ostream &stream) {
	auto printCell = [&stream](TreeNode * node) {
		stream << "[" << setw(2) << node->value << "]";
	};

	auto padCell = [&stream](bool isLink) {
		if (isLink)
			stream << "____";
		else
			stream << "    ";
	};

	function<int(TreeNode * node)> calculateWidth = [&calculateWidth](TreeNode * node) -> int {
		if (!node) {
			return 0;
		}

		int lchild, rchild;
		lchild = calculateWidth(node->left);
		rchild = calculateWidth(node->right);
		return 1 + lchild + rchild;
	};

	function<int(TreeNode * node)> calculateDepth = [&calculateDepth](TreeNode * node) ->int {
		if (!node) return 0;
		return 1 + max(calculateDepth(node->left), calculateDepth(node->right));
	};

	function<void(TreeNode * root, int depth, int layer)> printLayer = [&stream, &printLayer, &calculateWidth, &padCell, &printCell](TreeNode * root, int depth, int layer) {
		if (!root) {
			return;
		}

		if (layer > depth) {
			printLayer(root->left, depth + 1, layer);
			padCell(false);
			printLayer(root->right, depth + 1, layer);
		}
		else if (layer == depth) {
			if (root->left) {
				if (root->left->left) {
					replicate([&stream, &padCell]() { padCell(false); }, calculateWidth(root->left->left));
				}

				padCell(false);
				if (root->left->right) {
					replicate([&stream, &padCell]() { padCell(true); }, calculateWidth(root->left->right));
				}
			}

			printCell(root);

			if (root->right) {
				if (root->right->left) {
					replicate([&stream, &padCell]() { padCell(true); }, calculateWidth(root->right->left));
				}

				padCell(false);
				if (root->right->right) {
					replicate([&stream, &padCell]() { padCell(false); }, calculateWidth(root->right->right));
				}
			}
		}
	};

	for (int i = 0; i < calculateDepth(this->root); ++i) {
		printLayer(this->root, 0, i);
		stream << endl;
	}
};