#pragma once

#include "Object.h"

class DirectX11;

class Level;
class World;

class Actor : public Object
{
public:
	Actor();
	virtual ~Actor();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------

	// -----------------------------------
	// Main : Util
	// -----------------------------------
	void SetOuter(std::shared_ptr<Object> inOuter);
	std::shared_ptr<Object> GetOuter();
	
	Level* GetLevel();
	std::shared_ptr<World> GetWorld();

	template<typename T>
	T* GetTypedOuter()
	{
		return static_cast<T*>(pOuter.get());
	}
private:
	std::shared_ptr<Object> pOuter = nullptr;
};