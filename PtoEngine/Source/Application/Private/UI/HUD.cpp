
#include "UI/HUD.h"
#include "Slate/Message/S_ChoiceBox.h"

#include "GameSettings.h"

#include "Slate/HorizontalBox.h"
#include "Slate/VerticalBox.h"
#include "Slate/GridPanel.h"
#include "Slate/CanvasPanel.h"
#include "Slate/Overlay.h"
#include "Slate/TextBlock.h"
#include "Slate/Border.h"

#include "Framework/World.h"
#include "Level/Level2D.h"

#include "EngineSettings.h"

#include <memory>

#define __DEBUG 0

#if __DEBUG
#include <format>
#include "Component/BoxCollision2D.h"
#include "Actor/Actor2D.h"
#endif

using namespace DirectX;

HUD::HUD(std::shared_ptr<Object> inOwner, DirectX11& dx, DX::IMouseInterface* mouse)
	: UserWidget(
		inOwner, 
		dx, 
		mouse, 
		EngineSettings::GetWindowSize().x,
		EngineSettings::GetWindowSize().y
	)
{
	pRootSlate = std::make_shared<S_CanvasPanel>(FVector2D(EngineSettings::GetWindowSize().x, EngineSettings::GetWindowSize().y), GetRt2D());

	// Game Message
	{

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