
#include "UI/LandmarkUI.h"
#include "Slate/Border.h"
#include "Slate/TextBlock.h"
#include "Slate/CanvasPanel.h"

#include "EngineSettings.h"
#include "Core/DirectX.h"

#include "Framework/World.h"
#include "Math/Vector.h"

using namespace DirectX;

LandmarkUI::LandmarkUI(Object* inOwner, ID2D1RenderTarget* inRt2D, DirectX11& dx, DX::IMouseInterface* mouse, const std::wstring& inLandmarkName, const float& inPlayRate, const FOnWidgetAnimationCompleted& inCompleted)
	: UserWidget(inOwner, inRt2D, dx, mouse),
	mLandmarkName(inLandmarkName), 
	mPlayRate(inPlayRate),
	mDelegate(inCompleted)
{
}
LandmarkUI::~LandmarkUI()
{
	Anim_blackout.Deactivate();
	Anim_blackout.Clear();

	propColor.Clear();

	mDelegate.ClearBind();

	pEffectBorder.reset();
	pEffectBorder = nullptr;

	pText.reset();
	pText = nullptr;
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void LandmarkUI::NativeOnInitialized()
{
	pRootSlate = MakeSlate<S_CanvasPanel>(EngineSettings::GetWindowSize());

	/* Border */
	{
		FSlateBorderAppearance borderAppearance;
		borderAppearance.Type = EBorderType::Box;
		borderAppearance.bIsFill = true;
		borderAppearance.color = FColor(0.f, 0.f, 0.f, 1.f);
		pEffectBorder = MakeSlate<S_Border>(EngineSettings::GetWindowSize(), FSlateInfos(), borderAppearance);

		FSlateInfos textInfos;
		textInfos.HAlign = EHorizontalAlignment::Center;
		textInfos.VAlign = EVerticalAlignment::Center;

		FSlateFont textFont;
		textFont.fontSize = 50.f;

		FSlateTextAppearance textAppearance;
		textAppearance.color = FColor(1.f, 1.f, 1.f);
		pText = MakeSlate<S_TextBlock>(textInfos, textFont, textAppearance);
		pText->SetText(mLandmarkName);

		pRootSlate->AddChild(pEffectBorder);
		pEffectBorder->AddChild(pText);

		/* Animation */
		GetWorld()->GetTimerManager().SetTimer<&LandmarkUI::AnimationStart>(*this, 0.f, false, 1.f);
	}

	pRootSlate->UpdateWidget();
	UserWidget::NativeOnInitialized();
}
void LandmarkUI::AnimationStart()
{
	WidgetAnimation blackout;
	propColor.Clear();
	propColor.Assign(
		pText->GetAppearance(),
		&FSlateTextAppearance::color,
		FColor(1.f, 1.f, 1.f),
		FColor(0.f, 0.f, 0.f)
	);

	blackout.AssignProp(&propColor);
	blackout.OnWidgetAnimationCompleted.Bind<&LandmarkUI::Completed>(*this, "landmark");
	if (mDelegate.IsBound())
	{
		blackout.OnWidgetAnimationCompleted.Append(mDelegate);
	}
	AddAnimation(blackout);
	blackout.PlayForward(0.f, mPlayRate);
}
void LandmarkUI::Completed()
{

}