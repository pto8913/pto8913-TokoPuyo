
#include "UI/SettingsUI.h"

#include "Slate/CanvasPanel.h"
#include "Slate/VerticalBox.h"
#include "Slate/TextBlock.h"
#include "Slate/Button.h"
#include "Slate/Spacer.h"

#include "EngineSettings.h"
#include "GameSettings.h"

SettingsUI::SettingsUI(Object* inOwner, DirectX11& dx, DX::IMouseInterface* mouse)
	: UserWidget(
		inOwner, 
		dx, 
		mouse,
		EngineSettings::GetWindowSize().x,
		EngineSettings::GetWindowSize().y
	)
{
	const auto windowSize = EngineSettings::GetWindowSize();
	const auto padding = GameSettings::GAMESCREEN_PADDING;

	pRootSlate = std::make_shared<S_CanvasPanel>(windowSize, GetRt2D());
	pRootSlate->SetPosition({ 0, 0 });

	FSlateInfos menuVBInfos;
	menuVBInfos.VAlign = EVerticalAlignment::Center;
	const FVector2D menuVBSize = { windowSize.x / 2, windowSize.y / 2.f };
	auto pMenuVB = std::make_shared<S_VerticalBox>(menuVBSize, GetRt2D(), menuVBInfos);
	pMenuVB->SetPosition({ windowSize.x / 2 - menuVBSize.x / 2, windowSize.y / 2 - menuVBSize.y / 2 + padding.y });
	pRootSlate->AddChild(pMenuVB);

	/* Button */
	auto button = [this, &pMenuVB](const std::wstring& mode)
	{
		FSlateInfos infos;
		infos.HAlign = EHorizontalAlignment::Center;
		infos.padding = { 0.f, 2.5f, 0.f, 2.5f };
		FSlateButtonAppearance appearance;
		auto pButton = std::make_shared<S_Button>(FVector2D(200.f, 40.f), GetRt2D(), infos, appearance);

		/* Label */
		{
			FSlateInfos infos;
			infos.HAlign = EHorizontalAlignment::Fill;
			//infos.VAlign = EVerticalAlignment::Fill;
			infos.padding = { 5.f };
			FSlateFont font;
			font.fontSize = 30.f;
			FSlateTextAppearance appearance;

			auto pTextBlock = std::make_shared<S_TextBlock>(GetRt2D(), infos, font, appearance);
			pTextBlock->SetText(mode);
			pButton->AddChild(pTextBlock);
		}
		pMenuVB->AddChild(pButton);
		return std::move(pButton);
	};
	
	pRootSlate->UpdateWidget();
}

SettingsUI::SettingsUI(DirectX11& dx, DX::IMouseInterface* mouse)
	: SettingsUI(nullptr, dx, mouse)
{

}

SettingsUI::~SettingsUI()
{

}