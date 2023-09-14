
#include "UI/SettingsUI.h"

#include "Slate/CanvasPanel.h"
#include "Slate/VerticalBox.h"
#include "Slate/HorizontalBox.h"
#include "Slate/TextBlock.h"
#include "Slate/Button.h"
#include "Slate/Spacer.h"
#include "Slate/Slider.h"

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

	{
		FSlateInfos settingsVBInfos;
		settingsVBInfos.VAlign = EVerticalAlignment::Center;
		const FVector2D settingsVBSize = { windowSize.x / 2, windowSize.y / 2.f - 40.f };
		auto pSettingsVB = std::make_shared<S_VerticalBox>(GetRt2D(), settingsVBInfos); //settingsVBSize, GetRt2D(), settingsVBInfos);
		pMenuVB->AddChild(pSettingsVB);

		/* Audio Settings */
		{
			FSlateInfos HBInfos;
			HBInfos.HAlign = EHorizontalAlignment::Center;
			auto pHB = std::make_shared<S_HorizontalBox>(FVector2D(300.f, 40.f), GetRt2D(), HBInfos);

			FSlateInfos textInfos;
			FSlateFont font;
			FSlateTextAppearance textAppearance;
			auto pTextBlock = std::make_shared<S_TextBlock>(FVector2D(75.f, 40.f), GetRt2D(), textInfos, font, textAppearance);
			pTextBlock->SetText(L"音量");

			currentAudioValue = GameSettings::GetAudioVolume();

			FSlateInfos textAudioVolumeInfos;
			FSlateFont fontAudioVolume;
			FSlateTextAppearance textAudioVolumeAppearance;
			pText_AudioVolume = std::make_shared<S_TextBlock>(FVector2D(50.f, 40.f), GetRt2D(), textAudioVolumeInfos, fontAudioVolume, textAudioVolumeAppearance);

			FSlateInfos infos;
			FSlateSliderAppearance appearance;
			appearance.sliderLineThickness = 2.f;
			appearance.step = 0.01f;
			auto pVolumeSlider = std::make_shared<S_Slider>(FVector2D(175.f, 40.f), GetRt2D(), infos, appearance);
			pVolumeSlider->SetMaxValue(100.f);
			pVolumeSlider->OnValueChanged.Bind<&SettingsUI::OnAudioVolumeChanged>(*this, "SettingsUI");
			pVolumeSlider->SetValue(currentAudioValue);

			pSettingsVB->AddChild(pHB);

			pHB->AddChild(pTextBlock);
			pHB->AddChild(pText_AudioVolume);
			pHB->AddChild(pVolumeSlider);
		}
	}
	/* Button */
	auto button = [this](const std::wstring& mode)
	{
		FSlateInfos infos;
		infos.padding = { 2.5f, 0.f, 2.5f, 0.f };
		FSlateButtonAppearance appearance;
		auto pButton = std::make_shared<S_Button>(FVector2D(145.f, 40.f), GetRt2D(), infos, appearance);

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
		return std::move(pButton);
	};

	/* Buttons */
	FSlateInfos buttonHBinfos;
	buttonHBinfos.HAlign = EHorizontalAlignment::Center;
	buttonHBinfos.padding = { 0.f,2.5f,0.f,2.5f };
	auto pButtonsHB = std::make_shared<S_HorizontalBox>(FVector2D(300.f, 40.f), GetRt2D(), buttonHBinfos);
	pMenuVB->AddChild(pButtonsHB);

	/*  Button Save */
	auto pButton_Save = button(L"保存する");
	pButton_Save->OnClicked.Bind<&SettingsUI::OnClickedSave>(*this, "SettingsUI");

	/* Button Return */
	auto pButton_Return = button(L"戻る");
	pButton_Return->OnClicked.Bind<&SettingsUI::OnClickedReturnTitle>(*this, "SettingsUI");

	pButtonsHB->AddChild(pButton_Save);
	pButtonsHB->AddChild(pButton_Return);

	pRootSlate->UpdateWidget();
}
SettingsUI::SettingsUI(DirectX11& dx, DX::IMouseInterface* mouse)
	: SettingsUI(nullptr, dx, mouse)
{

}

SettingsUI::~SettingsUI()
{
	OnClickedReturnToTitle.ClearBind();

	pText_AudioVolume.reset();
	pText_AudioVolume = nullptr;
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void SettingsUI::OnClickedReturnTitle(DX::MouseEvent inMouseEvent)
{
	OnClickedReturnToTitle.Broadcast(inMouseEvent);
}
void SettingsUI::OnClickedSave(DX::MouseEvent inMouseEvent)
{
	GameSettings::SetAudioVolume(currentAudioValue);
}
void SettingsUI::OnAudioVolumeChanged(float inValue)
{
	currentAudioValue = inValue;
	pText_AudioVolume->SetText(std::to_wstring((int)inValue));
}