#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>
#include <functional>
#include <iomanip>

using namespace std;

struct position {
	int x;
	int y;
	int moves;
};

template <class T>
class Maze {
public:
	Maze(unsigned int width, unsigned int height) : data(width * height), width(width), height(height) {};
	unsigned int width;
	unsigned int height;
	vector<T> data;
	T & operator()(unsigned int x, unsigned int y) {
		return data[y * width + x];
	}

	T & operator()(const position & pos) {
		return data[pos.y * width + pos.x];
	}
};

class Solver {
public:
	Solver(int width, int height, int estart, int dmov, int denv) : maze(width, height), closed(width, height), estart(estart), dmov(dmov), denv(denv), width(width), height(height) {
		fill(closed.data.begin(), closed.data.end(), 0);
	};

	Maze<unsigned int> maze;
	Maze<int> closed;
	int solve();
private:
	queue<pair<position, int>> open;
	
	int estart;
	int dmov;
	int denv;
	int width;
	int height;

	bool is_final(const position & pos) const { return pos.x == width - 1 && pos.y == height - 1; };

	bool is_valid(const position & pos) const {
		return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
	}

	pair<position, int> execute(const position & from, position to, int olde) {
		int e = olde;
		if (maze(from) < maze(to)) e += dmov;
		else if (maze(from) > maze(to)) e = max(0, e - dmov);

		// second set of rules
		if (e >= denv + maze(to)) e -= denv;
		else if (e > maze(to)) e = maze(to);

		to.moves = from.moves + 1;
		return make_pair(to, e);
	}

	void try_expand(const position & old, position pos, int energy) {
		if (energy <= 0) return;

		if (is_valid(pos)) {
			auto r = execute(old, pos, energy);
			if (closed(r.first) < r.second) {
				open.push(r);
				closed(pos) = r.second;
			}
		}
	}

	void try_expand(const position & old, int energy) {
		try_expand(old, position{ old.x + 1, old.y }, energy);
		try_expand(old, position{ old.x, old.y + 1 }, energy);
		try_expand(old, position{ old.x - 1, old.y }, energy);
		try_expand(old, position{ old.x, old.y - 1 }, energy);
	}
};

int Solver::solve() {
	open.push(make_pair(position{ 0,0 }, estart));
	while (!open.empty()) {
		auto top = open.front(); open.pop();

		if (is_final(top.first)) 
			return top.first.moves;

		// expand
		try_expand(top.first, top.second);
	}

	return -1;
}

int main()
{
	int width, height;
	int estart;
	int dmov;
	int denv;

	cin >> height;
	cin >> width;
	cin >> estart;
	cin >> dmov;
	cin >> denv;
	Solver s(width, height, estart, dmov, denv);
	for (int i = 0; i < width * height; ++i) {
		cin >> s.maze.data[i];
	}

	cout << s.solve() << endl;
	return 0;
}