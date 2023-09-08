
#include "UI/TitleUI.h"

#include "Slate/CanvasPanel.h"
#include "Slate/VerticalBox.h"
#include "Slate/TextBlock.h"

#include "EngineSettings.h"

TitleUI::TitleUI(std::shared_ptr<Object> inOwner, DirectX11& dx, DX::IMouseInterface* mouse)
	: UserWidget(
		inOwner, 
		dx, 
		mouse,
		EngineSettings::GetWindowSize().x,
		EngineSettings::GetWindowSize().y
	)
{
	pRootSlate = std::make_shared<S_CanvasPanel>(EngineSettings::GetWindowSize(), GetRt2D());

	//auto pMenuVB = MakeSlate<S_VerticalBox>(FVector2D(), GetRt2D(), );

	//auto pTextBlock = std::make_shared<S_TextBlock>(GetRt2D());
}

TitleUI::TitleUI(DirectX11& dx, DX::IMouseInterface* mouse)
	: TitleUI(nullptr, dx, mouse)
{
}

TitleUI::~TitleUI()
{

}
