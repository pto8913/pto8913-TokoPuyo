
#include "Level/ObjectCollection2D.h"

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
	for (auto&& elem : pActors)
	{
		//std::sort(elem.second.begin(), elem.second.end(), SortCondition);
	}
}
bool ObjectCollection2D::SortCondition(std::shared_ptr<Actor2D> a, std::shared_ptr<Actor2D> b)
{
	return a->GetSortOrder() < b->GetSortOrder();
}