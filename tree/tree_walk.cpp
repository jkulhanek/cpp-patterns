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

void preorder_recursive(node * n, function<void(const node *)> action) {
	if (!n) return;

	action(n);
	preorder_recursive(n->left, action);
	preorder_recursive(n->right, action);
}

void postorder_recursive(node * n, function<void(const node *)> action) {
	if (!n) return;
	
	postorder_recursive(n->left, action);
	postorder_recursive(n->right, action);
	action(n);
}

void inorder_recursive(node * n, function<void(const node *)> action) {
	if (!n) return;

	inorder_recursive(n->left, action);
	action(n);
	inorder_recursive(n->right, action);
}

void preorder(node * n, function<void(const node *)> action) {
	if (!n) return;
	
	stack<node *> path;
	node * current = n;

	while (current || !path.empty()) {
		while (current) {
			action(current);
			path.push(current);
			current = current->left;
		}

		if (!path.empty()) {
			node * &top = path.top();
			current = top->right;
			path.pop();
		}
	}
}

void postorder(node * n, function<void(const node *)> action) {
	if (!n) return;

	stack<node *> path;
	node * current = n;

	while (current || !path.empty()) {
		while (current) {
			if (current->right) path.push(current->right);
			path.push(current);
			current = current->left;
		}

		current = path.top();
		path.pop();
		if (current->right && !path.empty() && path.top() == current->right) {
			path.pop();
			path.push(current);
			current = current->right;
		}
		else {
			action(current);
			current = nullptr;
		}
	}
}

void inorder(node * n, function<void(const node *)> action) {
	if (!n) return;
	stack<node *> path;
	
	node * current = n;
	while (current || !path.empty()) {
		while (current) {
			path.push(current);
			current = current->left;
		}

		if (!path.empty()) {
			node * &top = path.top();
			action(top);
			current = top->right;
			path.pop();
		}
	}
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

	const node * current() const { return _current; }
private:
	node * _current;
	node * _current_pointer;
	bool is_started = false;
	stack<node *> path;
};

void destroy(node * n) {
	if (!n) return;
	destroy(n->left);
	destroy(n->right);
	delete n;
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
	auto printNode = [](const node * n) {
		cout << n->value << endl;
	};

	inorder_recursive(tree, printNode);

	InorderTraverser traverser(tree);
	while (traverser.step()) {
		printNode(traverser.current());
	}

	destroy(tree);

	return 0;
}