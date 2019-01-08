#include <iostream>
#include "../maze/maze.h"
#include "../maze/flood_fill.h"
#include "../maze/print.h"

int main() {
	maze m(4,4);
	m.data[0] = 1;
	m.data[1] = 1;
	m.data[2] = 1;
	m.data[3] = 0;

	m.data[4] = 1;
	m.data[5] = 1;
	m.data[6] = 0;
	m.data[7] = 1;

	m.data[8] = 1;
	m.data[9] = 0;
	m.data[10] = 1;
	m.data[11] = 1;

	m.data[12] = 0;
	m.data[13] = 1;
	m.data[14] = 1;
	m.data[15] = 1;

	print(m, 1, std::cout);
	flood_fill(m, 1, 1, 3);
	print(m, 1, std::cout);
}