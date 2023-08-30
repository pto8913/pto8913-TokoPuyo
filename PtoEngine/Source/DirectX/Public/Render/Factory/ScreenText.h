#pragma once

#include "Bindable.h"

class DirectX11;

class ScreenText : public Bindable
{
public:
	ScreenText(DirectX11& dx, UINT inWidth, UINT inHeight);
	~ScreenText();

	static std::shared_ptr<ScreenText> Make(DirectX11& dx, UINT inWidth, UINT inHeight);

	virtual void Bind(DirectX11& dx) override;
	void Bind(std::wstring text, D2D1_RECT_F rect);

	static std::string GenerateID(UINT inWidth, UINT inHeight)
	{
		inWidth; inHeight;
		using namespace std::string_literals;
		return typeid(ScreenText).name() + "#"s + std::string("ScreenText");
	}
	std::string GetID() const noexcept
	{
		return GenerateID(0, 0);
	}
protected:
	UINT width;
	UINT height;

	IDXGIKeyedMutex* keyedMutex11 = nullptr;
	IDXGIKeyedMutex* keyedMutex10 = nullptr;
	ID2D1RenderTarget* D2DRenderTarget = nullptr;
	ID2D1SolidColorBrush* Brush = nullptr;
	ID3D11ShaderResourceView* d2dTexture = nullptr;
	IDWriteTextFormat* TextFormat = nullptr;

	std::wstring printText;
};

class ScreenTextOnlyOutput : public Bindable
{
public:
	ScreenTextOnlyOutput(DirectX11& dx, UINT inWidth, UINT inHeight);
	~ScreenTextOnlyOutput();

	static std::shared_ptr<ScreenTextOnlyOutput> Make(DirectX11& dx, UINT inWidth, UINT inHeight);

	virtual void Bind(DirectX11& dx) override;

	static std::string GenerateID(UINT inWidth, UINT inHeight)
	{
		inWidth; inHeight;
		using namespace std::string_literals;
		return typeid(ScreenTextOnlyOutput).name() + "#"s + std::string("ScreenTextOnlyOutput");
	}
	std::string GetID() const noexcept
	{
		return GenerateID(0, 0);
	}

	ID2D1RenderTarget*& GetRT2D() noexcept { return D2DRenderTarget; }
	IDXGIKeyedMutex*& GetMutex11() noexcept { return keyedMutex11; }
	IDXGIKeyedMutex*& GetMutex10() noexcept { return keyedMutex10; }
protected:
	UINT width;
	UINT height;

	ID3D11ShaderResourceView* d2dTexture = nullptr;
	ID2D1RenderTarget* D2DRenderTarget = nullptr;
	IDXGIKeyedMutex* keyedMutex11 = nullptr;
	IDXGIKeyedMutex* keyedMutex10 = nullptr;
};
