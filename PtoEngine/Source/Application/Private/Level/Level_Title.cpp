
#include "Level/Level_Title.h"

#include "UI/TitleUI.h"

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
	pTitleUI.reset();
	pTitleUI = nullptr;
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void Level_Title::BeginPlay(DirectX11& dx)
{
	Level::BeginPlay(dx);

	pTitleUI = std::make_shared<TitleUI>(
		this,
		dx,
		GetWorld()->GetPlayerController()->GetMouse()
	);
	pTitleUI->OnClickedTokoPuyo.Bind<&Level_Title::OnClickedTokoPuyo>(*this, "Title");
	pTitleUI->AddToViewport();
}
void Level_Title::Tick(DirectX11& dx, float deltaSec)
{
	Level::Tick(dx, deltaSec);

	if (pTitleUI != nullptr)
	{
		pTitleUI->Tick(dx, deltaSec);
	}
}

void Level_Title::OnClickedTokoPuyo(DX::MouseEvent inMouseEvent)
{
	PtoGameInstance& gameInstance = PtoGameInstance::Get();
	gameInstance.OpenWorld(static_cast<int>(EWorldId::TokoPuyo));
}

