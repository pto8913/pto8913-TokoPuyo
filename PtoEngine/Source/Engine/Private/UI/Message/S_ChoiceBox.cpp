
#include "UI/Message/S_ChoiceBox.h"
#include "UI/Message/S_ChoiceButton.h"

#include <format>

S_ChoiceBox::S_ChoiceBox(FVector2D inSize, ID2D1RenderTarget* inD2DRT, const TArray<FChoiceInfos>& inChoiceInfos, FSlateInfos inSlateInfos)
	: S_VerticalBox(inSize, inD2DRT, inSlateInfos)
{
	SetChoiceButton(inChoiceInfos);
}
S_ChoiceBox::S_ChoiceBox(ID2D1RenderTarget* inD2DRT, const TArray<FChoiceInfos>& inChoiceInfos, FSlateInfos inSlateInfos)
	: S_ChoiceBox({0,0}, inD2DRT, inChoiceInfos, inSlateInfos)
{
}

S_ChoiceBox::~S_ChoiceBox()
{
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
	int numOfButton = inChoiceInfos.Size();
	//static_assert(numOfButton > 0, "ChoiceBox Error : ChoiceInfos must greater than 0.");
	if (numOfButton > 0)
	{
		int currNumOfButton = pChildren.size();
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
				auto button = std::make_shared<S_ChoiceButton>(FVector2D(GetSize().x, 30.f), pD2DRT, FChoiceInfos(), info, appearance);
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
