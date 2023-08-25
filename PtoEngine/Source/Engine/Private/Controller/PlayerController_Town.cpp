
#include "Controller/PlayerController_Town.h"

#include "Object/Character/Player.h"

#include "Core/DirectX.h"

#include "Input/Camera.h"

#include "Engine/World.h"

using namespace DirectX;

PlayerController_Town::PlayerController_Town(DirectX11& dx)
	: PlayerController(dx)
{
	pCamera = std::make_unique<Camera>(dx, "PlayerControllerTown", DirectX::XMFLOAT3(0, -5, 0), 0.f, PI / 2.f);
}
PlayerController_Town::~PlayerController_Town()
{
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void PlayerController_Town::BeginPlay(DirectX11& dx)
{
	PlayerController::BeginPlay(dx);
}
void PlayerController_Town::Tick(DirectX11& dx, float deltaSec)
{
	PlayerController::Tick(dx, deltaSec);

	if (pCamera != nullptr)
	{
		pCamera->SetLocation(GetWorld()->GetPlayer()->GetLocation());
		pCamera->ExecuteTasks(dx);
	}
}