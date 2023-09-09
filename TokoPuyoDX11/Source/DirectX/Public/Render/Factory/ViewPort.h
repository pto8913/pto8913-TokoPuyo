#pragma once

#include "Render/Factory/Bindable.h"

class DirectX11;

class ViewPort : public Bindable
{
public:
	ViewPort(float width, float height);
	virtual ~ViewPort();

	virtual void Bind(DirectX11& dx) override;
private:
	D3D11_VIEWPORT viewport;
};