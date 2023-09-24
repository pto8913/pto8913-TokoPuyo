#pragma once

#include "Graphics/Render/Bindable.h"

class DirectX11;

class ViewPort : public Bindable
{
public:
	ViewPort(float width, float height);
	virtual void Bind(DirectX11& dx) override;
private:
	D3D11_VIEWPORT viewport;
};