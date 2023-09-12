#pragma once

#include <map>
#include <memory>
#include <vector>
#include <algorithm>
#include "Layer.h"

class DirectX11;
class Actor;
class Object;

class ObjectCollection
{
public:
	virtual ~ObjectCollection();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void Add(std::shared_ptr<Object> in, bool sort = true);
	virtual void Add(const Layer::EActorLayer& inLayer, std::shared_ptr<Object> in, bool sort = true);
	virtual void Append(std::vector<std::shared_ptr<Object>>& in);
	virtual void Tick(DirectX11& dx, float deltaSec);
	virtual void Clear();

protected:
	void RemovePendingObjects();

	using MapType = std::map<Layer::EActorLayer, std::vector<std::shared_ptr<Object>>>;
	using IterType = MapType::iterator;
	using ConstIterType = MapType::const_iterator;
public:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	MapType pObjects;
};