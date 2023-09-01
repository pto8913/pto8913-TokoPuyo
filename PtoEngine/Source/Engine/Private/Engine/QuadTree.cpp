
#include "Engine/QuadTree.h"

#include "EngineSettings.h"

#include "Component/BoxCollision.h"
#include "Helper/BoxHelper.h"

QuadTree::QuadTree(int inMaxObj, int inMaxLevel, int inLevel, FBox inBound, QuadTree* inParent)
	:
	mMaxObjects(inMaxObj),
	mMaxLevels(inMaxLevel),
	mLevel(inLevel),
	mBounds(inBound),
	pParent(inParent)
{
	pChildren.assign(4, nullptr);
}
QuadTree::QuadTree()
	: QuadTree(
		5,
		5,
		0,
		FBox(FVector(0.f, 0.f, 0.f), FVector(EngineSettings::GetWindowSize().x, 0.f, EngineSettings::GetWindowSize().y)),
		nullptr
	)
{
}

QuadTree::~QuadTree()
{
	pParent = nullptr;
	for (auto&& child : pChildren)
	{
		if (child != nullptr)
		{
			child.reset();
			child = nullptr;
		}
	}
	pChildren.clear();
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void QuadTree::Add(std::shared_ptr<BoxCollision> in)
{
	if (pChildren[0] != nullptr)
	{
		int idxToPlaceObject = GetChildIndexForObject(in->GetBoundingBox());
		if (idxToPlaceObject != thisTree)
		{
			pChildren[idxToPlaceObject]->Add(in);
			return;
		}
	}

	pBoxCollision.emplace_back(in);

	if (pBoxCollision.size() > mMaxObjects && mLevel < mMaxLevels && pChildren[0] == nullptr)
	{
		Split();

		auto iter = pBoxCollision.begin();
		while (iter != pBoxCollision.end())
		{
			auto obj = *iter;
			int idxToPlaceObject = GetChildIndexForObject(in->GetBoundingBox());
			if (idxToPlaceObject != thisTree)
			{
				pChildren[idxToPlaceObject]->Add(in);
				iter = pBoxCollision.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}
}
void QuadTree::Remove(std::shared_ptr<BoxCollision> in)
{
	int idxToPlaceObject = GetChildIndexForObject(in->GetBoundingBox());

	if (idxToPlaceObject == thisTree || pChildren[idxToPlaceObject] == nullptr)
	{
		for (int i = 0; i < pBoxCollision.size(); ++i)
		{
			if (pBoxCollision[i]->GetOwner() == in->GetOwner())
			{
				pBoxCollision.erase(pBoxCollision.begin() + i);
				break;
			}
		}
	}
	else
	{
		return pChildren[idxToPlaceObject]->Remove(in);
	}
}
void QuadTree::Clear()
{
	for (auto&& elem : pBoxCollision)
	{
		if (elem != nullptr)
		{
			elem.reset();
			elem = nullptr;
		}
	}

	for (int i = 0; i < 4; ++i)
	{
		if (pChildren[i] != nullptr)
		{
			pChildren[i]->Clear();
			pChildren[i].reset();
			pChildren[i] = nullptr;
		}
	}
}

std::vector<std::shared_ptr<BoxCollision>> QuadTree::Search(const FBox& area)
{
	std::vector<std::shared_ptr<BoxCollision>> OverlapCandidates;
	Search(area, OverlapCandidates);

	std::vector<std::shared_ptr<BoxCollision>> out;
	for (auto&& collision : OverlapCandidates)
	{
		if (collision != nullptr)
		{
			if (Box::IsInBox(area, collision->GetBoundingBox()))
			{
				out.emplace_back(collision);
			}
		}
	}
	return out;
}
void QuadTree::Search(const FBox& area, std::vector<std::shared_ptr<BoxCollision>>& out)
{
	out.insert(out.end(), pBoxCollision.begin(), pBoxCollision.end());

	if (pChildren[0] != nullptr)
	{
		int idxToPlaceObject = GetChildIndexForObject(area);

		if (idxToPlaceObject == thisTree)
		{
			for (int i = 0; i < 4; ++i)
			{
				if (Box::IsInBox(pChildren[i]->GetBounds(), area))
				{
					pChildren[i]->Search(area, out);
				}
			}
		}
		else
		{
			pChildren[idxToPlaceObject]->Search(area, out);
		}
	}
}

const FBox& QuadTree::GetBounds() const
{
	return mBounds;
}

int QuadTree::GetChildIndexForObject(const FBox& objectBound)
{
	int idx = -1;

	float vertical = mBounds.leftTop.x + mBounds.GetWidth() / 2.f;
	float horizontal = mBounds.leftTop.z + mBounds.GetHeight() / 2.f;

	bool U = (objectBound.leftTop.z < horizontal) && (objectBound.GetHeight() + objectBound.leftTop.z < horizontal);
	bool D = objectBound.leftTop.z > horizontal;
	bool L = (objectBound.leftTop.x < vertical) && (objectBound.GetHeight() + objectBound.leftTop.x < vertical);
	bool R = objectBound.leftTop.x > vertical;

	if (R)
	{
		if (U)
		{
			idx = childUR;
		}
		else if (D)
		{
			idx = childLR;
		}
	}
	else if (L)
	{
		if (U)
		{
			idx = childUL;
		}
		else if (D)
		{
			idx = childLL;
		}
	}
	return idx;
}

void QuadTree::Split()
{
	const int childWidth = mBounds.GetWidth() / 2;
	const int childHeight = mBounds.GetHeight() / 2;

	pChildren[childUL] = std::make_shared<QuadTree>(
		mMaxObjects,
		mMaxLevels,
		mLevel + 1,
		FBox(
			FVector(mBounds.leftTop.x,              mBounds.leftTop.z              , 0.f), 
			FVector(mBounds.leftTop.x + childWidth, mBounds.leftTop.z - childHeight, 0.f)
		),
		this
	);
	pChildren[childUR] = std::make_shared<QuadTree>(
		mMaxObjects, 
		mMaxLevels, 
		mLevel + 1,
		FBox(
			FVector(mBounds.leftTop.x + childWidth    , mBounds.leftTop.z              , 0.f),
			FVector(mBounds.leftTop.x + childWidth * 2, mBounds.leftTop.z - childHeight, 0.f)
		),
		this
	);
	pChildren[childLL] = std::make_shared<QuadTree>(
		mMaxObjects, 
		mMaxLevels, 
		mLevel + 1,
		FBox(
			FVector(mBounds.leftTop.x             , mBounds.leftTop.z - childHeight      , 0.f),
			FVector(mBounds.leftTop.x + childWidth, mBounds.leftTop.z - childHeight * 2.f, 0.f)
		),
		this
	);
	pChildren[childLR] = std::make_shared<QuadTree>(
		mMaxObjects, 
		mMaxLevels, 
		mLevel + 1,
		FBox(
			FVector(mBounds.leftTop.x + childWidth      , mBounds.leftTop.z - childHeight      , 0.f),
			FVector(mBounds.leftTop.x + childWidth * 2.f, mBounds.leftTop.z - childHeight * 2.f, 0.f)
		),
		this
	);
}