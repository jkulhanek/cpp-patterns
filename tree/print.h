#include "tree_node.h"

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