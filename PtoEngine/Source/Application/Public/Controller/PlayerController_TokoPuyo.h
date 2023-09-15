#pragma once

#include "Framework/PlayerController.h"

class DirectX11;

class PlayerController_TokoPuyo : public PlayerController
{
public:
	PlayerController_TokoPuyo(DirectX11& dx);
	~PlayerController_TokoPuyo();

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return typeid(PlayerController_TokoPuyo).name() + std::string(" : ") + "#"s + std::to_string(mID);
	}
protected:

};