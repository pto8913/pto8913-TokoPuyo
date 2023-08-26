#pragma once

#include <map>
#include <memory>
#include <vector>
#include "Object/Actor2DTypes.h"

class Actor2D;
class DirectX11;

class ObjectCollection
{
public:
	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	void Add(std::shared_ptr<Actor2D>& in);
	void Append(std::vector<std::shared_ptr<Actor2D>>& in);
	void Sort();
	void Tick(DirectX11& dx, float deltaSec);
	void Remove();

protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	std::map<EActor2DLayer, std::vector<std::shared_ptr<Actor2D>>> pActors;
};