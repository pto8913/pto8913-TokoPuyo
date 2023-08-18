
#include "UI/HUD.h"
#include "UI/Message/S_ChoiceBox.h"

#include "GameSettings.h"

#include "UI/Slate/HorizontalBox.h"
#include "UI/Slate/VerticalBox.h"
#include "UI/Slate/GridPanel.h"
#include "UI/Slate/CanvasPanel.h"
#include "UI/Slate/Overlay.h"
#include "UI/Slate/ProgressBar.h"
#include "UI/Slate/TextBlock.h"
#include "UI/Slate/Border.h"

#include "Engine/World.h"
#include "Level/Level.h"

#include "Core/AppSettings.h"

#include "Object/Ground/GroundBase.h"
#include "Object/Character/CharacterBase.h"
#include "Object/Event/EventBase.h"

#define _DEBUG 1

#if _DEBUG
#include <format>
#endif

using namespace DirectX;

HUD::HUD(std::shared_ptr<Object> inOwner, DirectX11& dx, DX::IMouseInterface* mouse, UINT windowSizeW, UINT windowSizeH)
	: UserWidget(inOwner, dx, mouse, windowSizeW, windowSizeH)
{
	m_pRootSlate = std::make_shared<S_CanvasPanel>(XMFLOAT2(windowSizeW, windowSizeH), m_pRt2D);

	// HP Bar
	{
		FSlateInfos textInfos;
		textInfos.HAlign = EHorizontalAlignment::Left;
		textInfos.VAlign = EVerticalAlignment::Center;
		auto pHPText = std::make_shared<S_TextBlock>(m_pRt2D, textInfos);
		pHPText->SetText(L"HP:");

		FSlateInfos infos;
		infos.HAlign = EHorizontalAlignment::Center;
		infos.VAlign = EVerticalAlignment::Center;

		m_pHPText = std::make_shared<S_TextBlock>(m_pRt2D, infos);
		m_pHPText->SetText(L"0 / 0");

		infos.padding = { 1.f,1.f,1.f };

		FSlateProgressAppearance HPBarAppear;
		HPBarAppear.BackgroundColor = FColor(0.5f, 0.5f, 0.5f);
		HPBarAppear.ProgressColor = FColor(0.f, 0.75f, 0.5f);

		m_pHPBar = std::make_shared<S_ProgressBar>(m_pRt2D, infos, HPBarAppear);
		m_pHPBar->SetPercent(0.64f);

		FSlateInfos hbInfos;
		hbInfos.HAlign = EHorizontalAlignment::Fill;
		hbInfos.VAlign = EVerticalAlignment::Center;
		auto pHPBarOverlay = std::make_shared<S_Overlay>(m_pRt2D, hbInfos);
		pHPBarOverlay->SetSize(XMFLOAT2(200, 20));

		auto pHPBarHB = std::make_shared<S_HorizontalBox>(XMFLOAT2(400.f, 30.f), m_pRt2D);
		pHPBarHB->SetPosition(GameSettings::GAMESCREEN_PADDING);

		m_pRootSlate->AddChild(pHPBarHB);

		pHPBarHB->AddChild(pHPText);
		pHPBarHB->AddChild(pHPBarOverlay);

		pHPBarOverlay->AddChild(m_pHPBar);
		pHPBarOverlay->AddChild(m_pHPText);
	}

	// Game Message
	{

	}

	// Game Infos
	{
		auto pGameInfosVB = std::make_shared<S_VerticalBox>(XMFLOAT2(300.f, GameSettings::GAMEUI_RIGHT_BOTTOM.y), m_pRt2D);
		//pGameInfosVB->SetPosition(XMFLOAT2(0,0));
		pGameInfosVB->SetPosition({ GameSettings::GAMEUI_LEFT_TOP.x, GameSettings::GAMEUI_LEFT_TOP.y });
		m_pRootSlate->AddChild(pGameInfosVB);

		// Floor Name
		{
			FSlateInfos infos;
			infos.HAlign = EHorizontalAlignment::Center;
			infos.VAlign = EVerticalAlignment::Center;

			FSlateTextAppearance appearance;
			appearance.hAlign = EHorizontalAlignment::Center;

			m_pFloorText = std::make_shared<S_TextBlock>(m_pRt2D, infos, FSlateFont(), appearance);
			m_pFloorText->SetText(L"É\ÉmçzéR1F");

			FSlateBorderAppearance boxAppearance;
			boxAppearance.bIsFill = false;
			boxAppearance.color = FColor(0.5f, 0.5f, 0.5f, 1.f);
			boxAppearance.Type = EBorderType::Box;
			boxAppearance.lineWidth = 10.f;

			FSlateInfos boxInfos;
			boxInfos.padding.top = 0.f;
			boxInfos.padding.bottom = 5.f;

			auto pbox = std::make_shared<S_Border>(XMFLOAT2(300.f, 50.f), m_pRt2D, boxInfos, boxAppearance);

			pGameInfosVB->AddChild(pbox);
			pbox->AddChild(m_pFloorText);
		}

		// Map
		{
			FSlateInfos infos;
			infos.HAlign = EHorizontalAlignment::Center;
			infos.VAlign = EVerticalAlignment::Center;

			m_pMapGP = std::make_shared<S_GridPanel>(m_pRt2D, infos);

			FSlateBorderAppearance boxAppearance;
			boxAppearance.bIsFill = false;
			boxAppearance.color = FColor(0.5f, 0.5f, 0.5f, 1.f);
			boxAppearance.Type = EBorderType::Box;
			boxAppearance.lineWidth = 10.f;

			FSlateInfos boxInfos;
			boxInfos.padding.top = 5.f;
			boxInfos.padding.bottom = 5.f;

			auto pbox = std::make_shared<S_Border>(XMFLOAT2(300.f, 300.f), m_pRt2D, boxInfos, boxAppearance);
			pGameInfosVB->AddChild(pbox);
			pbox->AddChild(m_pMapGP);
		}

		// Inventory
		{
		}

		pGameInfosVB->UpdateWidget();
	}

	m_pRootSlate->SetPosition({ 0, 0 });
	m_pRootSlate->UpdateWidget();
}
HUD::HUD(DirectX11& dx, DX::IMouseInterface* mouse, UINT windowSizeW, UINT windowSizeH)
	: HUD(nullptr, dx, mouse, windowSizeW, windowSizeH)
{
}

void HUD::Draw()
{
	m_pRootSlate->Draw();
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------

// --------------------------
// Main : Status
// --------------------------
void HUD::OnHPChanged(int inCurrent, int inMax)
{
	m_pHPBar->SetPercent(inCurrent / inMax);
	m_pHPText->SetText(std::format(L"{} / {}", inCurrent, inMax).c_str());
}

// -----------------------------------------------------
// Main : HUDInterface
// -----------------------------------------------------
ID2D1RenderTarget* HUD::GetRt2D()
{
	return m_pRt2D;
}

// --------------------------
// Main : HUDInterface : External
// --------------------------
void HUD::AddSlate(std::shared_ptr<SlateBase> inSlate)
{
	m_pRootSlate->AddChild(inSlate);
	m_pRootSlate->UpdateWidget();
}
void HUD::RemoveSlate(std::shared_ptr<SlateBase> inSlate)
{
	m_pRootSlate->RemoveChild(inSlate);
	m_pRootSlate->UpdateWidget();
}

// --------------------------
// Main : HUDInterface : Game Infos : Map
// --------------------------
void HUD::ResetMap(const Level2D* pLevel)
{
	m_pMapGP->ClearChildren();

	FSlateGridPanelAppearance gpAppearance;
	gpAppearance.column = pLevel->GetWidth();
	gpAppearance.row = pLevel->GetHeight();
	m_pMapGP->SetAppearance(gpAppearance);

	m_pMapGP->SetSize({
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
			auto cell = std::make_shared<S_Border>(m_pRt2D, infos);
			cell->SetSize({ mapSize, mapSize });
			const auto& ground = pLevel->GetGroundLayerID(x, y);
			if (ground != nullptr)
			{
				FSlateBorderAppearance apperance;
				apperance.Type = EBorderType::Box;
				apperance.color = FColor(0.f, 0.f, 1.f);
				cell->SetAppearance(apperance);
			}

			const auto& eventData = pLevel->GetEventLayerID(x, y);
			if (eventData != nullptr)
			{
				if (eventData->GetEventType() == EEventId::Exit)
				{
					FSlateBorderAppearance apperance;
					apperance.Type = EBorderType::Box;
					apperance.color = FColor(1.f, 0.f, 1.f);
					cell->SetAppearance(apperance);
				}
			}

			const auto& character = pLevel->GetCharacterLayerID(x, y);
			if (character != nullptr)
			{
				if (character->GetCharacterType() == ECharacterId::Player)
				{
					FSlateBorderAppearance apperance;
					apperance.Type = EBorderType::Border;
					apperance.color = FColor(0.f, 1.f, 1.f);
					apperance.roundSize = { 2.5f, 2.5f };
					cell->SetAppearance(apperance);
				}
			}
			m_pMapGP->AddChild(cell);
		}
	}

	m_pRootSlate->UpdateWidget();
}
void HUD::UpdateMap(const Level2D* pLevel)
{
	if (m_pMapGP->GetChildrenCount() == 0)
	{
		ResetMap(pLevel);
	}
	else
	{
		for (int y = 0; y < pLevel->GetHeight(); ++y)
		{
			for (int x = 0; x < pLevel->GetWidth(); ++x)
			{
				auto& slot = m_pMapGP->GetChildAt(x, y);
				auto cell = static_pointer_cast<S_Border>(slot);
				cell->GetAppearance().color = FColor(0, 0, 0);

				const auto& ground = pLevel->GetGroundLayerID(x, y);
				if (ground != nullptr)
				{
					FSlateBorderAppearance apperance;
					apperance.Type = EBorderType::Box;
					apperance.color = FColor(0.f, 0.f, 1.f);
					cell->SetAppearance(apperance);
				}

				const auto& eventData = pLevel->GetEventLayerID(x, y);
				if (eventData != nullptr)
				{
					if (eventData->GetEventType() == EEventId::Exit)
					{
						FSlateBorderAppearance apperance;
						apperance.Type = EBorderType::Box;
						apperance.color = FColor(1.f, 0.f, 1.f);
						cell->SetAppearance(apperance);
					}
				}

				const auto& character = pLevel->GetCharacterLayerID(x, y);
				if (character != nullptr)
				{
					if (character->GetCharacterType() == ECharacterId::Player)
					{
						FSlateBorderAppearance apperance;
						apperance.Type = EBorderType::Border;
						apperance.color = FColor(0.f, 1.f, 1.f);
						apperance.roundSize = { 2.5f, 2.5f };
						cell->SetAppearance(apperance);
					}
				}
			}
		}
	}
}

// --------------------------
// Main : HUDInterface : Effect
// --------------------------
void HUD::Effect_Blackout(float playRate)
{
	if (pEffectBorder == nullptr)
	{
		FSlateBorderAppearance appearance;
		appearance.Type = EBorderType::Box;
		appearance.bIsFill = true;
		appearance.color = FColor(0, 0, 0, 1.f);

		pEffectBorder = std::make_shared<S_Border>(m_pRootSlate->GetSize(), m_pRt2D, FSlateInfos(), appearance);
	}

	WidgetAnimation blackout(0.f, playRate);
	WidgetAnimationProperty propBlack;
	propBlack.Assign(
		pEffectBorder->GetAppearance(), 
		&FSlateBorderAppearance::color, 
		FColor(0, 0, 0, 1.f), 
		FColor(0, 0, 0, 0.f)
	);

	blackout.AssignProp(propBlack);
	blackout.SetCallback<&HUD::Effect_Blckout_Completed>(*this);

	m_pRootSlate->AddChild(pEffectBorder);
	m_pRootSlate->UpdateWidget();

	blackout.Accept();

	AddAnimation(blackout);
}

void HUD::Effect_Blckout_Completed()
{
	m_pRootSlate->RemoveChild(pEffectBorder);
	m_pRootSlate->UpdateWidget();

	pEffectBorder.reset();
	pEffectBorder = nullptr;
}