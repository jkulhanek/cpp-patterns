#include <algorithm>
#include <vector>
#include <iostream>
#include <functional>
#include <stack>
#include <queue>
#include <bitset>

#include "../maze/maze.h"
#include "../maze/print.h"

using namespace std;

struct state {
public:
	int x;
	int y;
	int cost;
};

class Maze : public maze<int> {
public:
	Maze(int width, int height) : maze(width, height), closed(width * height) {
		fill(closed.begin(), closed.end(), false);
	};

	state initial_state;
	state final_state;

	vector<bool> closed;

	void expand(const state & s, function<void(const state & s)> callback) const {
		if (s.x > 0) callback(state{ s.x - 1, s.y, s.cost + 1 });
		if (s.y > 0) callback(state{ s.x, s.y - 1, s.cost + 1 });
		if (s.x < width - 1) callback(state{ s.x + 1, s.y, s.cost + 1 });
		if (s.y < height - 1) callback(state{ s.x, s.y + 1, s.cost + 1 });
	}

	bool is_closed(const state & c) {
		return closed[width * c.y + c.x];
	}

	void close_state(const state & c) {
		closed[width * c.y + c.x] = true;
	}

	bool is_final(const state & s) const {
		return s.x == this->final_state.x && s.y == this->final_state.y;
	}
};


Maze createMaze() {
	Maze m(10, 5);
	m.initial_state = state{ 0,0,0 };
	m.final_state = state{ 9,4,0 };
	fill(m.data.begin(), m.data.end(), 0);
	m.data[m.width * 0 + 5] = 1;
	m.data[m.width * 1 + 5] = 1;
	m.data[m.width * 2 + 5] = 1;
	m.data[m.width * 3 + 5] = 1;
	m.data[m.width * 4 + 7] = 1;
	m.data[m.width * 1 + 7] = 1;
	m.data[m.width * 2 + 7] = 1;
	m.data[m.width * 3 + 7] = 1;
	return m;
}

bool dfs(Maze m, state & final_state) {
	stack<state> open;
	open.push(m.initial_state);

	while (!open.empty()) {
		auto s = open.top();
		open.pop();

		bool isClosed = m.is_closed(s);
		if (isClosed) continue;
		m.close_state(s);

		if (m.is_final(s)) {
			final_state = s;
			return true;
		}

		m.expand(s, [&open, &m](const state &s) {
			bool isClosed = m.is_closed(s);
			if (!isClosed)
				open.push(s);
		});
	}
}

bool bfs(Maze & m, state & final_state) {
	queue<state> open;
	open.push(m.initial_state);

	while (!open.empty()) {
		auto s = open.front();
		open.pop();
		
		bool isClosed = m.is_closed(s);
		if (isClosed) continue;
		m.close_state(s);

		if (m.is_final(s)) {
			final_state = s;
			return true;
		}

		m.expand(s, [&open, &m](const state &s) {
			bool isClosed = m.is_closed(s);
			if (!isClosed)
				open.push(s);
		});
	}
}

class StateCompare {
public:
	bool operator()(const state &i1, const state &i2) {
		return i1.cost > i2.cost;
	};
};

bool uniform_cost_search(Maze & m, state & final_state) {
	priority_queue<state, vector<state>, StateCompare> open;
	open.push(m.initial_state);

	while (!open.empty()) {
		auto s = open.top();
		open.pop();

		bool isClosed = m.is_closed(s);
		if (isClosed) continue;
		m.close_state(s);

		if (m.is_final(s)) {
			final_state = s;
			return true;
		}

		m.expand(s, [&open, &m](const state &s) {
			bool isClosed = m.is_closed(s);
			if (!isClosed)
				open.push(s);
		});

		
	}
};

int main() {
	Maze m = createMaze();
	print(m, 1, cout);

	state final;
	if (dfs(m, final)) {
		cout << "len: " << final.cost << endl;
	}

	char c;
	cin >> c;
}