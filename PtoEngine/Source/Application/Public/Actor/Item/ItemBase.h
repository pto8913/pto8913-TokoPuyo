#pragma once

#include "Actor/Actor2D.h"

#include "ItemTypes.h"

class ItemBase : public Actor2D
{
public:
	ItemBase(DirectX11& dx, const EItemId& inItemType);

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return typeid(ItemBase).name() + std::string(" : ") + "#"s + std::to_string(mID);
	}

	void SetItemType(const EItemId& inItemType);
	const EItemId& GetItemType() const noexcept;

private:
	EItemId itemType;
};