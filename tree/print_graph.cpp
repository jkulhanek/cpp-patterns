#include <algorithm>
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

struct tree_node {
public:
	tree_node() : parent(nullptr), left(nullptr), right(nullptr) {};
	tree_node * parent;
	tree_node * left;
	tree_node * right;
	unsigned int i;
};

void destroy(tree_node * node) {
	if (!node) return;
	destroy(node->left);
	destroy(node->right);
	delete node;
}

class Solver {
public:
	Solver() {};

	vector<tree_node *> nodes;
	vector<pair<int, int>> center_positions;
	tree_node * root;
	int depth = 0;

	~Solver() {
		for (auto & node : nodes) {
			delete node;
		}
	}

	void fill_edge(tree_node * n, tree_node * nto);
	void fix_graph();
private:
	int fix_node(tree_node * n, const tree_node * parent, int depth, int offset, int & max_depth);
};

void Solver::fill_edge(tree_node * n, tree_node * nto) {
	if (!n->left) n->left = nto;
	else if (!n->right) n->right = nto;
	else if (!n->parent) n->parent = nto;
}

int Solver::fix_node(tree_node * n, const tree_node * parent, int depth, int offset, int & max_depth) {
	if (!n) return 0;
	if (n->left == parent) {
		swap(n->left, n->right);
		swap(n->right, n->parent);
	}
	else if (n->right == parent) {
		swap(n->right, n->parent);
	}

	int lsize = fix_node(n->left, n, depth + 1, offset, max_depth);
	int rsize = fix_node(n->right, n, depth + 1, offset + lsize + 1, max_depth);
	this->center_positions[n->i].first = offset + lsize;
	this->center_positions[n->i].second = depth;
	max_depth = max(depth, max_depth);
	return 1 + lsize + rsize;
}

void Solver::fix_graph() {
	if (!this->root) return;
	this->depth = 1;
	int lsize = fix_node(this->root->left, this->root, 1, 0, this->depth);
	int rsize = fix_node(this->root->right, this->root, 1, lsize + 1, this->depth);
	this->center_positions[this->root->i].first = lsize;
	this->center_positions[this->root->i].second = 0;
}

int main() {
	unsigned int n, r_g, l;
	cin >> n;
	cin >> r_g;
	cin >> l;

	Solver s;
	s.nodes.resize(n);
	s.center_positions.resize(n);

	for(unsigned int i = 0; i < n;++i) {
		tree_node * n = new tree_node();
		n->i = i;
		s.nodes[n->i] = n;

		if (n->i == r_g) {
			s.root = n;
		}
	}

	while (1 < n--) {
		unsigned int a, b;
		cin >> a;
		cin >> b;
		s.fill_edge(s.nodes[a], s.nodes[b]);
		s.fill_edge(s.nodes[b], s.nodes[a]);
	}

	s.fix_graph();

	vector<unsigned int> ls(l);
	unsigned int i = 0;
	while (0 < l--) {
		cin >> ls[i++];
	}

	for(auto & i : ls) {
		auto & p = s.center_positions[i];
		cout << p.first << " " << (s.depth - p.second) << endl;
	}

	return 0;
}