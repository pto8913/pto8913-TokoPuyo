#pragma once

#include "Object/Actor2D.h"

#include "ItemTypes.h"

class ItemBase : public Actor2D
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