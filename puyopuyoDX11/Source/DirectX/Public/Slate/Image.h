#pragma once

#include "SlateSlotBase.h"

struct FSlateImageAppearance
{
	enum class EImageType
	{
		Image, /* like a .png */
		Border, /* only color */
	};
};

class S_Image : public SlateSlotBase
{
public:
	S_Image(DirectX11& dx, DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {}, std::wstring inFileName = L"");
	S_Image(DirectX11& dx, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {}, std::wstring inFileName = L"")
		: S_Image(dx, {0,0}, inD2DRT, inSlateInfos, inFileName)
	{}
	
	virtual void Draw() override;

	void SetFileName(std::wstring in);
private:
	std::wstring m_FileName;
	ID2D1Bitmap* m_pBitmap = nullptr;
};