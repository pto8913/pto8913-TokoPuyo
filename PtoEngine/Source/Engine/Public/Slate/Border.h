#pragma once

#include "SlateContainerBase.h"
#include "Engine/Color.h"

enum class EBorderType
{
	Image,
	Border,
	Box,
	None,
};

struct FSlateBorderAppearance
{
public:
	FSlateBorderAppearance()
		: Type(EBorderType::Border),
		fileName(L""),
		color(0.f, 1.f),
		bIsFill(true),
		roundSize(5.f, 5.f)
	{
	}

	EBorderType Type;

	// ------------------------------------------------------
	// Type : Image
	// ------------------------------------------------------
	std::wstring fileName;

	// ------------------------------------------------------
	// Type : Border, Box
	// ------------------------------------------------------
	FColor color;
	bool bIsFill = false;

	// --------------------------
	// Not Fill
	// --------------------------
	float lineWidth = 1.f;

	// ------------------------------------------------------
	// Type : Border
	// ------------------------------------------------------
	FVector2D roundSize;
};

class S_Border : public SlotContainerOnlyOne
{
public:
	S_Border(ID2D1RenderTarget* inD2DRT, FVector2D inSize, FSlateInfos inSlateInfos = {}, FSlateBorderAppearance inAppearance = {});
	S_Border(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {}, FSlateBorderAppearance inAppearance = {});
	virtual ~S_Border();

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	virtual std::string GetName() const override
	{
		if (pParent != nullptr)
		{
			return pParent->GetName() + "_S_Border";
		}
		return "S_Border";
	}
	virtual void Draw() override;

	void SetAppearance(const FSlateBorderAppearance& inAppearance);
	FSlateBorderAppearance& GetAppearance();

protected:
	void SetFileName(std::wstring in);
	virtual void Update() override;

	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------

	ID2D1Bitmap* pBitmap = nullptr;
	FSlateBorderAppearance mAppearance;

};