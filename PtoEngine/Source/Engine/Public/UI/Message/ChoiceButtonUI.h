#pragma once

#include "UI/UserWidget.h"
#include "ChoiceTypes.h"

class S_TextBlock;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChoice, DX::MouseEvent, const FChoiceInfos&);

class ChoiceButtonUI : public UserWidget
{
public:
	ChoiceButtonUI(Object* inOwner, ID2D1RenderTarget* inRt2D, DirectX11& dx, DX::IMouseInterface* mouse, const FChoiceInfos& inChoiceInfos = {});
	virtual ~ChoiceButtonUI();


	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	virtual void NativeOnInitialized() override;

	virtual std::string GetName() const override
	{
		return "ChoiceButtonUI";
	}
	FOnChoice& GetOnChoice();

	// --------------------------
	// Main : Button
	// --------------------------
	void SetChoiceInfos(const FChoiceInfos& inChoiceInfos);

protected:
	void OnClickedEvent(DX::MouseEvent eventData);

	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	std::shared_ptr<S_TextBlock> pLabel = nullptr;

	FOnChoice OnChoice;
	FChoiceInfos mChoiceInfos;
};