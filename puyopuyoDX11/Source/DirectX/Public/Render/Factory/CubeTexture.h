#pragma once

#include "Bindable.h"

class RenderTargetView;
class DepthStencilView;
class DirectX11;

class CubeTexture : public Bindable
{
public:
	CubeTexture(DirectX11& dx, const std::wstring& path, UINT newSlot);
	~CubeTexture();

	static std::shared_ptr<CubeTexture> Make(DirectX11& dx, const std::wstring& inFileName, UINT inSlot = 0);

	virtual void Bind(DirectX11& dx) override;

	static std::string GenerateID(const std::wstring& inFileName, UINT slot = 0)
	{
		inFileName;
		using namespace std::string_literals;
		return typeid(CubeTexture).name() + "#"s + std::to_string(slot);
	}
	std::string GetID() const noexcept
	{
		return GenerateID(fileName, slot);
	}
private:
	UINT slot;
	std::wstring fileName;
protected:
	ID3D11ShaderResourceView* m_pTextureView;
};
//
//class CubeTargetTexture : public Bindable
//{
//public:
//	CubeTargetTexture(DirectX11& dx, UINT width, UINT height, UINT slot = 0, DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM);
//
//	std::shared_ptr<RenderTargetView> Get(size_t index) const;
//private:
//	UINT slot;
//protected:
//	ID3D11ShaderResourceView* m_pTextureView;
//	std::vector<std::shared_ptr<RenderTargetView>> renderTargets;
//};
//
//class CubeDepthTexture : public Bindable
//{
//public:
//	CubeDepthTexture(DirectX11& dx, UINT size, UINT inSlot = 0);
//
//	std::shared_ptr<DepthStencilView> Get(size_t index) const;
//private:
//	UINT slot;
//protected:
//	ID3D11ShaderResourceView* m_pTextureView;
//	std::vector<std::shared_ptr<DepthStencilView>> depthBuffers;
//};