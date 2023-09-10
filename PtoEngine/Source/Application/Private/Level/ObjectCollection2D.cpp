
#include "Level/ObjectCollection2D.h"

#include "Actor/Actor2D.h"
#include "Actor/Actor2DTypes.h"

void ObjectCollection2D::Add(const Layer::EActorLayer& inLayer, std::shared_ptr<Object> in, bool sort)
{
	ObjectCollection::Add(inLayer, in, sort);
	if (sort)
	{
		Sort();
	}
}
void ObjectCollection2D::Append(std::vector<std::shared_ptr<Object>>& in)
{
	ObjectCollection::Append(in);
	Sort();
}
void ObjectCollection2D::Sort()
{
	auto SortCondition = [](std::shared_ptr<Object> a, std::shared_ptr<Object> b)
	{
		return static_pointer_cast<Actor2D>(a)->GetSortOrder() < static_pointer_cast<Actor2D>(b)->GetSortOrder();
	};

	for (auto&& elem : pActors)
	{
		if (!std::is_sorted(elem.second.begin(), elem.second.end(), SortCondition))
		{
			std::sort(elem.second.begin(), elem.second.end(), SortCondition);
		}
	}
}
