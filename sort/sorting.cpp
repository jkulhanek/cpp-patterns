#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <functional>
#include <array>
using namespace std;


template <class T>
void merge(vector<T> & a, vector<T> & aux, int lo, int mid, int hi, const function<bool(const T &, const T &)> & comp)
{
	int i = lo, j = mid + 1;
	for (int k = lo; k <= hi; k++)
		if (i > mid) aux[k] = a[j++];
		else if (j > hi) aux[k] = a[i++];
		else if (comp(a[j], a[i])) aux[k] = a[j++];
		else aux[k] = a[i++];
}

template <class string>
void radixsort(vector<string> & a, int W) {
	int N = a.size();
	const int R = 256;
	vector<string> aux(a.size());
	array<unsigned int, R> count;

	bool isAux = false;
	for (int d = W - 1; d >= 0; d--) {
		vector<string> & from = isAux ? aux : a;
		vector<string> & to = isAux ? a : aux;
		count.fill(0);
		for (int i = 0; i < N; i++)
			count[from[i].at(d) + 1]++;

		for (int r = 0; r < R - 1; r++)
			count[r + 1] += count[r];
		for (int i = 0; i < N; i++)
			to[count[from[i].at(d)]++] = from[i];

		isAux = !isAux;
	}

	if (isAux) {
		swap(a, aux);
	}
}

template <class T>
void insertsort(vector<T> & a, const function<bool(const T &, const T &)> &  comp) {
	for (int i = 1; i < a.size(); i++)
		for (int j = i; j > 0 && comp(a[j], a[j - 1]); j--)
			swap(a[j], a[j - 1]);
}

template <class T>
int partition(vector<T> & a, int lo, int hi, const function<bool(const T &, const T &)> &  comp) {
	int i = lo, j = hi + 1;
	T v = a[lo];
	while (true) {
		while (comp(a[++i], v)) if (i == hi) break;
		while (comp(v, a[--j])) if (j == lo) break;
		if (i >= j) break;
		swap(a[i], a[j]);
	}

	swap(a[lo], a[j]);
	return j;
}

template <class T>
void quicksort(vector<T> & a, int lo, int hi, const function<bool(const T &, const T &)> &  comp)
{
	if (hi <= lo) return;
	int j = partition(a, lo, hi, comp);
	quicksort(a, lo, j - 1, comp);
	quicksort(a, j + 1, hi, comp);
}

template <class T>
void quicksort(vector<T> & a, const function<bool(const T &, const T &)> & comp) {
	quicksort(a, 0, a.size() - 1, comp);
}

template <class T>
void mergesort(vector<T> & a, const function<bool(const T &, const T &)> &  comp) {
	vector<T> helper(a.size());
	int n = a.size();
	bool isHelper = false;
	for (int bSize = 1; bSize < n; bSize *= 2) {
		for (int lo = 0; lo < n; lo += 2 * bSize) {
			auto & from = isHelper ? helper : a;
			auto & to = isHelper ? a : helper;
			merge(from, to, lo, lo + bSize - 1, min(lo + bSize * 2, n) - 1, comp);
		}

		isHelper = !isHelper;
	}

	if (isHelper) {
		swap(a, helper);
	}
}

int main() {
	vector<string> data;
	data.push_back("aaa");
	data.push_back("abc");
	data.push_back("sed");
	data.push_back("can");
	data.push_back("acd");
	data.push_back("deb");
	data.push_back("ael");
	data.push_back("bds");

	vector<string> unsorted(data);
	sort(unsorted.begin(), unsorted.end());
	for_each(unsorted.begin(), unsorted.end(), [](string & c) {
		cout << c << ", ";
	});

	cout << endl;

	auto & comp = [](const string & a, const string & b) -> bool {
		return a < b;
	};


	copy(data.begin(), data.end(), unsorted.begin());
	cout << "Testing mergesort" << endl;
	mergesort<string>(unsorted, comp);
	for_each(unsorted.begin(), unsorted.end(), [](string & c) {
		cout << c << ", ";
	});
	cout << endl;

	copy(data.begin(), data.end(), unsorted.begin());
	cout << "Testing radix sort lsd" << endl;
	radixsort<string>(unsorted, 3);
	for_each(unsorted.begin(), unsorted.end(), [](string & c) {
		cout << c << ", ";
	});
	cout << endl;


	copy(data.begin(), data.end(), unsorted.begin());
	cout << "Testing quick sort" << endl;
	quicksort<string>(unsorted, comp);
	for_each(unsorted.begin(), unsorted.end(), [](string & c) {
		cout << c << ", ";
	});
	cout << endl;

	copy(data.begin(), data.end(), unsorted.begin());
	cout << "Testing insert sort" << endl;
	insertsort<string>(unsorted, comp);
	for_each(unsorted.begin(), unsorted.end(), [](string & c) {
		cout << c << ", ";
	});
	cout << endl;

	return 0;
}