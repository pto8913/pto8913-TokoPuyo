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
	virtual ~ScreenTexture();

	static std::shared_ptr<ScreenTexture> Make(DirectX11& dx, const std::wstring& inFileName, FLOAT inWidth, FLOAT inHeight);

	void UpdateTexture(const std::wstring& inFileName);
	void UpdateWidth(const float& inWidth);
	void UpdateHeight(const float& inHeight);
	void UpdateSize(const float& inWidth, const float& inHeight);

	virtual void Bind(DirectX11& dx) override;
	void Bind(const DirectX::XMVECTOR& loc, const float& angle = 0.f);

	static std::string GenerateID(const std::wstring& inFileName, FLOAT, FLOAT)
	{
		using namespace std::string_literals;
		return "ScreenTexture_" + "#"s + std::string("ScreenTexture") + "#"s + Util::w2String(std::wstring(inFileName));
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

	ID2D1Bitmap* pBitmap = nullptr;
	ID2D1RenderTarget* D2DRenderTarget = nullptr;
};
