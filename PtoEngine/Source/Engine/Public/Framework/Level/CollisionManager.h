#pragma once

#include "QuadTree.h"

class BoxCollision;
class Actor;

class CollisionManager
{
public:
	CollisionManager();
	virtual ~CollisionManager();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	void Add(std::vector<std::shared_ptr<Actor>>& in);
	void Add(std::shared_ptr<BoxCollision>& in);

	void Clear();
	void Tick();
	void Resolve();
protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	std::vector<std::shared_ptr<BoxCollision>> pCollisions;
	QuadTree mCollisionTree;
};