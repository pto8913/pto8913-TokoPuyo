#pragma once

#include "Controller/PlayerController.h"

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

protected:
	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------
	std::unique_ptr<Camera> pCamera = nullptr;
};