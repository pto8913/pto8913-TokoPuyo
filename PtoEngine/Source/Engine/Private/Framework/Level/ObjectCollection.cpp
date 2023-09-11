
#include "Framework/Level/ObjectCollection.h"
#include "Object/Actor.h"

ObjectCollection::~ObjectCollection()
{
	Clear();
	pObjects.clear();
}

void ObjectCollection::Add(std::shared_ptr<Object> in, bool sort)
{
	if (IsValid(in))
	{
		Add(in->GetLayer(), in, sort);
	}
}
void ObjectCollection::Add(const Layer::EActorLayer& inLayer, std::shared_ptr<Object> in, bool sort)
{
	if (IsValid(in))
	{
		auto itr = pObjects.find(inLayer);
		if (itr != pObjects.end())
		{
			pObjects[inLayer].push_back(in);
		}
		else
		{
			std::vector<std::shared_ptr<Object>> arr(1, in);
			pObjects.insert(std::make_pair(inLayer, arr));
		}

		switch (inLayer)
		{
		case Layer::EActorLayer::UI:
			break;
		default:
			if (std::shared_ptr<Actor> actor = static_pointer_cast<Actor>(in))
			{
				actor->OnDestroyed.Bind<&ObjectCollection::ActorDestroyed>(*this, "World");
			}
			break;
		}
	}
}
void ObjectCollection::Append(std::vector<std::shared_ptr<Object>>& in)
{
	for (auto&& elem : in)
	{
		Add(elem, false);
	}
}

void ObjectCollection::Tick(DirectX11& dx, float deltaSec)
{
	RemovePendingObjects();

	for (auto&& elem : pObjects)
	{
		for (auto&& actor : elem.second)
		{
			if (actor->GetTickEnabled())
			{
				actor->Tick(dx, deltaSec);
			}
		}
	}
}
void ObjectCollection::Clear()
{
	for (auto&& elem : pObjects)
	{
		auto iter = elem.second.begin();
		while (iter != elem.second.end())
		{
			auto& obj = *iter;
			if (obj != nullptr)
			{
				obj->EndPlay();
				obj.reset();
				obj = nullptr;
				iter = elem.second.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}
}
void ObjectCollection::RemovePendingObjects()
{
	for (auto&& elem : pObjects)
	{
		auto iter = elem.second.begin();
		while (iter != elem.second.end())
		{
			auto& obj = *iter;
			if (obj != nullptr)
			{
				if (obj->IsPendingKill())
				{
					obj->EndPlay();
					obj.reset();
					obj = nullptr;
					iter = elem.second.erase(iter);
				}
				else
				{
					++iter;
				}
			}
			else
			{
				++iter;
			}
		}
	}
}
void ObjectCollection::ActorDestroyed(std::shared_ptr<Actor> in)
{
	for (auto&& elem : pObjects)
	{
		auto iter = elem.second.begin();
		while (iter != elem.second.end())
		{
			auto& obj = *iter;
			if (obj != nullptr)
			{
				auto actor = static_pointer_cast<Actor>(obj);
				if (actor->GetID() == in->GetID())
				{
					obj->EndPlay();
					obj.reset();
					obj = nullptr;
					iter = elem.second.erase(iter);
					break;
				}
				else
				{
					++iter;
				}
			}
			else
			{
				++iter;
			}
		}
	}
}