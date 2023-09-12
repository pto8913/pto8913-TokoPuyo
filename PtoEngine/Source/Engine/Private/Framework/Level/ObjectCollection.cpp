
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
Object* ObjectCollection::GetObjectByID(int id, Layer::EActorLayer inLayer)
{
	if (id == -1)
	{
		return nullptr;
	}
	if (inLayer == Layer::EActorLayer::InValid)
	{
		for (auto&& layer : pObjects)
		{
			for (auto&& obj : layer.second)
			{
				if (obj->GetID() == id)
				{
					return obj.get();
				}
			}
		}
	}
	else
	{
		for (auto&& obj : pObjects[inLayer])
		{
			if (obj->GetID() == id)
			{
				return obj.get();
			}
		}
	}
	return nullptr;
}
Object* ObjectCollection::GetObjectByTag(const std::wstring& inTag, Layer::EActorLayer inLayer)
{
	if (inTag.empty())
	{
		return nullptr;
	}
	if (inLayer == Layer::EActorLayer::InValid)
	{
		for (auto&& layer : pObjects)
		{
			for (auto&& obj : layer.second)
			{
				if (obj->HasTag(inTag))
				{
					return obj.get();
				}
			}
		}
	}
	else
	{
		for (auto&& obj : pObjects[inLayer])
		{
			if (obj->HasTag(inTag))
			{
				return obj.get();
			}
		}
	}
	return nullptr;
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
