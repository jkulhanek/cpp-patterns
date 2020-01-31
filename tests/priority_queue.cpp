#include <iostream>
#include <queue>

using namespace std;

class PriorityQueue {

public:
	PriorityQueue() {};
	PriorityQueue(vector<int> && data) {
		this->data = data;
		for (int i = data.size() / 2; i >= 0; --i) {
			repair_top(i);
		}
	}

	void push(int value);
	int pop();
	bool empty() { return data.empty(); }
private:
	std::vector<int> data;
	void repair_top(int i);
};

void PriorityQueue::push(int value) {
	int index = data.size();
	data.push_back(value);
	while (index > 0) {
		int oldIndex = index;
		index >>= 1;
		if (data[oldIndex] < data[index]) {
			swap(data[oldIndex], data[index]);
		}
		else {
			break;
		}		
	}
}

int PriorityQueue::pop() {
	int ret = data[0];
	data[0] = data[data.size() - 1];
	data.pop_back();
	this->repair_top(0);
	return ret;
}

void PriorityQueue::repair_top(int index) {
	int size = data.size();
	while (index < size) {
		int oldIndex = index;
		if (index * 2 + 1 < size && data[index] > data[index * 2 + 1] && (index * 2 + 2 >= size || data[index * 2 + 1] < data[index * 2 + 2])) index = index * 2 + 1;
		else if (index * 2 + 2 < size && data[index] > data[index * 2 + 2]) index = index * 2 + 2;

		if (oldIndex == index) {
			break;
		}
		else {
			swap(data[oldIndex], data[index]);
		}
	}
}

void main() {
	vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(8);
	v.push_back(6);
	v.push_back(3);
	v.push_back(7);
	v.push_back(4);
	v.push_back(5);
	PriorityQueue q(move(v));
	// std::make_heap(begin(numbers), end(numbers));

	while (!q.empty()) {
		std::cout << q.pop() << std::endl;
	}
}