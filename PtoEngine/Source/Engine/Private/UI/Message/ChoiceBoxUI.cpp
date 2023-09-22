
#include "UI/Message/ChoiceBoxUI.h"
#include "UI/Message/ChoiceButtonUI.h"

#include "Slate/VerticalBox.h"

#include "Framework/World.h"

#include <format>

ChoiceBoxUI::ChoiceBoxUI(Object* inOwner, ID2D1RenderTarget* inRt2D, DirectX11& dx, DX::IMouseInterface* mouse, const std::vector<FChoiceInfos>& inChoiceInfos)
	: UserWidget(inOwner, inRt2D, dx, mouse),
	pMouse(mouse),
	pDX(&dx)
{
	SetChoiceButton(inChoiceInfos);
}
ChoiceBoxUI::~ChoiceBoxUI()
{
	OnChoiceResult.ClearBind();

	pDX = nullptr;
	pMouse = nullptr;
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void ChoiceBoxUI::NativeOnInitialized()
{
	pRootSlate = MakeSlate<S_VerticalBox>();

	pRootSlate->UpdateWidget();
	UserWidget::NativeOnInitialized();
}
FOnChoceResult& ChoiceBoxUI::GetOnChoiceResult()
{
	return OnChoiceResult;
}

// --------------------------
// Main : Button
// --------------------------
void ChoiceBoxUI::SetChoiceButton(const std::vector<FChoiceInfos>& inChoiceInfos)
{
	int numOfButton = (int)inChoiceInfos.size();
	//static_assert(numOfButton > 0, "ChoiceBox Error : ChoiceInfos must greater than 0.");
	if (numOfButton > 0)
	{
		int currNumOfButton = (int)pRootSlate->GetChildrenCount();
		if (numOfButton > currNumOfButton)
		{
			for (int i = 0; i < numOfButton; ++i)
			{
				auto button = GetWorld()->CreateWidget<ChoiceButtonUI>(this, *pDX, pMouse);
				AddWidget(button);
				button->GetOnChoice().Bind<&ChoiceBoxUI::OnClicked>(*this, std::format("choice_button_{}", currNumOfButton));
			}
		}
		else if (numOfButton < currNumOfButton)
		{
			while (numOfButton < currNumOfButton)
			{
				const auto& button = static_cast<ChoiceButtonUI*>(pChildWidgets.at(currNumOfButton - 1));
				button->GetOnChoice().Unbind(std::format("choice_button_{}", currNumOfButton - 1));
				RemoveWidget(currNumOfButton - 1);
				--currNumOfButton;
			}
		}

		pRootSlate->SetSize(FVector2D(200.f, numOfButton * 35.f));

		for (int i = 0; i < numOfButton; ++i)
		{
			const auto& button = static_cast<ChoiceButtonUI*>(pChildWidgets.at(i));;
			if (button != nullptr)
			{
				button->SetChoiceInfos(inChoiceInfos[i]);
			}
		}
		pRootSlate->UpdateWidget();
	}
}

void ChoiceBoxUI::OnClicked(DX::MouseEvent inMouseEvent, const FChoiceInfos& inChoiceInfos)
{
	OnChoiceResult.Broadcast(inChoiceInfos);
}
