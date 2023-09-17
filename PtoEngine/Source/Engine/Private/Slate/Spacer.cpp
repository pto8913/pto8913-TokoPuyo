
#include "Slate/Spacer.h"

S_Spacer::S_Spacer(ID2D1RenderTarget* inD2DRT, FVector2D inSize, FSlateInfos inSlateInfos)
	: SlateSlotBase(inD2DRT, inSize, inSlateInfos)
{
}
S_Spacer::S_Spacer(ID2D1RenderTarget* inD2DRT, FSlateInfos inSlateInfos)
	: S_Spacer(inD2DRT, { 0,0 }, inSlateInfos)
{
}