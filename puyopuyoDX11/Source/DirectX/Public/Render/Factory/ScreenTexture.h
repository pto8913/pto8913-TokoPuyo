#pragma once

#include "Bindable.h"
#include <dxgi.h>
#include <d2d1.h>
#include "Utils/String.h"

class DirectX11;

class ScreenTexture : public Bindable
{
public:
	ScreenTexture(DirectX11& dx, const std::wstring& inFileName, FLOAT inWidth, FLOAT inHeight);
	~ScreenTexture();

	static std::shared_ptr<ScreenTexture> Make(DirectX11& dx, const std::wstring& inFileName, FLOAT inWidth, FLOAT inHeight);

	virtual void Bind(DirectX11& dx, ID3D11DeviceContext* pContext) override;
	void Bind(DirectX::XMFLOAT2 offset);

	static std::string GenerateID(const std::wstring& inFileName, FLOAT inWidth, FLOAT inHeight)
	{
		inWidth; inHeight;
		using namespace std::string_literals;
		return typeid(ScreenTexture).name() + "#"s + std::string("ScreenTexture") + "#"s + Util::w2String(std::wstring(inFileName));
	}
	std::string GetID() const noexcept
	{
		return GenerateID(fileName, 0, 0);
	}
protected:
	const std::wstring fileName;
	FLOAT width;
	FLOAT height;

	ID2D1Bitmap* m_pBitmap;

	IDXGIKeyedMutex* keyedMutex11;
	IDXGIKeyedMutex* keyedMutex10;

	ID2D1RenderTarget* D2DRenderTarget;
	ID3D11ShaderResourceView* d2dTexture;
};
