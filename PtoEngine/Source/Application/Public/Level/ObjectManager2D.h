#pragma once

#include "Framework/Level/ObjectManager.h"
#include <algorithm>
#include "Framework/Level/Layer.h"

class DirectX11;

class Actor;

class ObjectManager2D : public ObjectManager 
{
	friend class Level2D;
public:
	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void Add(const Layer::EActorLayer& inLayer, std::shared_ptr<Object> in, bool sort = true) override;
	virtual void Append(std::vector<std::shared_ptr<Object>>& in) override;
	virtual void Sort();
};