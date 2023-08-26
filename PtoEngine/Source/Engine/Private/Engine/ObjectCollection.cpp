
#include "Engine/ObjectCollection.h"
#include "Object/Actor2D.h"

void ObjectCollection::Add(std::shared_ptr<Actor2D>& in)
{
	auto layer = in->GetLayer();
	auto itr = pActors.find(layer);
	if (itr != pActors.end())
	{
		pActors[layer].push_back(in);
	}
	else
	{
		std::vector<std::shared_ptr<Actor2D>> arr(1, in);
		pActors.insert(std::make_pair(layer, arr));
	}
	pActors.emplace(layer, in);
}
void ObjectCollection::Append(std::vector<std::shared_ptr<Actor2D>>& in)
{
	for (auto&& elem : in)
	{
		Add(elem);
	}
	Sort();
}

void ObjectCollection::Sort()
{
	auto LayerSort = [](std::shared_ptr<Actor2D> a, std::shared_ptr<Actor2D> b)
	{
		return a->GetSortOrder() < b->GetSortOrder();
	};

	for (auto&& elem : pActors)
	{
		std::sort(elem.second.begin(), elem.second.end(), LayerSort);
	}
}
void ObjectCollection::Tick(DirectX11& dx, float deltaSec)
{
	for (auto&& elem : pActors)
	{
		for (auto&& actor : elem.second)
		{
			if (actor->GetVisibility())
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
			auto obj = *iter;
			if (obj != nullptr)
			{
				iter = elem.second.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}
}