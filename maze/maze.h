#ifndef maze_h
#define maze_h
#include <vector>

template <class T>
class maze {
public:
	maze() {};
	maze(const int width, const int height) : width(width), height(height), data(width * height) {};
	std::vector<T> data;
	int width;
	int height;
};
#endif // !maze_h