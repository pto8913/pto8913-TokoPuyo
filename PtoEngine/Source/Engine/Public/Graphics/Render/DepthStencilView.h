#pragma once

#include "Bindable.h"

class DirectX11;

class DepthStencilView : public Bindable
{
public:
	DepthStencilView(DirectX11& dx, UINT width, UINT height);
	DepthStencilView(DirectX11& dx, ID3D11Texture2D* pTexture, UINT face);
	virtual ~DepthStencilView();

	ID3D11DepthStencilView*& Get();
	virtual void Bind(DirectX11&) override {};

	void Clear(DirectX11& dx);
private:
	ID3D11DepthStencilView* pDepthStencilView = nullptr;
	ID3D11Texture2D* pDepthStencilViewBuffer = nullptr;
};