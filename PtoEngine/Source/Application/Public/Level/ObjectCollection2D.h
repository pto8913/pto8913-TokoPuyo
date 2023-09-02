#pragma once

#include "Framework/Level/ObjectCollection.h"
#include "Actor/Actor2D.h"
#include <algorithm>

class DirectX11;

class ObjectCollection2D : public ObjectCollection 
{
	friend class Level2D;
public:
	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void Add(const Layer::EActorLayer& inLayer, std::shared_ptr<Actor> in, bool sort = true) override;
	virtual void Append(std::vector<std::shared_ptr<Actor>>& in) override;
	virtual void Sort();
	virtual bool SortCondition(std::shared_ptr<Actor2D> a, std::shared_ptr<Actor2D> b);
};