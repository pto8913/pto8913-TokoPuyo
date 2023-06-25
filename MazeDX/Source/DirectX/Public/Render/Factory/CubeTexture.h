#pragma once

#include "Bindable.h"

class RenderTargetView;
class DepthStencilView;
class DirectX11;

class CubeTexture : public Bindable
{
public:
	CubeTexture(DirectX11& dx, const std::string& path, UINT newSlot);
	~CubeTexture();

	virtual void Bind(DirectX11& dx) override;
private:
	UINT slot;
protected:
	ID3D11ShaderResourceView* m_pTextureView;
};

class CubeTargetTexture : public Bindable
{
public:
	CubeTargetTexture(DirectX11& dx, UINT width, UINT height, UINT slot = 0, DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM);

	std::shared_ptr<RenderTargetView> Get(size_t index) const;
private:
	UINT slot;
protected:
	ID3D11ShaderResourceView* m_pTextureView;
	std::vector<std::shared_ptr<RenderTargetView>> renderTargets;
};

class CubeDepthTexture : public Bindable
{
public:
	CubeDepthTexture(DirectX11& dx, UINT size, UINT inSlot = 0);

	std::shared_ptr<DepthStencilView> Get(size_t index) const;
private:
	UINT slot;
protected:
	ID3D11ShaderResourceView* m_pTextureView;
	std::vector<std::shared_ptr<DepthStencilView>> depthBuffers;
};