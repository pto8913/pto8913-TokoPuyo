
#include "Actor/Item/ItemBase.h"

#include "Component/SpriteComponent.h"

struct FItemSettings : public FActor2DSettings
{
public:
	FItemSettings(const std::wstring& inFileName, const std::wstring& inTag, const FVector2D& inSize)
		: FActor2DSettings(inFileName, inTag, inSize, Layer::Item)
	{
	}

	FItemSettings(const std::wstring& inFileName, const std::wstring& inTag)
		: FActor2DSettings(inFileName, inTag, Layer::Item)
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
	mLayer = Layer::EActorLayer::Entities;
	mSortOrder = Layer::Item;
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void ItemBase::SetItemType(const EItemId& inItemType)
{
	GetSpriteComp()->UpdateTexture(ItemList.at(inItemType).fileName);
	itemType = inItemType;
}
const EItemId& ItemBase::GetItemType() const noexcept
{
	return itemType;
}