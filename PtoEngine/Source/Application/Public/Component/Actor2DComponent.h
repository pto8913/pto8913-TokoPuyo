#pragma once

#include "Component/ComponentBase.h"
#include "Framework/Level/Layer.h"
#include "Actor/Actor2DTypes.h"
#include "Math/Vector.h"

class Actor2DComponent : public ComponentBase
{
public:
	Actor2DComponent(Actor* inOwner, const Layer::EOrder& inSortOrder);
	virtual ~Actor2DComponent();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	void SetSortOrder(Layer::EOrder inSortOrder);
	Layer::EOrder GetSortOrder() const;

	const FVector2D& Get2DIdx() const;
	void Set2DIdx(const FVector2D& in);

protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	Layer::EOrder mSortOrder = Layer::UnOrder;

	FVector2D m2DIdx;
};