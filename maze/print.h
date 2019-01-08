#include <ostream>
#include <iomanip>
#include "maze.h"

void print(const maze & m, const int space, std::ostream & cout) {
	for (int i = 0; i < m.height; ++i) {
		for (int j = 0; j < m.width - 1; ++j) {
			cout << std::setw(space) << m.data[i * m.width + j] << " ";
		}
		if (m.width > 0) {
			cout << std::setw(space) << m.data[(i + 1) * m.width - 1];
		}

		cout << std::endl;
	}
}