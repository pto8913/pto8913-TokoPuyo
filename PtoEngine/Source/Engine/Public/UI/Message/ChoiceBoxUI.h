#pragma once

#include "UI/UserWidget.h"
#include "ChoiceTypes.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnChoceResult, const FChoiceInfos&);

class ChoiceBoxUI : public UserWidget
{
public:
	ChoiceBoxUI(Object* inOwner, ID2D1RenderTarget* inRt2D, DirectX11& dx, DX::IMouseInterface* mouse, const std::vector<FChoiceInfos>& inChoiceInfos);
	virtual ~ChoiceBoxUI();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void NativeOnInitialized() override;
	virtual std::string GetName() const override
	{
		return "ChoiceBoxUI";
	}
	FOnChoceResult& GetOnChoiceResult();

	// --------------------------
	// Main : Button
	// --------------------------
	void SetChoiceButton(const std::vector<FChoiceInfos>& inChoiceInfos);

protected:
	void OnClicked(DX::MouseEvent inMouseEvent, const FChoiceInfos& inChoiceInfos);

	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	FOnChoceResult OnChoiceResult;
	DirectX11* pDX = nullptr;
	DX::IMouseInterface* pMouse = nullptr;

};