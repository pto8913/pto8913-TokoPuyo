#pragma once

#include "Render/Factory/Bindable.h"

class DirectX11;

class ViewPort : public Bindable
{
public:
	ViewPort(float width, float height);
	virtual void Bind(DirectX11& dx, ID3D11DeviceContext* pContext) override;
private:
	D3D11_VIEWPORT viewport;
};