#pragma once

#include "Framework/Level/ObjectCollection.h"
#include <algorithm>
#include "Framework/Level/Layer.h"

class DirectX11;

class Actor;

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
};