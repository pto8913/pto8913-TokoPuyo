#pragma once

#include <d2d1.h>

#include "Render/VertexShader.h"

class BlendState;
class PixelShader;
class VertexShader;
class IndexBuffer;
class Topology;
class InputLayout;
class TransformConstantBuffer;
class ScreenTextOnlyOutput;
class SamplerState;
class Rasterizer;

class WidgetManager
{
public:
	virtual ~WidgetManager();

	static WidgetManager& Get();

	void Init(DirectX11& dx);
	bool IsInitialized() const;

	std::shared_ptr<BlendState> pBlendState = nullptr;

	std::shared_ptr<PixelShader> pPixelShader = nullptr;
	std::shared_ptr<VertexShader> pVertexShader = nullptr;

	std::shared_ptr<IndexBuffer> pIndexBuffer = nullptr;
	std::shared_ptr<VertexBuffer<DX::FVertex2D>> pVertexBuffer = nullptr;
	std::shared_ptr<Topology> pTopology = nullptr;

	std::shared_ptr<InputLayout> pInputLayout = nullptr;
	std::shared_ptr<TransformConstantBuffer> pTCB = nullptr;

	std::shared_ptr<ScreenTextOnlyOutput> pScreenTextOnlyOutput = nullptr;
	ID2D1RenderTarget* pRt2D = nullptr;
	IDXGIKeyedMutex* pMutex11 = nullptr;
	IDXGIKeyedMutex* pMutex10 = nullptr;

	std::shared_ptr<SamplerState> pSamplerState = nullptr;
	std::shared_ptr<Rasterizer> pRasterizer = nullptr;

	bool bIsInitialized = false;
};