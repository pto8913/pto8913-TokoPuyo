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
	virtual void SetOuter(std::shared_ptr<Object> inOuter);
	std::shared_ptr<Object> GetOuter();
	
	std::shared_ptr<Level> GetLevel();
	virtual std::shared_ptr<World> GetWorld() override final;

	template<typename T>
	std::shared_ptr<T> GetTypedOuter()
	{
		return std::static_pointer_cast<T>(pOuter);
	}
private:
	std::shared_ptr<Object> pOuter = nullptr;
};