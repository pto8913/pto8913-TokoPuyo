
#include "Helper/LayerHelper.h"

int LayerHelper::ConvertTypeToLayer(const EGroundId& in)
{
	return GetGroundId();
}
int LayerHelper::ConvertTypeToLayer(const EEventId& in)
{
	return GetEventId();
}
int LayerHelper::ConvertTypeToLayer(const EItemId& in)
{
	return GetItemId();
}
int LayerHelper::ConvertTypeToLayer(const EBuildingId& in)
{
	return GetBuildingId();
}
int LayerHelper::ConvertTypeToLayer(const ECharacterId& in)
{
	return GetCharacterId();
}

bool LayerHelper::IsGroundId(int inLayer)
{
	return inLayer == GetGroundId();
}
bool LayerHelper::IsEventId(int inLayer)
{
	return inLayer == GetEventId();
}
bool LayerHelper::IsItemId(int inLayer)
{
	return inLayer == GetItemId();
}
bool LayerHelper::IsBuildingId(int inLayer)
{
	return inLayer == GetBuildingId();
}
bool LayerHelper::IsCharacterId(int inLayer)
{
	return inLayer == GetCharacterId();
}

int LayerHelper::GetGroundId()
{
	return 0;
}
int LayerHelper::GetEventId()
{
	return 1;
}
int LayerHelper::GetItemId()
{
	return 2;
}
int LayerHelper::GetBuildingId()
{
	return 3;
}
int LayerHelper::GetCharacterId()
{
	return 4;
}