
#include "Framework/Level/CollisionCollection.h"

#include "Object/Actor.h"

#include "Helper/BoxHelper.h"

CollisionCollection::CollisionCollection()
{

}

CollisionCollection::~CollisionCollection()
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
void CollisionCollection::Add(std::vector<std::shared_ptr<Actor>>& in)
{
	for (auto&& actor : in)
	{
		if (auto collision = actor->GetComponent<BoxCollision>())
		{
			pCollisions.emplace_back(collision);
		}
	}
}
void CollisionCollection::Add(std::shared_ptr<BoxCollision>& in)
{
	pCollisions.emplace_back(in);
}
void CollisionCollection::Clear()
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

void CollisionCollection::Tick()
{
	mCollisionTree.Clear();
	for (auto&& collision : pCollisions)
	{
		mCollisionTree.Add(collision);
	}
	Resolve();
}
void CollisionCollection::Resolve()
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