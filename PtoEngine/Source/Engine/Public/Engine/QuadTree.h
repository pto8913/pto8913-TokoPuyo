#pragma once

#include <vector>
#include "Component/BoxCollision.h"

class QuadTree
{
public:
	QuadTree();
	QuadTree(int inMaxObj, int inMaxLevel, int inLevel, FBox inBound, QuadTree* inParent);
	virtual ~QuadTree();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	void Add(std::shared_ptr<BoxCollision> in);
	void Remove(std::shared_ptr<BoxCollision> in);
	void Clear();

	std::vector<std::shared_ptr<BoxCollision>> Search(const FBox& area);
protected:
	void Search(const FBox& area, std::vector<std::shared_ptr<BoxCollision>>& out);
public:
	const FBox& GetBounds() const;

protected:
	int GetChildIndexForObject(const FBox& objectBound);

	void Split();

	const int thisTree = -1;
	const int childUL = 0;
	const int childUR = 1;
	const int childLL = 2;
	const int childLR = 3;

	int mMaxObjects;
	int mMaxLevels;

	QuadTree* pParent = nullptr;
	std::vector<std::shared_ptr<QuadTree>> pChildren;
	std::vector<std::shared_ptr<BoxCollision>> pBoxCollision;

	int mLevel;

	FBox mBounds;
};