#pragma once

#include "Framework/PlayerController.h"

class Camera;

class PlayerController_Town : public PlayerController
{
public:
	PlayerController_Town(DirectX11& dx);
	~PlayerController_Town();

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	virtual void BeginPlay(DirectX11& dx) override;
	virtual void Tick(DirectX11& dx, float deltaSec) override;
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return typeid(PlayerController_Town).name() + std::string(" : ") + "#"s + std::to_string(mID);
	}
protected:
	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------
	std::unique_ptr<Camera> pCamera = nullptr;
};