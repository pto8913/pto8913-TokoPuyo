#pragma once

#include <map>
#include <memory>
#include <vector>
#include "Actor/Actor2DTypes.h"

class Actor2D;
class DirectX11;

template<typename TClass, typename = typename std::enable_if_t<std::is_base_of_v<Actor2D, TClass>>>
struct LayerObject
{
public:


	std::vector<std::vector<TClass>> pActors;
};

class ObjectCollection
{
	friend class Level2D;
public:
	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	template<typename TClass, typename = typename std::enable_if_t<std::is_base_of_v<Actor2D, TClass>>>
	void Add(std::shared_ptr<TClass> in, bool sort = true)
	{
		Add(in->GetLayer(), static_pointer_cast<Actor2D>(in), sort);
	}
	void Add(const EActor2DLayer& inLayer, std::shared_ptr<Actor2D> in, bool sort = true);
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