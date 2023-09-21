
#include "Slate/SlateSlotBase.h"

SlateSlotBase::SlateSlotBase(ID2D1HwndRenderTarget* inRt2D, FVector2D inSize, FSlateInfos inSlateInfos)
	: SlateBase(inRt2D, inSize, inSlateInfos)
{
}
SlateSlotBase::SlateSlotBase(ID2D1HwndRenderTarget* inRt2D, FSlateInfos inSlateInfos)
	: SlateBase(inRt2D, { 0,0 }, inSlateInfos)
{
}