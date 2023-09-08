#pragma once

#include <map>
#include <memory>
#include <vector>
#include <algorithm>
#include "Layer.h"

class DirectX11;
class Actor;

class ObjectCollection
{
public:
	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void Add(std::shared_ptr<Actor> in, bool sort = true);
	virtual void Add(const Layer::EActorLayer& inLayer, std::shared_ptr<Actor> in, bool sort = true);
	virtual void Append(std::vector<std::shared_ptr<Actor>>& in);
	virtual void Tick(DirectX11& dx, float deltaSec);
	virtual void Remove();

protected:
	virtual void ActorDestroyed(std::shared_ptr<Actor> in);

	using MapType = std::map<Layer::EActorLayer, std::vector<std::shared_ptr<Actor>>>;
	using IterType = MapType::iterator;
	using ConstIterType = MapType::const_iterator;
public:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	MapType pActors;
};