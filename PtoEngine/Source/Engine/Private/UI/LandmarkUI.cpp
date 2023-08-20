
#include "UI/LandmarkUI.h"
#include "UI/Slate/Border.h"
#include "UI/Slate/TextBlock.h"
#include "UI/Slate/CanvasPanel.h"

#include "Core/AppSettings.h"
#include "Core/DirectX.h"

#include "Engine/World.h"
#include "Engine/Vector.h"

using namespace DirectX;

LandmarkUI::LandmarkUI(std::shared_ptr<Object> inOwner, DirectX11& dx, const std::wstring& inLandmarkName, const float& inPlayRate, const FOnWidgetAnimationCompleted& inCompleted)
	: UserWidget(inOwner, dx, nullptr, (int)AppSettings::windowSize.x, (int)AppSettings::windowSize.y),
	mLandmarkName(inLandmarkName), 
	mPlayRate(inPlayRate),
	mDelegate(inCompleted)
{
	pRootSlate = std::make_shared<S_CanvasPanel>(AppSettings::windowSize, m_pRt2D);

	/* Border */
	{
		FSlateBorderAppearance borderAppearance;
		borderAppearance.Type = EBorderType::Box;
		borderAppearance.bIsFill = true;
		borderAppearance.color = FColor(0.f, 0.f, 0.f, 1.f);
		pEffectBorder = std::make_shared<S_Border>(AppSettings::windowSize, m_pRt2D, FSlateInfos(), borderAppearance);

		FSlateInfos textInfos;
		textInfos.HAlign = EHorizontalAlignment::Center;
		textInfos.VAlign = EVerticalAlignment::Center;

		FSlateFont textFont;
		textFont.fontSize = 50.f;

		FSlateTextAppearance textAppearance;
		textAppearance.color = FColor(1.f, 1.f, 1.f);
		pText = std::make_shared<S_TextBlock>(m_pRt2D, textInfos, textFont, textAppearance);
		pText->SetText(mLandmarkName);

		pRootSlate->AddChild(pEffectBorder);
		pEffectBorder->AddChild(pText);

		/* Animation */
		GetWorld()->GetTimerManager().SetTimer<&LandmarkUI::AnimationStart>(*this, 0.f, false, 1.f);
	}

	pRootSlate->UpdateWidget();
}
LandmarkUI::LandmarkUI(DirectX11& dx, const std::wstring& inLandmarkName, const float& inPlayRate, const FOnWidgetAnimationCompleted& inCompleted)
	: LandmarkUI(nullptr, dx, inLandmarkName, inPlayRate, inCompleted)
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
void LandmarkUI::AnimationStart()
{
	WidgetAnimation blackout(0.f, mPlayRate);
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
		blackout.OnWidgetAnimationCompleted.Append(mDelegate, "external");
	}
	blackout.Activate();
	AddAnimation(blackout);
}
void LandmarkUI::Completed()
{

}