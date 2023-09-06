
#include "Level/ObjectCollection2D.h"

#include "Actor/Actor2D.h"
#include "Actor/Actor2DTypes.h"

#include "Component/Actor2DComponent.h"

void ObjectCollection2D::Add(const Layer::EActorLayer& inLayer, std::shared_ptr<Actor> in, bool sort)
{
	ObjectCollection::Add(inLayer, in, sort);
	if (sort)
	{
		Sort();
	}
}
void ObjectCollection2D::Append(std::vector<std::shared_ptr<Actor>>& in)
{
	ObjectCollection::Append(in);
	Sort();
}
void ObjectCollection2D::Sort()
{
	auto SortCondition = [](std::shared_ptr<Actor> a, std::shared_ptr<Actor> b)
	{
		auto ac = a->GetComponent<Actor2DComponent>();
		auto bc = b->GetComponent<Actor2DComponent>();

		return ac->GetSortOrder() < bc->GetSortOrder();
	};

	for (auto&& elem : pActors)
	{
		if (!std::is_sorted(elem.second.begin(), elem.second.end(), SortCondition))
		{
			std::sort(elem.second.begin(), elem.second.end(), SortCondition);
		}
	}
}
