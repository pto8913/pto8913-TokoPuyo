#pragma once

#include <vector>

struct UnionFind
{
public:
	UnionFind() = default;
	UnionFind(int n);

	void clear();
	int root(int x);
	bool IsSame(int x, int y);
	bool unite(int x, int y);
	int size(int x);

	int numOfParent() const noexcept;
	void GetParent(std::vector<int>& out) const noexcept;
private:
	std::vector<int> _parent, _rank, _size;
	int N;
};