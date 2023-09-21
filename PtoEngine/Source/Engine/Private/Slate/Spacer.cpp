
#include "Slate/Spacer.h"

S_Spacer::S_Spacer(ID2D1HwndRenderTarget* inRt2D, FVector2D inSize, FSlateInfos inSlateInfos)
	: SlateSlotBase(inRt2D, inSize, inSlateInfos)
{
}
S_Spacer::S_Spacer(ID2D1HwndRenderTarget* inRt2D, FSlateInfos inSlateInfos)
	: S_Spacer(inRt2D, { 0,0 }, inSlateInfos)
{
}