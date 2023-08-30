#pragma once

#include "SlateSlotBase.h"

class S_Image : public SlateSlotBase
{
public:
	S_Image(FVector2D inSize, ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {}, std::wstring inFileName = L"");
	S_Image(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos = {}, std::wstring inFileName = L"");
	virtual ~S_Image();
	
	// ------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------
	virtual void Draw() override;

	void SetFileName(std::wstring in);
private:
	// ------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------
	std::wstring mFileName;
	ID2D1Bitmap* pBitmap = nullptr;
};