
#include "Object/Event/Event_DungeonExit.h"

Event_DungeonExit::Event_DungeonExit(DirectX11& dx)
	: EventChoiceBase(dx, EEventId::Exit)
{
	ChoiceInfos.Add(FChoiceInfos(L"降りる", L"Yes"));
	ChoiceInfos.Add(FChoiceInfos(L"やめる", L"Cancel"));
}
Event_DungeonExit::~Event_DungeonExit()
{
	OnChoiceYes.ClearBind();
	OnChoiceNo.ClearBind();
}

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