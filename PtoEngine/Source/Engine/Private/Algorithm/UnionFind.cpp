
#include "Algorithm/UnionFind.h"
#include "Algorithm/algo.h"

UnionFind::UnionFind(int n)
	: _parent(n, -1), _rank(n, 0), _size(n, 1), N(n)
{

}

void UnionFind::clear()
{
	_parent.clear();
	_rank.clear();
	_size.clear();
	_parent.assign(N, -1);
	_rank.assign(N, 0);
	_size.assign(N, 1);
}

int UnionFind::root(int x)
{
	if (_parent[x] == -1)
	{
		return x;
	}
	else
	{
		return _parent[x] = root(_parent[x]);
	}
}

bool UnionFind::IsSame(int x, int y)
{
	return root(x) == root(y);
}

bool UnionFind::unite(int x, int y)
{
	int root_x = root(x);
	int root_y = root(y);
	if (root_x == root_y)
	{
		return false;
	}
	if (_rank[root_x] < _rank[root_y])
	{
		Algo::swap(root_x, root_y);
	}
	_parent[root_y] = root_x;
	if (_rank[root_x] == _rank[root_y])
	{
		++_rank[root_x];
	}
	_size[root_x] += _size[root_y];
	return true;
}

int UnionFind::size(int x)
{
	return _size[root(x)];
}