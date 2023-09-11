
#include "UI/HUD.h"
#include "Slate/Message/S_ChoiceBox.h"

#include "GameSettings.h"

#include "Slate/HorizontalBox.h"
#include "Slate/VerticalBox.h"
#include "Slate/GridPanel.h"
#include "Slate/CanvasPanel.h"
#include "Slate/Overlay.h"
#include "Slate/ProgressBar.h"
#include "Slate/TextBlock.h"
#include "Slate/Border.h"

#include "Framework/World.h"
#include "Level/Level2D.h"

#include "EngineSettings.h"

#include "Actor/Ground/GroundBase.h"
#include "Actor/Character/CharacterBase.h"
#include "Actor/Event/EventBase.h"

#include "GameState/GameState_Dungeon.h"
#include <memory>

#define __DEBUG 0

#if __DEBUG
#include <format>
#include "Component/BoxCollision2D.h"
#include "Actor/Actor2D.h"
#endif

using namespace DirectX;

HUD::HUD(Object* inOwner, DirectX11& dx, DX::IMouseInterface* mouse)
	: UserWidget(
		inOwner, 
		dx, 
		mouse, 
		EngineSettings::GetWindowSize().x,
		EngineSettings::GetWindowSize().y
	)
{
	pRootSlate = std::make_shared<S_CanvasPanel>(FVector2D(EngineSettings::GetWindowSize().x, EngineSettings::GetWindowSize().y), GetRt2D());

	// HP Bar
	{
		FSlateInfos textInfos;
		textInfos.HAlign = EHorizontalAlignment::Left;
		textInfos.VAlign = EVerticalAlignment::Center;
		auto pHPText = std::make_shared<S_TextBlock>(GetRt2D(), textInfos);
		pHPText->SetText(L"HP : ");

		FSlateInfos infos;
		infos.HAlign = EHorizontalAlignment::Center;
		infos.VAlign = EVerticalAlignment::Center;

		pHPBarText = std::make_shared<S_TextBlock>(GetRt2D(), infos);
		pHPBarText->SetText(L"0 / 0");

		infos.padding = FRect(1.f, 1.f, 1.f, 1.f);

		FSlateProgressAppearance HPBarAppear;
		HPBarAppear.BackgroundColor = FColor(0.5f, 0.5f, 0.5f);
		HPBarAppear.ProgressColor = FColor(0.f, 0.75f, 0.5f);

		pHPBar = std::make_shared<S_ProgressBar>(GetRt2D(), infos, HPBarAppear);
		pHPBar->SetPercent(0.64f);

		FSlateInfos hbInfos;
		hbInfos.HAlign = EHorizontalAlignment::Fill;
		hbInfos.VAlign = EVerticalAlignment::Center;
		auto pHPBarOverlay = std::make_shared<S_Overlay>(GetRt2D(), hbInfos);
		pHPBarOverlay->SetSize(FVector2D(200, 20));

		auto pHPBarHB = std::make_shared<S_HorizontalBox>(FVector2D(400.f, 30.f), GetRt2D());
		pHPBarHB->SetPosition(GameSettings::GAMESCREEN_PADDING);

		pRootSlate->AddChild(pHPBarHB);

		pHPBarHB->AddChild(pHPText);
		pHPBarHB->AddChild(pHPBarOverlay);

		pHPBarOverlay->AddChild(pHPBar);
		pHPBarOverlay->AddChild(pHPBarText);
	}

	// Game Message
	{

	}

	// Game Infos
	{
		auto pGameInfosVB = std::make_shared<S_VerticalBox>(FVector2D(300.f, GameSettings::GAMEUI_RIGHT_BOTTOM.y), GetRt2D());
		//pGameInfosVB->SetPosition(FVector2D(0,0));
		pGameInfosVB->SetPosition({ GameSettings::GAMEUI_LEFT_TOP.x, GameSettings::GAMEUI_LEFT_TOP.y });
		pRootSlate->AddChild(pGameInfosVB);

		// Floor Name
		{
			FSlateInfos infos;
			infos.HAlign = EHorizontalAlignment::Center;
			infos.VAlign = EVerticalAlignment::Center;

			FSlateTextAppearance appearance;
			appearance.hAlign = EHorizontalAlignment::Center;

			pFloorText = std::make_shared<S_TextBlock>(GetRt2D(), infos, FSlateFont(), appearance);
			auto gameState = static_pointer_cast<GameState_Dungeon>(GetWorld()->GetGameState());
			pFloorText->SetText(gameState->GetDungeonFloorName());

			FSlateBorderAppearance boxAppearance;
			boxAppearance.bIsFill = false;
			boxAppearance.color = FColor(0.5f, 0.5f, 0.5f, 1.f);
			boxAppearance.Type = EBorderType::Box;
			boxAppearance.lineWidth = 10.f;

			FSlateInfos boxInfos;
			boxInfos.padding.top = 0.f;
			boxInfos.padding.bottom = 5.f;

			auto pbox = std::make_shared<S_Border>(FVector2D(300.f, 50.f), GetRt2D(), boxInfos, boxAppearance);

			pGameInfosVB->AddChild(pbox);
			pbox->AddChild(pFloorText);
		}

		// Map
		{
			FSlateInfos infos;
			infos.HAlign = EHorizontalAlignment::Center;
			infos.VAlign = EVerticalAlignment::Center;

			pMapGP = std::make_shared<S_GridPanel>(GetRt2D(), infos);

			FSlateBorderAppearance boxAppearance;
			boxAppearance.bIsFill = false;
			boxAppearance.color = FColor(0.5f, 0.5f, 0.5f, 1.f);
			boxAppearance.Type = EBorderType::Box;
			boxAppearance.lineWidth = 10.f;

			FSlateInfos boxInfos;
			boxInfos.padding.top = 5.f;
			boxInfos.padding.bottom = 5.f;

			auto pbox = std::make_shared<S_Border>(FVector2D(300.f, 300.f), GetRt2D(), boxInfos, boxAppearance);
			pGameInfosVB->AddChild(pbox);
			pbox->AddChild(pMapGP);
		}

		// Inventory
		{
		}

		pGameInfosVB->UpdateWidget();
	}
#if __DEBUG
	DrawDebugScreen();
#endif

	pRootSlate->SetPosition({ 0, 0 });
	pRootSlate->UpdateWidget();
}
HUD::HUD(DirectX11& dx, DX::IMouseInterface* mouse)
	: HUD(nullptr, dx, mouse)
{
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------

void HUD::Draw()
{
	UserWidget::Draw();

#if __DEBUG
	for (auto elem : pBoxDebug)
	{
		auto ptr = static_cast<Actor2D*>(elem->GetOwner());
		auto v = ptr->Get2DIdx();
		auto level = static_pointer_cast<Level2D>(GetWorld()->GetLevel());
		if (level->IsInScreen(v.x, v.y))
		{
			auto ground = level->GetGroundLayer(v.x, v.y);
			if (ground != nullptr)
			{
				auto s = ptr->GetActorLocation();
				switch (ConvertToGroundTile(ground->GetGroundType()))
				{
				case EGroundTile::Wall:
					elem->DrawDebug(GetRt2D());
					break;
				default:
					break;
				}
			}
		}
	}
	for (auto&& elem : pScreenGrid)
	{
		elem->Draw();
	}
#endif
}

// --------------------------
// Main : Status
// --------------------------
void HUD::OnHPChanged(int inCurrent, int inMax)
{
	pHPBar->SetPercent(float(inCurrent / inMax));
	pHPBarText->SetText(std::format(L"{} / {}", inCurrent, inMax).c_str());
}

// -----------------------------------------------------
// Main : Game Infos
// -----------------------------------------------------
void HUD::NextFloor(const Level2D* pLevel)
{
	ResetMap(pLevel);

	auto gameState = static_pointer_cast<GameState_Dungeon>(GetWorld()->GetGameState());
	SetFloorName(gameState->GetDungeonFloorName());
}
void HUD::PlayerMoved(const FVector& inPos)
{
	auto level = static_pointer_cast<Level2D>(GetWorld()->GetLevel());
	UpdateMap(level.get());
}

void HUD::SetFloorName(const std::wstring in)
{
	pFloorText->SetText(in);
}
void HUD::ResetMap(const Level2D* pLevel)
{
	pMapGP->ClearChildren();

	FSlateGridPanelAppearance gpAppearance;
	gpAppearance.column = pLevel->GetWidth();
	gpAppearance.row = pLevel->GetHeight();
	pMapGP->SetAppearance(gpAppearance);

	pMapGP->SetSize({
		mapSize * pLevel->GetWidth(),
		mapSize * pLevel->GetHeight()
	});

	for (int y = 0; y < pLevel->GetHeight(); ++y)
	{
		for (int x = 0; x < pLevel->GetWidth(); ++x)
		{
			FSlateInfos infos;
			infos.HAlign = EHorizontalAlignment::Fill;
			infos.VAlign = EVerticalAlignment::Fill;
			auto pCell = std::make_shared<S_Border>(GetRt2D(), infos);
			pCell->SetSize({ mapSize, mapSize });
			pMapGP->AddChild(pCell);
		}
	}

	pRootSlate->UpdateWidget();
}
void HUD::UpdateMap(const Level2D* pLevel)
{
	if (pMapGP != nullptr)
	{
		if (pMapGP->GetChildrenCount() == 0)
		{
			ResetMap(pLevel);
		}
		else
		{
			for (int y = 0; y < pLevel->GetHeight(); ++y)
			{
				for (int x = 0; x < pLevel->GetWidth(); ++x)
				{
					auto& slot = pMapGP->GetChildAt(x, y);
					auto pCell = static_pointer_cast<S_Border>(slot);
					pCell->GetAppearance().color = FColor(0, 0, 0);
					SetMap(pLevel, pCell, x, y);
				}
			}
		}
	}
}
void HUD::SetMap(const Level2D* pLevel, std::shared_ptr<S_Border> pCell, const int& x, const int& y)
{
	const auto& ground = pLevel->GetGroundLayer(x, y);
	if (ground != nullptr)
	{
		FSlateBorderAppearance apperance;
		apperance.Type = EBorderType::Box;
		switch (ConvertToGroundTile(ground->GetGroundType()))
		{
		case EGroundTile::Room:
		case EGroundTile::Path:
			apperance.color = FColor(0.f, 0.f, 1.f);
			break;
		default:
			apperance.color = FColor(0.f, 0.f, 0.f);
			break;
		}
		pCell->SetAppearance(apperance);
	}

	const auto& eventData = pLevel->GetEventLayer(x, y);
	if (eventData != nullptr)
	{
		if (eventData->GetEventType() == EEventId::Exit)
		{
			FSlateBorderAppearance apperance;
			apperance.Type = EBorderType::Box;
			apperance.color = FColor(1.f, 0.f, 1.f);
			pCell->SetAppearance(apperance);
		}
	}

	const auto& character = pLevel->GetCharacterLayer(x, y);
	if (character != nullptr)
	{
		if (character->GetCharacterType() != ECharacterId::Player)
		{
			FSlateBorderAppearance apperance;
			apperance.Type = EBorderType::Border;
			apperance.color = FColor(0.f, 1.f, 1.f);
			apperance.roundSize = { 2.5f, 2.5f };
			pCell->SetAppearance(apperance);
		}
		else
		{
			FSlateBorderAppearance apperance;
			apperance.Type = EBorderType::Border;
			apperance.color = FColor(1.f, 1.f, 1.f);
			apperance.roundSize = { 2.5f, 2.5f };
			pCell->SetAppearance(apperance);
		}
	}
}

// -----------------------------------------------------
// Main : Debug
// -----------------------------------------------------
void HUD::AddBoxDebug(std::shared_ptr<BoxCollision2D> in)
{
#if __DEBUG
	pBoxDebug.push_back(in);
#endif
}

void HUD::DrawDebugScreen()
{
#if __DEBUG
	FSlateInfos infos;
	infos.HAlign = EHorizontalAlignment::Fill;
	infos.VAlign = EVerticalAlignment::Fill;

	FSlateBorderAppearance apperance;
	apperance.Type = EBorderType::Box;
	apperance.color = FColor(1.f, 1.f, 0.f);
	apperance.bIsFill = true;
	auto cell = EngineSettings::GetGameScreen2DCellSize();
	for (int y = 0; y < 20; ++y)
	{
		for (int x = 0; x < 20; x++)
		{
			auto ptr = std::make_shared<S_Border>(FVector2D(2.f, 2.f), GetRt2D(), infos, apperance);
			ptr->SetPosition(FVector2D(cell.x * x, cell.y * y));
			pScreenGrid.push_back(ptr);
		}
	}
#endif
}