#pragma once

#include "Bindable.h"

class DirectX11;

class RenderTargetView : public Bindable
{
public:
	//RenderTargetView(DirectX11& dx, UINT width, UINT height);
	RenderTargetView(DirectX11& dx, ID3D11Texture2D* pTexture, std::optional<UINT> face = {});
	virtual ~RenderTargetView();

	ID3D11RenderTargetView*& Get();

	virtual void Bind(DirectX11&) override {};

	void Clear(DirectX11& dx);

private:
	ID3D11RenderTargetView* pRenderTargetView = nullptr;
	//ID3D11Texture2D* pRenderTargetViewBuffer = nullptr;
};