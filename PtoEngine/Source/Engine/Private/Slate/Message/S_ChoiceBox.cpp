
#include "Slate/Message/S_ChoiceBox.h"
#include "Slate/Message/S_ChoiceButton.h"

#include <format>

S_ChoiceBox::S_ChoiceBox(ID2D1RenderTarget* inRt2D, DirectX11& dx, FVector2D inSize, const TArray<FChoiceInfos>& inChoiceInfos, FSlateInfos inSlateInfos)
	: S_VerticalBox(inRt2D, inSize, inSlateInfos)
{
	pDX = &dx;
	SetChoiceButton(inChoiceInfos);
}
S_ChoiceBox::S_ChoiceBox(ID2D1RenderTarget* inRt2D, DirectX11& dx, const TArray<FChoiceInfos>& inChoiceInfos, FSlateInfos inSlateInfos)
	: S_ChoiceBox(inRt2D, dx, { 0,0 }, inChoiceInfos, inSlateInfos)
{
}

S_ChoiceBox::~S_ChoiceBox()
{
	ClearChildren();
	OnChoiceResult.ClearBind();
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
FOnChoceResult& S_ChoiceBox::GetOnChoiceResult()
{
	return OnChoiceResult;
}

// --------------------------
// Main : Button
// --------------------------
void S_ChoiceBox::SetChoiceButton(const TArray<FChoiceInfos>& inChoiceInfos)
{
	int numOfButton = (int)inChoiceInfos.Size();
	//static_assert(numOfButton > 0, "ChoiceBox Error : ChoiceInfos must greater than 0.");
	if (numOfButton > 0)
	{
		int currNumOfButton = (int)pChildren.size();
		if (numOfButton > currNumOfButton)
		{
			FSlateInfos info;
			info.padding.top = 2.5f;
			info.padding.bottom = 2.5f;

			FSlateButtonAppearance appearance;
			appearance.HoverColor = FColor(0.f, 1.f, 0.75f, 1.f);
			appearance.PressColor = FColor(0.f, 0.5f, 0.4f, 1.f);

			for (int i = 0; i < numOfButton; ++i)
			{
				auto button = std::make_shared<S_ChoiceButton>(pRt2D, *pDX, FVector2D(GetSize().x, 30.f), FChoiceInfos(), info, appearance);
				AddChild(button);
				button->GetOnChoice().Bind<&S_ChoiceBox::OnClicked>(*this, std::format("choice_button_{}", GetChildrenCount()));
			}
		}
		else if (numOfButton < currNumOfButton)
		{
			while (numOfButton < currNumOfButton)
			{
				const auto& button = static_cast<S_ChoiceButton*>(GetChildAt(currNumOfButton - 1));
				button->GetOnChoice().Unbind(std::format("choice_button_{}", currNumOfButton - 1));
				RemoveChild(currNumOfButton - 1);
				--currNumOfButton;
			}
		}

		SetSize(FVector2D(200.f, numOfButton * 35.f));

		for (int i = 0; i < numOfButton; ++i)
		{
			const auto& button = static_cast<S_ChoiceButton*>(GetChildAt(i));;
			if (button != nullptr)
			{
				button->SetChoiceInfos(inChoiceInfos[i]);
			}
		}
		UpdateWidget();
	}
}

void S_ChoiceBox::OnClicked(DX::MouseEvent inMouseEvent, const FChoiceInfos& inChoiceInfos)
{
	OnChoiceResult.Broadcast(inChoiceInfos);
}
