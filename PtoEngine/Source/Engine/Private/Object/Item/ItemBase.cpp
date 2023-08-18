
#include "Object/Item/ItemBase.h"

struct FItemSettings : public FLayerObject2DSettings
{
public:
	FItemSettings(const std::wstring& inFileName, const std::wstring& inTag, const DirectX::XMFLOAT2& inSize)
		: FLayerObject2DSettings(inFileName, inTag, inSize)
	{
	}

	FItemSettings(const std::wstring& inFileName, const std::wstring& inTag)
		: FLayerObject2DSettings(inFileName, inTag)
	{
	}
};

const std::map<EItemId, FItemSettings> ItemList =
{

};

ItemBase::ItemBase(DirectX11& dx, const EItemId& inItemType)
	: LayerObject2DBase(
		dx,
		ItemList.at(inItemType),
		3
	),
	itemType(inItemType)
{
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