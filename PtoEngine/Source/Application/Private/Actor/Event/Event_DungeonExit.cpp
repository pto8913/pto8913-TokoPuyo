
#include "Actor/Event/Event_DungeonExit.h"

Event_DungeonExit::Event_DungeonExit(DirectX11& dx)
	: EventChoiceBase(dx, EEventId::Exit)
{
	ChoiceInfos.Add(FChoiceInfos(L"ç~ÇËÇÈ", L"Yes"));
	ChoiceInfos.Add(FChoiceInfos(L"Ç‚ÇﬂÇÈ", L"Cancel"));
}
Event_DungeonExit::~Event_DungeonExit()
{
	OnChoiceYes.ClearBind();
	OnChoiceNo.ClearBind();
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void Event_DungeonExit::OnChoiceResult(const FChoiceInfos& inChoiceInfos)
{
	if (inChoiceInfos.tag == L"Yes")
	{
		OutputDebugStringA("Dungeon Exit Yes");
		OnChoiceYes.Broadcast();
	}
	else if (inChoiceInfos.tag == L"Cancel")
	{
		OutputDebugStringA("Dungeon Exit No");
		CloseChoiceUI();
		OnChoiceNo.Broadcast();
	}
}