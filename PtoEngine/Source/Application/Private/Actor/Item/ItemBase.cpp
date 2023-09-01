
#include "Actor/Item/ItemBase.h"

struct FItemSettings : public FActor2DSettings
{
public:
	FItemSettings(const std::wstring& inFileName, const std::wstring& inTag, const FVector2D& inSize)
		: FActor2DSettings(inFileName, inTag, inSize)
	{
	}

	FItemSettings(const std::wstring& inFileName, const std::wstring& inTag)
		: FActor2DSettings(inFileName, inTag)
	{
	}
};

const std::map<EItemId, FItemSettings> ItemList =
{

};

ItemBase::ItemBase(DirectX11& dx, const EItemId& inItemType)
	: Actor2D(
		dx,
		ItemList.at(inItemType)
	),
	itemType(inItemType)
{
	mSortOrder = Layer::Item;
	mLayer = EActor2DLayer::Entities;
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void ItemBase::SetItemType(const EItemId& inItemType)
{
	UpdateTexture(ItemList.at(inItemType).fileName);
	itemType = inItemType;
}
const EItemId& ItemBase::GetItemType() const noexcept
{
	return itemType;
}