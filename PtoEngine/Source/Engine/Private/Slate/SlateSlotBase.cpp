
#include "Slate/SlateSlotBase.h"

SlateSlotBase::SlateSlotBase(ID2D1RenderTarget* inD2DRT, FVector2D inSize, FSlateInfos inSlateInfos)
	: SlateBase(inD2DRT, inSize, inSlateInfos)
{
}
SlateSlotBase::SlateSlotBase(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos)
	: SlateBase(inD2DRT, { 0,0 }, inSlateInfos)
{
}