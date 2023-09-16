
#include "Level/Level_Title.h"

#include "UI/TitleUI.h"
#include "UI/SettingsUI.h"
#include "UI/DebugUI.h"

#include "Framework/World.h"
#include "Framework/PlayerController.h"

#include "PtoGameInstance.h"
#include "World/WorldTypes.h"

Level_Title::Level_Title(DirectX11& dx)
	: Level(dx)
{
}
Level_Title::~Level_Title()
{
	if (pTitleUI)
	{
		pTitleUI->MarkPendingKill();
	}
	pTitleUI = nullptr;

	if (pSettingsUI)
	{
		pSettingsUI->MarkPendingKill();
	}
	pSettingsUI = nullptr;

	if (pDebugUI)
	{
		pDebugUI->MarkPendingKill();
	}
	pDebugUI = nullptr;
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void Level_Title::BeginPlay(DirectX11& dx)
{
	Level::BeginPlay(dx);

	pTitleUI = GetWorld()->CreateWidget<TitleUI>(
		this,
		dx,
		GetWorld()->GetPlayerController()->GetMouse()
	);
	pTitleUI->OnClickedTokoPuyo.Bind<&Level_Title::OnClickedTokoPuyo>(*this, "Title");
	pTitleUI->OnClickedOpenSettings.Bind<&Level_Title::OnClickedOpenSettings>(*this, "Title");
	pTitleUI->AddToViewport();

	pDebugUI = GetWorld()->CreateWidget<DebugUI>(
		this, 
		dx, 
		GetWorld()->GetPlayerController()->GetMouse()
	);
	pDebugUI->AddToViewport();
}

// --------------------------
// Main : Play TokoPuyo
// --------------------------
void Level_Title::OnClickedTokoPuyo(DX::MouseEvent inMouseEvent)
{
	PtoGameInstance& gameInstance = PtoGameInstance::Get();
	gameInstance.OpenWorld(static_cast<int>(EWorldId::TokoPuyo));
}

// --------------------------
// Main : SettingsUI
// --------------------------
void Level_Title::OnClickedOpenSettings(DX::MouseEvent inMouseEvent)
{
	if (pTitleUI != nullptr)
	{
		pTitleUI->RemoveFromParent();
	}
	if (pSettingsUI == nullptr)
	{
		pSettingsUI = GetWorld()->CreateWidget<SettingsUI>(
			GetWorld(),
			*pDX,
			GetWorld()->GetPlayerController()->GetMouse()
		);
		//pSettingsUI->OnClickedReturnToTitle.Bind<&Level_Title::OnClickedCloseSettings>(*this, "Title");
	}
	//pSettingsUI->AddToViewport();
}
void Level_Title::OnClickedCloseSettings(DX::MouseEvent inMouseEvent)
{
	if (pSettingsUI != nullptr)
	{
		pSettingsUI->RemoveFromParent();
	}
	pTitleUI->AddToViewport();
}
