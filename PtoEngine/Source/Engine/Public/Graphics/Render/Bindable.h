#pragma once

#include "Graphics/Graphics/Core/DirectXResource.h"

#include <optional>

class DirectX11;
class DrawableObject;

class Bindable : public DirectXResource
{
public:
	virtual ~Bindable() = default;

	virtual void Bind(DirectX11& dx) = 0;
	virtual void InitParentRefrence(const DrawableObject& pParent) { pParent; };
};