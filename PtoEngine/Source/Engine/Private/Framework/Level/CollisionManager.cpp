
#include "Framework/Level/CollisionManager.h"

#include "Object/Actor.h"

#include "Helper/BoxHelper.h"

CollisionManager::CollisionManager()
{

}

CollisionManager::~CollisionManager()
{
	for (auto&& elem : pCollisions)
	{
		if (elem != nullptr)
		{
			elem.reset();
			elem = nullptr;
		}
	}
	pCollisions.clear();
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void CollisionManager::Add(std::vector<std::shared_ptr<Actor>>& in)
{
	for (auto&& actor : in)
	{
		if (auto collision = actor->GetComponent<BoxCollision>())
		{
			pCollisions.emplace_back(collision);
		}
	}
}
void CollisionManager::Add(std::shared_ptr<BoxCollision>& in)
{
	pCollisions.emplace_back(in);
}
void CollisionManager::Clear()
{
	auto iter = pCollisions.begin();
	while (iter != pCollisions.end())
	{
		auto obj = *iter;

		if (obj != nullptr)
		{
			obj.reset();
			obj = nullptr;
			iter = pCollisions.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CollisionManager::Tick()
{
	mCollisionTree.Clear();
	for (auto&& collision : pCollisions)
	{
		mCollisionTree.Add(collision);
	}
	Resolve();
}
void CollisionManager::Resolve()
{
	for (auto&& collision : pCollisions)
	{
		auto treeCollisions = mCollisionTree.Search(collision->GetBoundingBox());
		for (auto&& treeCollision : treeCollisions)
		{
			if (treeCollision->GetOwner()->GetID() == collision->GetOwner()->GetID())
			{
				continue;
			}

			if (Box::IsInBox(collision->GetBoundingBox(), treeCollision->GetBoundingBox()))
			{
				collision->EnterVolume(treeCollision);
			}
		}
	}
}