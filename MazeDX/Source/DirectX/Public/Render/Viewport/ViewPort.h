#pragma once

#include "DirectX/DirectXHead.h"

#include "Render/Factory/Bindable.h"

class DirectX11;

class ViewPort : public Bindable
{
public:
	ViewPort(DirectX11& dx, float width, float height);
	virtual void Bind(DirectX11& dx) override;
private:
	D3D11_VIEWPORT viewport;
};