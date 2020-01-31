#include <algorithm>
#include <stack>
#include "maze.h"

void flood_fill_recursive_internal(maze & m, int x, int y, int original_color, int color) {
	if (x < 0 || x >= m.width || y < 0 || y >= m.height) return;
	if (m.data[y * m.width + x] == color) return; // Already filled
	if (m.data[y * m.width + x] == original_color) m.data[y * m.width + x] = color;
	else {
		return;
	}

	flood_fill_recursive_internal(m, x + 1, y, original_color, color);
	flood_fill_recursive_internal(m, x, y + 1, original_color, color);
	flood_fill_recursive_internal(m, x - 1, y, original_color, color);
	flood_fill_recursive_internal(m, x, y - 1, original_color, color);
}

void flood_fill_recursive(maze & m, int x, int y, int color) {
	flood_fill_recursive_internal(m, x, y, m.data[y * m.width + x], color);
}


struct position {
	position(int x, int y) : x(x), y(y) {};
	int x;
	int y;
};

void flood_fill(maze & m, int x, int y, int color) {
	const int old_color = m.data[y * m.width + x];
	std::stack<position> positions;
	while(!positions.empty()) {
		const int x = positions.top().x;
		const int y = positions.top().y;
		positions.pop();

		if (x < 0 || x >= m.width || y < 0 || y >= m.height) continue;
		if (m.data[y * m.width + x] == color) continue; // Already filled
		if (m.data[y * m.width + x] == old_color) m.data[y * m.width + x] = color;
		else {
			continue;
		}

		positions.emplace(position(x + 1, y));
		positions.emplace(position(x, y + 1));
		positions.emplace(position(x - 1, y));
		positions.emplace(position(x, y - 1));
	}
}