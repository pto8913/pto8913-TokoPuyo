#pragma once

#include "Bindable.h"

class RenderTargetView;
class DepthStencilView;
class DirectX11;

class CubeTexture : public Bindable
{
public:
	CubeTexture(DirectX11& dx, const std::wstring& path, UINT newSlot);
	virtual ~CubeTexture();

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
	ID3D11ShaderResourceView* pTextureView = nullptr;
};