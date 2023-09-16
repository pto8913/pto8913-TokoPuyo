#pragma once

#include "Framework/World.h"

class World_Title : public World
{
public:
	virtual void SetLevel(DirectX11& dx) override;

	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return "World_Title" + "#"s + std::to_string(mID);
	}
};