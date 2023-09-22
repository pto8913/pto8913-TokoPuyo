
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
	if (IsValid(pTitleUI))
	{
		pTitleUI->MarkPendingKill();
	}
	pTitleUI = nullptr;

	if (IsValid(pSettingsUI))
	{
		pSettingsUI->MarkPendingKill();
	}
	pSettingsUI = nullptr;
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
	FTimerDelegate openSettingsUI;
	openSettingsUI.SetTimer<&Level_Title::OpenSettingsUI>(*this, 0.f, false, 0.01f);
	GetWorld()->GetTimerManager().SetNextTickTimer(openSettingsUI);
}
void Level_Title::OpenSettingsUI()
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
		pSettingsUI->OnClickedReturnToTitle.Bind<&Level_Title::OnClickedCloseSettings>(*this, GetName());
		pSettingsUI->_OnAudioVolumeChanged.Bind<&Level_Title::OnAudioVolumeChanged>(*this, GetName());
	}
	pSettingsUI->AddToViewport();
}
void Level_Title::OnClickedCloseSettings(DX::MouseEvent inMouseEvent)
{
	if (pSettingsUI != nullptr)
	{
		pSettingsUI->RemoveFromParent();
	}
	pTitleUI->AddToViewport();
}
void Level_Title::OnAudioVolumeChanged(float inValue)
{
	PtoGameInstance& gameInstance = PtoGameInstance::Get();
	gameInstance.OnAudioVolumeChanged(inValue);
}