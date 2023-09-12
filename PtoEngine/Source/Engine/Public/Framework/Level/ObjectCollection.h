#pragma once

#include <unordered_map>
#include <memory>
#include <vector>
#include <algorithm>
#include <string>
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

	virtual Object* GetObjectByID(int id, Layer::EActorLayer inLayer = Layer::EActorLayer::InValid);
	virtual Object* GetObjectByTag(const std::wstring& inTag, Layer::EActorLayer inLayer = Layer::EActorLayer::InValid);
protected:
	virtual void RemovePendingObjects();

	using MapType = std::unordered_map<Layer::EActorLayer, std::vector<std::shared_ptr<Object>>>;
public:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	MapType pObjects;
};