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
	S_Image(DirectX::XMFLOAT2 inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {}, std::wstring inFileName = L"");
	S_Image(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {}, std::wstring inFileName = L"")
		: S_Image({0,0}, inD2DRT, inSlateInfos, inFileName)
	{}
	virtual ~S_Image();

	virtual void Draw() override;

	void SetFileName(std::wstring in);
private:
	std::wstring m_FileName;
	ID2D1Bitmap* m_pBitmap = nullptr;
};