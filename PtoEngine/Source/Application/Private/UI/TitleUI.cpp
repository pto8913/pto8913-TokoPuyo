
#include "UI/TitleUI.h"

#include "Slate/CanvasPanel.h"
#include "Slate/VerticalBox.h"
#include "Slate/TextBlock.h"

#include "EngineSettings.h"
#include "GameSettings.h"

TitleUI::TitleUI(std::shared_ptr<Object> inOwner, DirectX11& dx, DX::IMouseInterface* mouse)
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

	const FVector2D menuVBSize = { 400, windowSize.y };
	auto pMenuVB = MakeSlate<S_VerticalBox>(menuVBSize, GetRt2D());
	pMenuVB->SetPosition({ windowSize.x / 2 - menuVBSize.x / 2, padding.y });
	pRootSlate->AddChild(pMenuVB);

	/* Title */
	{
		FSlateInfos infos;
		infos.HAlign = EHorizontalAlignment::Fill;
		infos.VAlign = EVerticalAlignment::Fill;
		FSlateFont font;
		font.fontSize = 40.f;
		FSlateTextAppearance appearance;

		auto pTextBlock = std::make_shared<S_TextBlock>(GetRt2D(), infos, font, appearance);
		pTextBlock->SetText(L"pto8193 ‚Õ‚æ‚Õ‚æ");
		pMenuVB->AddChild(pTextBlock);
	}

	pRootSlate->UpdateWidget();
}

TitleUI::TitleUI(DirectX11& dx, DX::IMouseInterface* mouse)
	: TitleUI(nullptr, dx, mouse)
{
}

TitleUI::~TitleUI()
{

}
