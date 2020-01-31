#include <algorithm>
#include <functional>
#include <iostream>
#include <stack>

using namespace std;

struct node {
public:
	node(int value) :value(value), right(nullptr), left(nullptr) {};
	node(int value, node * left, node * right) :value(value), right(right), left(left) {};
	int value;
	node * left;
	node * right;
};

void destroy(node * n) {
	if (!n) return;
	destroy(n->left);
	destroy(n->right);
	delete n;
}

class InorderTraverser {
public:
	InorderTraverser(node * start) : _current_pointer(start) {};

	bool step() {
		while (_current_pointer || !path.empty()) {
			while (_current_pointer) {
				path.push(_current_pointer);
				_current_pointer = _current_pointer->left;
			}

			if (!path.empty()) {
				node * &top = path.top();
				_current = top;
				_current_pointer = top->right;
				path.pop();
				return true;
			}
		}

		return false;
	}

	bool can_walk() const {
		return _current_pointer || !path.empty();
	}

	node * current() { return _current; }
private:
	node * _current;
	node * _current_pointer;
	bool is_started = false;
	stack<node *> path;
};

bool find_exact_difference(node * n, pair<node *, node *> & result, int difference) {
	if (!n) return false;
	InorderTraverser trav1(n);
	InorderTraverser trav2(n);
	trav1.step();
	trav2.step();

	while (trav1.can_walk() || trav2.can_walk()) {
		if (!trav1.can_walk()) trav2.step();
		else if (!trav2.can_walk()) trav2.step();
		else {
			int travDifference = trav1.current()->value - trav2.current()->value;
			if (travDifference < difference) {
				trav1.step();
			}
			else {
				trav2.step();
			}
		}

		int travDifference = trav1.current()->value - trav2.current()->value;
		if (travDifference == difference) {
			result.first = trav2.current();
			result.second = trav1.current();
			return true;
		}
	}
}

node * createTree() {
	return new node(5,
		new node(3,
			new node(1),
			new node(4)
		),
		new node(8,
			new node(6, nullptr, new node(7)),
			new node(10)
		));
}

int main() {
	node * tree = createTree();

	pair<node *, node *> result;
	find_exact_difference(tree, result, 6);
	cout << result.first->value << " " << result.second->value;
	destroy(tree);

	return 0;
}