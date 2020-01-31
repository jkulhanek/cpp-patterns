#include <iostream>
#include <unordered_set>
#include <cstdlib>
#include <sstream>
#include "../binary_search_tree/avl.h"

void validateTreeContent(TreeNode * node, unordered_set<int> &values) {
	if (!node) return;
	validateTreeContent(node->left, values);
	validateTreeContent(node->right, values);

	if (values.count(node->value) > 0) {
		values.erase(node->value);
	}
	else {
		throw runtime_error("Value should not be in the tree: " + node->value);
	}
}

void validateTreeContent(const AVL & tree, const unordered_set<int> &values) {
	unordered_set<int> nvalues(values);
	validateTreeContent(tree.get_root(), nvalues);
	if (nvalues.size() > 0) {
		throw runtime_error("Values missing from the tree");
	}
}

int validateTreePropertyInternal(TreeNode * node) {
	if (!node) return 0;
	int ldepth = validateTreePropertyInternal(node->left);
	int rdepth = validateTreePropertyInternal(node->right);
	if (node->left_depth != ldepth) throw runtime_error("Left depth problem");
	if (node->right_depth != rdepth) throw runtime_error("Right depth problem");
	return max(ldepth, rdepth) + 1;
}

void validateTreeProperty(const AVL & tree) {
	validateTreePropertyInternal(tree.get_root());
}

void randomTest() {
	srand(5);
	const int totalOps = 20000;

	int sum = 0;
	AVL t1;
	unordered_set<int> values;
	stringstream code;
	for (int op = 0; op < totalOps; ++op) {
		if (rand() % 2 == 1 || values.size() == 0) {
			// Insert
			int val;
			do {
				val = rand() % 400;
			} while (find(values.begin(), values.end(), val) != values.end());

			values.emplace(val);
			t1.insert(val);
			code << "    tree.insert(" << val << ");" << endl;
			sum += val;
		}
		else {
			// delete
			int valIndex = rand() % values.size();
			auto it = values.begin();
			advance(it, valIndex);

			int val = *it;
			values.erase(it);
			t1.erase(val);
			code << "    tree.erase(" << val << ");" << endl;
			sum -= val;
		}
		validateTreeContent(t1, values);
		validateTreeProperty(t1);
		/*try {
			validateTreeContent(t1, values);
			validateTreeProperty(t1);
		}
		catch (exception e) {
			auto str = code.str();
			cout << str;
		}*/
	}
}

int main() {
	randomTest();
}