#pragma once

#include "Render/Bindable.h"

class DirectX11;

class RenderTargetView : public Bindable
{
public:
	//RenderTargetView(DirectX11& dx, UINT width, UINT height);
	RenderTargetView(DirectX11& dx, ID3D11Texture2D* pTexture, std::optional<UINT> face = {});
	~RenderTargetView();

	ID3D11RenderTargetView*& Get();

	virtual void Bind(DirectX11&) override {};

	void Clear(DirectX11& dx);

private:
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11Texture2D* m_pRenderTargetViewBuffer;
};