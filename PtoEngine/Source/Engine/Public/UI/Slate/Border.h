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
	DirectX::XMFLOAT2 roundSize;
};

class S_Border : public SlotContainerOnlyOne
{
public:
	S_Border(DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {}, FSlateBorderAppearance inAppearance = {});
	S_Border(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {}, FSlateBorderAppearance inAppearance = {});

	virtual void Draw() override;
	void SetAppearance(const FSlateBorderAppearance& inAppearance);
	FSlateBorderAppearance& GetAppearance();

protected:
	void SetFileName(std::wstring in);
	virtual void Update() override;

	ID2D1Bitmap* m_pBitmap = nullptr;
	FSlateBorderAppearance appearance;

};