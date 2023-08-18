#pragma once

#include "Object/LayerObjectBase.h"

#include "ItemTypes.h"

class ItemBase : public LayerObject2DBase
{
public:
	ItemBase(DirectX11& dx, const EItemId& inItemType);

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	void SetItemType(const EItemId& inItemType);
	const EItemId& GetItemType() const noexcept;

private:
	EItemId itemType;
};