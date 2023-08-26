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

	void UpdateTexture(const std::wstring& inFileName);

	virtual void Bind(DirectX11& dx) override;
	void Bind(DirectX::XMVECTOR offset);

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

	ID2D1Bitmap* m_pBitmap = nullptr;

	IDXGIKeyedMutex* keyedMutex11 = nullptr;
	IDXGIKeyedMutex* keyedMutex10 = nullptr;

	ID2D1RenderTarget* D2DRenderTarget = nullptr;
	ID3D11ShaderResourceView* d2dTexture = nullptr;
};
