#ifndef maze_h
#define maze_h
#include <vector>
class maze {
public:
	maze(const int width, const int height) : width(width), height(height), data(width * height) {};
	std::vector<int> data;
	int width;
	int height;
};
#endif // !maze_h