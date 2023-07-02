#pragma once

#pragma comment (lib, "D2D1.lib")
#pragma comment (lib, "dwrite.lib")

#include "Bindable.h"
#include <dxgi.h>
#include <d2d1.h>
#include <dwrite.h>
#include <sstream>

class DirectX11;

class Texture2D : public Bindable
{
public:
	Texture2D(DirectX11& dx, UINT inWidth, UINT inHeight);
	~Texture2D();

	static std::shared_ptr<Texture2D> Make(DirectX11& dx, UINT inWidth, UINT inHeight);

	virtual void Bind(DirectX11& dx) override;
	void Bind(DirectX11& dx, std::wstring text);

	static std::string GenerateID(UINT inWidth, UINT inHeight)
	{
		inWidth; inHeight;
		using namespace std::string_literals;
		return typeid(Texture2D).name() + "#"s + std::string("ScreenText");
	}
	std::string GetID() const noexcept
	{
		return GenerateID(0, 0);
	}
protected:
	UINT width;
	UINT height;

	IDXGIKeyedMutex* keyedMutex11;
	IDXGIKeyedMutex* keyedMutex10;
	ID2D1RenderTarget* D2DRenderTarget;
	ID2D1SolidColorBrush* Brush;
	ID3D11ShaderResourceView* d2dTexture;
	IDWriteTextFormat* TextFormat;

	std::wstring printText;
};
