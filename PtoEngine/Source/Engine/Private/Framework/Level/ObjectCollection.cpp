
#include "Framework/Level/ObjectCollection.h"
#include "Object/Actor.h"

void ObjectCollection::Add(std::shared_ptr<Actor> in, bool sort)
{
	Add(in->GetLayer(), in, sort);
}
void ObjectCollection::Add(const Layer::EActorLayer& inLayer, std::shared_ptr<Actor> in, bool sort)
{
	auto itr = pActors.find(inLayer);
	if (itr != pActors.end())
	{
		pActors[inLayer].push_back(in);
	}
	else
	{
		std::vector<std::shared_ptr<Actor>> arr(1, in);
		pActors.insert(std::make_pair(inLayer, arr));
	}

	in->OnDestroyed.Bind<&ObjectCollection::ActorDestroyed>(*this, "World");
}
void ObjectCollection::Append(std::vector<std::shared_ptr<Actor>>& in)
{
	for (auto&& elem : in)
	{
		Add(elem, false);
	}
}

void ObjectCollection::Tick(DirectX11& dx, float deltaSec)
{
	for (auto&& elem : pActors)
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
void ObjectCollection::Remove()
{
	for (auto&& elem : pActors)
	{
		auto iter = elem.second.begin();
		while (iter != elem.second.end())
		{
			auto& obj = *iter;
			if (obj != nullptr)
			{
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
void ObjectCollection::ActorDestroyed(std::shared_ptr<Actor> in)
{
	for (auto&& elem : pActors)
	{
		auto iter = elem.second.begin();
		while (iter != elem.second.end())
		{
			auto& obj = *iter;
			if (obj != nullptr)
			{
				if (obj->GetID() == in->GetID())
				{
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