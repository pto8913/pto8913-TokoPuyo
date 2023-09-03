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
	void UpdateWidth(const float& inWidth);
	void UpdateHeight(const float& inHeight);
	void UpdateSize(const float& inWidth, const float& inHeight);

	virtual void Bind(DirectX11& dx) override;
	void Bind(DirectX::XMVECTOR loc);

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

	ID2D1RenderTarget* GetRt2D();
protected:
	const std::wstring fileName;
	float width;
	float height;

	ID2D1Bitmap* m_pBitmap = nullptr;

	IDXGIKeyedMutex* keyedMutex11 = nullptr;
	IDXGIKeyedMutex* keyedMutex10 = nullptr;

	ID2D1RenderTarget* D2DRenderTarget = nullptr;
	ID3D11ShaderResourceView* d2dTexture = nullptr;
};
